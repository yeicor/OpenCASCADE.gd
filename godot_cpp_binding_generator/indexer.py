"""
Indexer module: MultiPassIndexer

Performs multi-pass discovery and classification of C++ types from headers using
libclang. It builds a type index with typedefs, aliases, records (classes/structs),
and enums. It also supports resolving typedef chains and classifying arbitrary
type spellings to guide downstream binding generation.

This module prioritizes clarity and extensibility. It intentionally aims for
conservative defaults so generated code compiles even when types are complex or
unknown (falling back to opaque representations).
"""

from __future__ import annotations

import glob
import logging
import re
from typing import Any, Dict, List, Optional, Tuple

from .cpptypes import TypeInfo
from .utils import BUILTIN_TYPE_ALIASES

try:
    from clang import cindex
    from clang.cindex import CursorKind
except Exception as exc:  # pragma: no cover - dependency guard
    raise RuntimeError(
        "clang.cindex (libclang) is required. "
        "Install the Python bindings for clang and ensure libclang is available."
    ) from exc


__all__ = ["MultiPassIndexer"]

logger = logging.getLogger("gdocc.indexer")


class MultiPassIndexer:
    """
    Multi-pass type indexer based on libclang.

    Pass 1 (discover):
        - Parse headers and collect typedefs, using aliases, records (struct/class/template), and enums.
        - Inject built-in primitive aliases.
        - Apply user-provided overrides as early entries.

    Pass 2 (resolution helpers):
        - Resolve typedef/alias chains to terminal spellings.
        - Classify arbitrary spellings to categories used by the emitter/scanner.
    """

    def __init__(
        self,
        index: "cindex.Index",
        clang_args: List[str],
        mapping_overrides: Optional[Dict[str, Any]] = None,
    ) -> None:
        self.index = index
        self.clang_args = list(clang_args or [])
        self.mapping_overrides: Dict[str, Any] = mapping_overrides or {}

        # Central registry of discovered types: fully-qualified name/spelling -> TypeInfo
        self.types: Dict[str, TypeInfo] = {}

    # ---------------- Pass 1: discovery ----------------

    def discover(self, header_files: List[str]) -> None:
        """
        Parse header globs and populate the internal type index.

        header_files:
            A list of glob patterns for C++ header files (e.g., ["inc/**/*.h"]).
        """
        for pattern in header_files or []:
            for path in glob.glob(pattern, recursive=True):
                logger.info("Parsing for discovery: %s", path)
                try:
                    tu = self.index.parse(path, args=self.clang_args)
                except Exception as e:
                    logger.warning("Failed to parse %s: %s", path, e)
                    continue
                if not tu:
                    logger.warning("Failed to parse %s (no translation unit)", path)
                    continue
                self._visit(tu.cursor)

        # Inject built-in aliases (e.g., OCCT typedefs to canonical C++ primitives)
        for alias, target in BUILTIN_TYPE_ALIASES.items():
            if alias not in self.types:
                ti = TypeInfo(alias, "typedef")
                ti.underlying = target
                self.types[alias] = ti

        # Apply user overrides early so classification can resolve them
        if isinstance(self.mapping_overrides, dict):
            for k, v in self.mapping_overrides.items():
                if k not in self.types:
                    ti = TypeInfo(k, "override")
                    if isinstance(v, dict) and "cpp" in v:
                        ti.underlying = v.get("cpp")
                    self.types[k] = ti

    def _visit(self, cursor: Optional["cindex.Cursor"]) -> None:
        """
        Depth-first traversal that collects typedefs/aliases, records and enums
        into the internal type index.
        """
        if cursor is None:
            return
        for c in cursor.get_children():
            try:
                k = c.kind
            except Exception:
                continue

            # Typedefs and using aliases
            if k in (CursorKind.TYPEDEF_DECL, CursorKind.TYPE_ALIAS_DECL):
                name = c.spelling or ""
                if not name:
                    continue
                ti = self.types.get(name) or TypeInfo(name, "typedef", c)
                # Try to discover the underlying type spelling
                underlying: Optional[str] = None
                try:
                    underlying = c.typedef_type.spelling  # type: ignore[attr-defined]
                except Exception:
                    try:
                        underlying = c.type.spelling
                    except Exception:
                        underlying = None
                ti.underlying = underlying
                self.types[name] = ti

            # Records: struct/class/class template declarations
            elif k in (CursorKind.STRUCT_DECL, CursorKind.CLASS_DECL, CursorKind.CLASS_TEMPLATE):
                if not c.spelling:
                    # Skip anonymous classes/structs
                    continue
                fq = self._fq_name(c)
                ti = self.types.get(fq) or TypeInfo(fq, "record", c)
                try:
                    ti.is_definition = c.is_definition()
                except Exception:
                    ti.is_definition = False
                self.types[fq] = ti

            # Enums and their values
            elif k == CursorKind.ENUM_DECL:
                if not c.spelling:
                    continue
                name = c.spelling
                ti = self.types.get(name) or TypeInfo(name, "enum", c)
                values: List[Tuple[str, int]] = []
                for child in c.get_children():
                    if child.kind == CursorKind.ENUM_CONSTANT_DECL:
                        try:
                            values.append((child.spelling, int(child.enum_value)))
                        except Exception:
                            # Fallback to name with zero if value is unavailable
                            values.append((child.spelling, 0))
                ti.enum_values = values
                self.types[name] = ti

            # Descend into other nodes
            else:
                self._visit(c)

    def _fq_name(self, cursor: "cindex.Cursor") -> str:
        """
        Build a fully-qualified name for a cursor by walking up to the translation unit.
        """
        parts: List[str] = []
        cur = cursor
        while cur and cur.kind != CursorKind.TRANSLATION_UNIT:
            if cur.spelling:
                parts.append(cur.spelling)
            cur = cur.semantic_parent
        parts.reverse()
        return "::".join(parts)

    # ---------------- Resolution and classification ----------------

    def resolve_typedef_chain(self, name: str, max_depth: int = 20) -> Optional[str]:
        """
        Follow typedef/alias chains up to a max depth and return the terminal
        spelling, if resolvable. Returns None if 'name' is falsy.

        For unknown names not in the index, the current 'name' is returned to
        allow callers to attempt further heuristics.
        """
        if not name:
            return None
        seen: set[str] = set()
        cur = name
        depth = 0

        while depth < max_depth and cur and cur not in seen:
            seen.add(cur)
            ti = self.types.get(cur)
            if not ti:
                # Not found in index: assume the spelling is terminal/builtin
                return cur
            if ti.kind in ("typedef", "override") and ti.underlying:
                cur = ti.underlying
                depth += 1
                continue
            # Enums or records or templates: terminal
            return ti.name
        return cur

    def classify_spelling(self, spelling: str) -> Dict[str, Any]:
        """
        Classify a type spelling into a normalized category understood by the
        emitter and scanner.

        Returns a dictionary with at least a 'kind' key, one of:
            - 'primitive': {target: <primitive_cpp_spelling>}
            - 'enum': {name: <fq_enum_name>, values: [(name, value), ...]}
            - 'record': {name: <fq_record_name>, def: bool}
            - 'opaque': {name?: <spelling>, note?: <reason>}
            - 'vector': {inner: <spelling>, inner_cls: <classification-of-inner>}
            - 'override': {mapping: <user-provided-mapping-dict>}
        """
        if not spelling:
            return {"kind": "opaque"}

        # Direct user override mapping
        if spelling in self.mapping_overrides:
            return {"kind": "override", "mapping": self.mapping_overrides[spelling]}

        # Resolve through typedef/alias chains
        resolved = self.resolve_typedef_chain(spelling) or spelling

        # Known built-in primitive aliases
        if resolved in BUILTIN_TYPE_ALIASES:
            return {"kind": "primitive", "target": BUILTIN_TYPE_ALIASES[resolved]}

        # Known types in our index
        ti = self.types.get(resolved)
        if ti:
            if ti.kind == "enum":
                return {"kind": "enum", "name": ti.name, "values": ti.enum_values}
            if ti.kind == "record":
                # If we have a definition, consider it forwardable; otherwise opaque
                return {"kind": "record" if ti.is_definition else "opaque", "name": ti.name, "def": ti.is_definition}
            if ti.kind in ("typedef", "override") and ti.underlying:
                # Recursively classify the underlying type
                return self.classify_spelling(ti.underlying)

        # Heuristics for smart pointers and templates common in large C++ libs
        # OCCT Handle<T>, variations, etc.
        if "Handle<" in spelling or "Handle_" in spelling or "::Handle<" in spelling:
            return {"kind": "opaque", "name": spelling, "note": "smartpointer"}

        # std::vector<T> support (conservative; mapped to Variant unless overridden)
        compact = spelling.replace(" ", "")
        if "std::vector<" in compact:
            m = re.search(r"std::vector\s*<\s*([^>]+)\s*>", spelling)
            if m:
                inner = m.group(1).strip()
                inner_cls = self.classify_spelling(inner)
                return {"kind": "vector", "inner": inner, "inner_cls": inner_cls}
            return {"kind": "opaque", "name": spelling, "note": "vector"}

        # Last resort: recognize common builtin names directly
        if spelling in ("int", "float", "double", "bool", "char", "long"):
            return {"kind": "primitive", "target": spelling}

        # Unknown/complex type -> treat as opaque
        return {"kind": "opaque", "name": spelling}
