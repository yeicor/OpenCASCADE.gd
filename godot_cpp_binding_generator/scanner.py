"""
Scanner module: APIScanner

Traverses parsed C++ headers using libclang and extracts a high-level API model
of public classes: methods, constructors, and nested enums. The extracted model
feeds the code emission stage to generate Godot GDExtension bindings.

Design goals:
- Keep the surface small, typed, and easy to grow.
- Be conservative: skip private/protected entities and template specializations
  we cannot reliably handle yet.
- Represent parameter and return types using libclang Type objects where possible
  (emitters can ask the indexer to classify them later).
"""

from __future__ import annotations

import glob
import logging
from dataclasses import dataclass
from typing import Any, Dict, List, Optional, Tuple

from .indexer import MultiPassIndexer
from .utils import collapse_name

try:
    from clang import cindex
    from clang.cindex import CursorKind, AccessSpecifier
except Exception as exc:  # pragma: no cover - dependency guard
    raise RuntimeError(
        "clang.cindex (libclang) is required. "
        "Install the Python bindings for clang and ensure libclang is available."
    ) from exc


__all__ = ["APIScanner"]

logger = logging.getLogger("gdocc.scanner")


@dataclass
class _TypeShim:
    """
    Minimal shim to emulate a clang Type-like object with a 'spelling' attribute.

    This is useful when synthesizing return types that don't naturally exist in
    the AST (e.g., constructor 'return types' that conceptually produce an
    instance of the class).
    """
    spelling: str


class APIScanner:
    """
    Walks the clang AST for provided headers and builds a dictionary of classes
    and their public API. The resulting structure is intentionally simple and
    stable so emitters can consume it without depending on libclang directly.

    Structure of `self.classes`:
        {
            "<flat_class_name>": {
                "fq": "<fully::qualified::name>",
                "methods": [
                    {
                        "name": "<method_name>",
                        "ret": <clang.Type>,
                        "params": [("<param_name>", <clang.Type>), ...],
                        "is_static": bool,
                        "is_const": bool,
                    },
                    ...
                ],
                "constructors": [
                    {
                        "name": "construct",  # synthesized
                        "ret": <_TypeShim of 'fq'>,  # synthesized record 'type'
                        "params": [("<param_name>", <clang.Type>), ...],
                    },
                    ...
                ],
                "enums": [
                    {"name": "<EnumName>", "values": [("<NAME>", int), ...]},
                    ...
                ],
            },
            ...
        }
    """

    def __init__(self, indexer: MultiPassIndexer, clang_args: List[str]) -> None:
        self.indexer = indexer
        self.clang_args = list(clang_args or [])
        self.classes: Dict[str, Dict[str, Any]] = {}

    # ---------------- Public API ----------------

    def scan(self, header_files: List[str]) -> Dict[str, Dict[str, Any]]:
        """
        Parse and traverse header files (supports glob patterns) and populate
        `self.classes` with discovered public classes and their members.

        Returns the populated classes dictionary for convenience.
        """
        for pattern in header_files or []:
            for path in glob.glob(pattern, recursive=True):
                logger.info("Scanning API in: %s", path)
                try:
                    tu = self.indexer.index.parse(path, args=self.clang_args)
                except Exception as e:
                    logger.warning("Failed to parse for API %s: %s", path, e)
                    continue
                if not tu:
                    logger.warning("Failed to parse for API %s (no translation unit)", path)
                    continue
                self._walk(tu.cursor)
        return self.classes

    # ---------------- Internal traversal ----------------

    def _walk(self, cursor: Optional["cindex.Cursor"], namespace_prefix: Optional[str] = None) -> None:
        """
        Depth-first traversal that collects public class declarations, methods,
        constructors, and nested enums.
        """
        if cursor is None:
            return
        for c in cursor.get_children():
            try:
                k = c.kind
            except Exception:
                continue

            if k in (CursorKind.CLASS_DECL, CursorKind.STRUCT_DECL, CursorKind.CLASS_TEMPLATE):
                if not getattr(c, "spelling", None):
                    continue
                result = self._process_class_decl(c)
                if result is None:
                    continue
                flat, cls = result
                self.classes[flat] = cls
                continue

            # Recurse into non-class nodes
            self._walk(c, namespace_prefix)

    def _process_class_decl(self, cursor: "cindex.Cursor") -> Optional[Tuple[str, Dict[str, Any]]]:
        """
        Build the class API model from a class/struct/template cursor.
        """
        fq = self._fq(cursor)
        if not fq:
            return None
        flat = collapse_name(fq)
        logger.info("API candidate class: %s", fq)

        cls: Dict[str, Any] = {"fq": fq, "methods": [], "constructors": [], "enums": []}

        for child in cursor.get_children():
            if child.kind == CursorKind.CXX_METHOD and self._is_public(child):
                method = self._collect_method(child)
                if method is not None:
                    cls["methods"].append(method)
                continue

            if child.kind == CursorKind.CONSTRUCTOR and self._is_public(child):
                ctor = self._collect_constructor(child, fq)
                if ctor is not None:
                    cls["constructors"].append(ctor)
                continue

            if child.kind == CursorKind.ENUM_DECL:
                enum_info = self._collect_enum(child)
                if enum_info is not None:
                    cls["enums"].append(enum_info)
                continue

        return flat, cls

    def _is_public(self, cursor: "cindex.Cursor") -> bool:
        """
        Check if a cursor has public access (defensively).
        """
        try:
            return getattr(cursor, "access_specifier", None) == AccessSpecifier.PUBLIC
        except Exception:
            return False

    def _collect_params(self, fn_cursor: "cindex.Cursor") -> List[Tuple[str, Any]]:
        """
        Extract parameters as (name, type) tuples. Falls back gracefully.
        """
        params: List[Tuple[str, Any]] = []
        try:
            for i, p in enumerate(fn_cursor.get_arguments()):
                pname = getattr(p, "spelling", None) or f"arg{i}"
                ptype = getattr(p, "type", None) or _TypeShim("Variant")  # type: ignore[assignment]
                params.append((pname, ptype))
        except Exception:
            # Best-effort: keep empty params if we can't retrieve them
            return []
        return params

    def _safe_result_type(self, fn_cursor: "cindex.Cursor") -> Any:
        """
        Get result type with a safe fallback.
        """
        try:
            return fn_cursor.result_type
        except Exception:
            return _TypeShim("Variant")  # type: ignore[assignment]

    def _collect_method(self, method_cursor: "cindex.Cursor") -> Optional[Dict[str, Any]]:
        """
        Collect method information into a serializable dict.
        """
        try:
            is_static = False
            is_const = False
            try:
                is_static = bool(method_cursor.is_static_method())
            except Exception:
                pass
            try:
                is_const = bool(method_cursor.is_const_method())
            except Exception:
                pass

            params = self._collect_params(method_cursor)
            ret_type = self._safe_result_type(method_cursor)

            return {
                "name": method_cursor.spelling,
                "ret": ret_type,
                "params": params,
                "is_static": is_static,
                "is_const": is_const,
            }
        except Exception:
            return None

    def _collect_constructor(self, ctor_cursor: "cindex.Cursor", fq: str) -> Optional[Dict[str, Any]]:
        """
        Collect constructor into a synthesized callable-like dict.
        """
        try:
            params = self._collect_params(ctor_cursor)
            return {"name": "construct", "ret": _TypeShim(fq), "params": params}
        except Exception:
            return None

    def _collect_enum(self, enum_cursor: "cindex.Cursor") -> Optional[Dict[str, Any]]:
        """
        Collect enum name and its constant values.
        """
        try:
            vals: List[Tuple[str, int]] = []
            for gc in enum_cursor.get_children():
                if gc.kind == CursorKind.ENUM_CONSTANT_DECL:
                    try:
                        vals.append((gc.spelling, int(gc.enum_value)))
                    except Exception:
                        vals.append((gc.spelling, 0))
            return {"name": enum_cursor.spelling, "values": vals}
        except Exception:
            return None

    def _fq(self, cursor: "cindex.Cursor") -> str:
        """
        Build a fully qualified name (ns::Type::Inner) by walking up the AST to
        the translation unit.
        """
        parts: List[str] = []
        cur = cursor
        while cur and cur.kind != CursorKind.TRANSLATION_UNIT:
            if getattr(cur, "spelling", None):
                parts.append(cur.spelling)
            cur = getattr(cur, "semantic_parent", None)
        parts.reverse()
        return "::".join(parts)
