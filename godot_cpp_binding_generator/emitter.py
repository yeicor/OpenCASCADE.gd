"""
Emitter module: GDEmitter

Generates Godot GDExtension C++ wrapper code for discovered C++ classes and their
APIs. This module focuses on clean, conservative code generation that compiles
and provides a solid foundation for iterative enhancement via mapping overrides.

Design choices:
- Unknown or complex types are treated as opaque (Variant or void*), ensuring the
  generated code compiles even without full type knowledge.
- Overloaded methods are disambiguated with a short hash suffix derived from the
  parameter type spellings.
- Registration and utility code is centralized and generated alongside wrappers.

This emitter intentionally prefers clarity and extensibility over feature completeness.
"""
from __future__ import annotations

import os
import textwrap
import logging
from typing import Any, Dict, List, Optional, Tuple

from .indexer import MultiPassIndexer
from .utils import sig_hash

logger = logging.getLogger("gdocc.emitter")

__all__ = ["GDEmitter"]


class GDEmitter:
    """
    Code emitter for Godot GDExtension C++ wrappers.

    Usage:
        emitter = GDEmitter(indexer, prefix="ocgd", godot_base="Object")
        emitter.emit_all(api_scanner, out_dir="out", impl_includes=headers_if_desired)
    """

    def __init__(self, indexer: MultiPassIndexer, prefix: str = "ocgd", godot_base: str = "Object"):
        self.indexer = indexer
        self.prefix = prefix
        self.godot_base = godot_base

    # ---------------- Public API ----------------

    def emit_all(self, api: Any, out_dir: str, impl_includes: Optional[List[str]] = None) -> None:
        """
        Emit all code artifacts:
        - <prefix>_utils.h: Utility helpers
        - <prefix>_register.h/.cpp: Registration boilerplate
        - <prefix>_<Class>.hpp/.cpp: Wrapper classes for each discovered class
        """
        os.makedirs(out_dir, exist_ok=True)

        # Utils header
        utils_path = os.path.join(out_dir, f"{self.prefix}_utils.h")
        with open(utils_path, "w", encoding="utf-8") as fh:
            fh.write(self._utils_header())

        # Registration unit
        register_header_path = os.path.join(out_dir, f"{self.prefix}_register.h")
        with open(register_header_path, "w", encoding="utf-8") as fh:
            fh.write(self._register(header=True, api=api))
        register_path = os.path.join(out_dir, f"{self.prefix}_register.cpp")
        with open(register_path, "w", encoding="utf-8") as fh:
            fh.write(self._register(header=False, api=api))

        # Wrappers
        for flat, cls in (api.classes or {}).items():
            h_text, cpp_text = self._emit_wrapper(flat, cls, impl_includes)
            h_path = os.path.join(out_dir, f"{self.prefix}_{flat}.hpp")
            cpp_path = os.path.join(out_dir, f"{self.prefix}_{flat}.cpp")
            with open(h_path, "w", encoding="utf-8") as fh:
                fh.write(h_text)
            with open(cpp_path, "w", encoding="utf-8") as fh:
                fh.write(cpp_text)
            logger.info("Emitted wrapper for %s", cls.get("fq", flat))

    # ---------------- Internal helpers ----------------

    def _utils_header(self) -> str:
        return textwrap.dedent(
            f"""
            // {self.prefix}_utils.h - generated helper for Godot GDExtension (Godot 4.4)
            #pragma once

            #include <gdextension_interface.h>
            #include <godot_cpp/core/class_db.hpp>
            #include <godot_cpp/core/defs.hpp>
            #include <godot_cpp/godot.hpp>
            #include <godot_cpp/variant/variant.hpp>
            #include <string>

            using namespace godot;

            // Basic helpers: convert std::string <-> String
            static String _string_to_godot(const std::string &s) {{ return String::utf8(s.c_str()); }}
            static std::string _string_from_godot(const String &s) {{ return std::string(s.utf8().get_data()); }}

            // TODO: add PackedArray <-> std::vector helpers here as you enable vector mappings

            """
        ).lstrip()

    def _register(self, header: bool, api: Any) -> str:
        init_fn = f"{self.prefix}_gd_library_init_bindings"

        # Merge header and implementation: emit a header-only registration unit.
        # The .cpp simply includes this header to avoid duplication.
        if not header:
            return f'#include "{self.prefix}_register.h"\n'

        lines: List[str] = []
        lines.append("#pragma once")
        lines.append(f'#include "{self.prefix}_utils.h"')
        lines.append("// Generated registration (header-only)")
        for flat, _ in (api.classes or {}).items():
            lines.append(f'#include "{self.prefix}_{flat}.hpp"')
        lines.append("")

        # Definitions
        lines.append(
            "void initialize_gdextension_types([[maybe_unused]] ModuleInitializationLevel p_level) {"
        )
        lines.append(
            "    if (p_level < MODULE_INITIALIZATION_LEVEL_SCENE) return; // Only initialize at the SCENE level or higher."
        )
        for flat in (api.classes or {}).keys():
            cls_name = f"{self.prefix}_{flat}"
            lines.append(f"    GDREGISTER_CLASS({cls_name});")
        lines.append("}")
        lines.append("")
        lines.append(
            "void uninitialize_gdextension_types([[maybe_unused]] ModuleInitializationLevel p_level) {"
        )
        lines.append("}")
        lines.append("")
        lines.append("extern \"C\" {")
        lines.append(
            f"GDExtensionBool GDE_EXPORT {init_fn}(GDExtensionInterfaceGetProcAddress p_get_proc_address,"
        )
        lines.append("                                     GDExtensionClassLibraryPtr p_library,")
        lines.append("                                     GDExtensionInitialization *r_initialization) {")
        lines.append("    GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);")
        lines.append("    init_obj.register_initializer(initialize_gdextension_types);")
        lines.append("    init_obj.register_terminator(uninitialize_gdextension_types);")
        lines.append("    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);")
        lines.append("")
        lines.append("    return init_obj.init();")
        lines.append("}")
        lines.append("}")
        return "\n".join(lines) + "\n"

    def _map_clang_type(self, t: Any) -> Dict[str, Any]:
        """
        Map a clang Type-like object to a classification understood by the emitter.

        Expects an object with a 'spelling' attribute (duck-typed).
        """
        spelling = getattr(t, "spelling", None)
        cls = self.indexer.classify_spelling(spelling or "")
        if cls.get("kind") == "override":
            return {"kind": "override", "mapping": cls.get("mapping")}
        return cls

    def _cpp_type_for_map(self, mapped: Dict[str, Any]) -> str:
        kind = mapped.get("kind")
        if kind == "primitive":
            return mapped.get("target", "int")
        if kind == "enum":
            return "int"
        if kind == "record":
            # represent as void* at wrapper ABI boundary; internal forwarding casts
            return "void *"
        if kind == "vector":
            return "Variant"
        if kind == "override":
            return mapped.get("mapping", {}).get("cpp", "Variant")
        # Fallback: Variant keeps things compilable
        return "Variant"

    def _emit_wrapper(self, flat: str, cls: Dict[str, Any], impl_includes: Optional[List[str]]) -> Tuple[str, str]:
        """
        Emit a pair of (header, cpp) strings for a single class entry.
        """
        fq = cls.get("fq", flat)
        wrapper = f"{self.prefix}_{flat}"

        # ---------------- Header ----------------
        h: List[str] = [f"#pragma once", f'#include "{self.prefix}_utils.h"']
        h.append("")
        h.append(f"class {wrapper} : public {self.godot_base} " + "{")
        h.append(f"    GDCLASS({wrapper}, {self.godot_base})")
        h.append("public:")
        h.append(f"    {wrapper}();")
        h.append(f"    virtual ~{wrapper}();")
        h.append("")
        # native pointer holder
        h.append("    // native pointer to underlying C++ object (opaque).")
        h.append("    void *native = nullptr;")
        h.append("    bool owned = false;")
        h.append("")

        # Method declarations
        for m in cls.get("methods", []):
            self._emit_wrapper_method_header(h, m)

        # Constructor overloads modeled as "construct" methods
        for ctor in cls.get("constructors", []):
            self._emit_wrapper_ctor_header(h, ctor)

        # Explicit destroy method
        h.append("")
        h.append("    void destroy();")
        h.append("")
        h.append("    static void _bind_methods();")
        h.append("};")

        header_text = "\n".join(h) + "\n"

        # ---------------- CPP ----------------
        cpp: List[str] = [f'#include "{wrapper}.hpp"']
        cpp.append("")
        # Optionally include the original headers so we can reference the real C++ types
        if impl_includes:
            for inc in impl_includes:
                cpp.append(f'#include "{inc}"')
            cpp.append("")

        cpp.append("using namespace godot;")
        cpp.append("")

        methods = list(cls.get("methods", []))
        ctors = list(cls.get("constructors", []))

        # Bind methods
        cpp.append(f"void {wrapper}::_bind_methods() " + "{")
        # Regular methods
        for m in methods:
            arg_names = [p[0] or f"arg{i}" for i, p in enumerate(m.get("params", []))]
            dargs = ", ".join('"' + n + '"' for n in arg_names) if arg_names else ""
            arg_types = [getattr(pt, "spelling", "Variant") for (_, pt) in m.get("params", [])]
            suffix = "_" + sig_hash(arg_types) if arg_types else ""
            method_name = m.get("name", "method") + suffix
            if dargs:
                cpp.append(
                    f'    ClassDB::bind_method(D_METHOD("{m.get("name", "method")}", {dargs}), &{wrapper}::{method_name});'
                )
            else:
                cpp.append(
                    f'    ClassDB::bind_method(D_METHOD("{m.get("name", "method")}"), &{wrapper}::{method_name});'
                )
        # Constructor "construct" methods
        for m in ctors:
            arg_names = [p[0] or f"arg{i}" for i, p in enumerate(m.get("params", []))]
            dargs = ", ".join('"' + n + '"' for n in arg_names) if arg_names else ""
            arg_types = [getattr(pt, "spelling", "Variant") for (_, pt) in m.get("params", [])]
            suffix = "_" + sig_hash(arg_types) if arg_types else ""
            method_name = "construct" + suffix
            if dargs:
                cpp.append(
                    f'    ClassDB::bind_method(D_METHOD("construct", {dargs}), &{wrapper}::{method_name});'
                )
            else:
                cpp.append(
                    f'    ClassDB::bind_method(D_METHOD("construct"), &{wrapper}::{method_name});'
                )
        # bind explicit destroy
        cpp.append(f'    ClassDB::bind_method(D_METHOD("destroy"), &{wrapper}::destroy);')
        cpp.append("}")
        cpp.append("")

        # ctor/dtor
        cpp.append(f"{wrapper}::{wrapper}() " + "{}")
        cpp.append(f"{wrapper}::~{wrapper}() " + "{")
        cpp.append("    if (this->native && this->owned) {")
        cpp.append("        this->destroy();")
        cpp.append("    }")
        cpp.append("}")
        cpp.append("")

        # Method bodies
        # Regular methods (implement returning actual values when possible)
        for m in methods:
            # Return type mapping
            mapped_ret = self._map_clang_type(m.get("ret"))
            ret_cpp = self._cpp_type_for_map(mapped_ret) if mapped_ret.get("kind") in (
                "primitive",
                "enum",
                "record",
                "override",
                "vector",
            ) else "void"

            # Param mapping
            param_sig: List[str] = []
            conv_lines: List[str] = []
            call_args: List[str] = []
            for i, (pname, ptype) in enumerate(m.get("params", [])):
                pm = self._map_clang_type(ptype)
                pname_safe = pname or f"arg{i}"
                if pm.get("kind") == "primitive":
                    cpp_type = pm["target"]
                    param_sig.append(f"{cpp_type} {pname_safe}")
                    call_args.append(pname_safe)
                elif pm.get("kind") == "enum":
                    param_sig.append(f"int {pname_safe}")
                    call_args.append(pname_safe)
                elif pm.get("kind") == "record":
                    # Pass underlying pointer
                    param_sig.append(f"void * {pname_safe}")
                    target_name = pm.get("name", "void")
                    call_args.append(f"reinterpret_cast<{target_name}*>({pname_safe})")
                elif pm.get("kind") == "vector":
                    # Accept PackedArray/Variant in Godot; conversion is a TODO
                    param_sig.append(f"Variant {pname_safe} /* PackedArray expected */")
                    conv_lines.append(
                        f"    // TODO: convert PackedArray/Variant to std::vector<{pm.get('inner', 'T')}> for {pname_safe}"
                    )
                    call_args.append(f"/* converted_{pname_safe} */")
                elif pm.get("kind") == "override":
                    # user supplied mapping (expects 'cpp' to be a C++ type)
                    cpp_t = pm.get("mapping", {}).get("cpp", "Variant")
                    param_sig.append(f"{cpp_t} {pname_safe}")
                    call_args.append(pname_safe)
                else:
                    # Unknown -> accept Variant to keep things compilable
                    param_sig.append(f"Variant {pname_safe} /* unsupported type: {getattr(ptype, 'spelling', 'unknown')} */")
                    call_args.append(pname_safe)

            arg_types = [getattr(pt, "spelling", "Variant") for (_, pt) in m.get("params", [])]
            suffix = "_" + sig_hash(arg_types) if arg_types else ""
            method_name = m.get("name", "method") + suffix
            sig = f"{ret_cpp} {wrapper}::{method_name}({', '.join(param_sig)})"

            body: List[str] = [sig + " {"]
            body.extend(conv_lines)

            # Helper: default return value
            def_ret = self._default_return_for(ret_cpp)

            # Forward call if possible
            if fq in self.indexer.types and self.indexer.types[fq].kind == "record":
                call_target = f"reinterpret_cast<{fq}*>(this->native)"
                body.append(f'    if (!this->native) {{ ERR_PRINT("Native pointer is null for {fq}");')
                if ret_cpp != "void":
                    body.append(f"        return {def_ret};")
                else:
                    body.append("        return;")
                body.append("    }")

                call_expr = f"{call_target}->{m.get('name', 'method')}({', '.join(call_args)})"
                # Implement concrete returns where possible
                kind = mapped_ret.get("kind")
                if ret_cpp == "void":
                    body.append(f"    {call_expr};")
                elif kind == "primitive":
                    body.append(f"    auto _ret = {call_expr};")
                    body.append("    return _ret;")
                elif kind == "enum":
                    body.append(f"    auto _ret = {call_expr};")
                    body.append("    return static_cast<int>(_ret);")
                elif kind == "override":
                    body.append(f"    auto _ret = {call_expr};")
                    body.append("    return _ret;")
                elif kind == "record":
                    ret_spell = getattr(m.get("ret"), "spelling", "")
                    body.append(f"    auto _ret = {call_expr};")
                    if "*" in (ret_spell or ""):
                        body.append("    return reinterpret_cast<void*>(_ret);")
                    else:
                        body.append("    // Returning record by value is not supported; return nullptr for now.")
                        body.append("    return nullptr;")
                elif kind == "vector":
                    body.append(f"    (void){call_expr};")
                    body.append(f"    return Variant(); // TODO: convert std::vector to Godot type")
                else:
                    body.append(f"    (void){call_expr};")
                    if ret_cpp != "void":
                        body.append(f"    return {def_ret};")
            else:
                body.append(f'    ERR_PRINT("Automatic forwarding not available for {fq}::{m.get("name", "method")}");')
                if ret_cpp != "void":
                    body.append(f"    return {def_ret};")

            body.append("}")
            cpp.append("\n".join(body))
            cpp.append("")

        # Constructor bodies (construct_* methods)
        for m in ctors:
            # Param mapping
            param_sig: List[str] = []
            conv_lines: List[str] = []
            call_args: List[str] = []
            for i, (pname, ptype) in enumerate(m.get("params", [])):
                pm = self._map_clang_type(ptype)
                pname_safe = pname or f"arg{i}"
                if pm.get("kind") == "primitive":
                    cpp_type = pm["target"]
                    param_sig.append(f"{cpp_type} {pname_safe}")
                    call_args.append(pname_safe)
                elif pm.get("kind") == "enum":
                    param_sig.append(f"int {pname_safe}")
                    call_args.append(pname_safe)
                elif pm.get("kind") == "record":
                    param_sig.append(f"void * {pname_safe}")
                    target_name = pm.get("name", "void")
                    call_args.append(f"reinterpret_cast<{target_name}*>({pname_safe})")
                elif pm.get("kind") == "vector":
                    param_sig.append(f"Variant {pname_safe} /* PackedArray expected */")
                    conv_lines.append(
                        f"    // TODO: convert PackedArray/Variant to std::vector<{pm.get('inner', 'T')}> for {pname_safe}"
                    )
                    call_args.append(f"/* converted_{pname_safe} */")
                elif pm.get("kind") == "override":
                    cpp_t = pm.get("mapping", {}).get("cpp", "Variant")
                    param_sig.append(f"{cpp_t} {pname_safe}")
                    call_args.append(pname_safe)
                else:
                    param_sig.append(f"Variant {pname_safe} /* unsupported type: {getattr(ptype, 'spelling', 'unknown')} */")
                    call_args.append(pname_safe)

            arg_types = [getattr(pt, "spelling", "Variant") for (_, pt) in m.get("params", [])]
            suffix = "_" + sig_hash(arg_types) if arg_types else ""
            method_name = "construct" + suffix
            sig = f"void {wrapper}::{method_name}({', '.join(param_sig)})"

            body: List[str] = [sig + " {"]
            body.extend(conv_lines)
            body.append("    if (this->native && this->owned) {")
            body.append(f"        delete reinterpret_cast<{fq}*>(this->native);")
            body.append("        this->native = nullptr;")
            body.append("        this->owned = false;")
            body.append("    }")
            body.append(f"    this->native = reinterpret_cast<void*>(new {fq}({', '.join(call_args)}));")
            body.append("    this->owned = true;")
            body.append("}")
            cpp.append("\n".join(body))
            cpp.append("")

        # explicit destroy()
        cpp.append(f"void {wrapper}::destroy() " + "{")
        cpp.append("    if (this->native && this->owned) {")
        cpp.append(f"        delete reinterpret_cast<{fq}*>(this->native);")
        cpp.append("    }")
        cpp.append("    this->native = nullptr;")
        cpp.append("    this->owned = false;")
        cpp.append("}")
        cpp.append("")

        return header_text, "\n".join(cpp)

    def _emit_wrapper_method_header(self, h: List[str], m: Dict[str, Any]) -> str:
        """
        Append a single method declaration to the header buffer.
        """
        mapped = self._map_clang_type(m.get("ret"))
        if mapped.get("kind") in ("primitive", "enum", "record", "override", "vector"):
            cpp_ret = self._cpp_type_for_map(mapped)
        else:
            # unsupported -> return void and be explicit
            cpp_ret = "void"

        # Parameters
        param_decls: List[str] = []
        for i, (pname, ptype) in enumerate(m.get("params", [])):
            pm = self._map_clang_type(ptype)
            pname_safe = pname or f"arg{i}"
            if pm.get("kind") in ("primitive", "record", "enum", "override", "vector"):
                cpp_param = self._cpp_type_for_map(pm)
                param_decls.append(f"{cpp_param} {pname_safe}")
            else:
                # keep it generic to ensure we can compile
                param_decls.append(
                    f"Variant {pname_safe} /* unsupported type: {getattr(ptype, 'spelling', 'unknown')} */"
                )

        # Name disambiguation for overloads
        arg_types = [getattr(pt, "spelling", "Variant") for (_, pt) in m.get("params", [])]
        suffix = "_" + sig_hash(arg_types) if arg_types else ""
        method_name = m.get("name", "method") + suffix

        h.append(f"    {cpp_ret} {method_name}({', '.join(param_decls)});")
        return method_name

    def _emit_wrapper_ctor_header(self, h: List[str], m: Dict[str, Any]) -> str:
        """
        Append a constructor wrapper declaration ('construct_*') to the header buffer.
        """
        # Parameters
        param_decls: List[str] = []
        for i, (pname, ptype) in enumerate(m.get("params", [])):
            pm = self._map_clang_type(ptype)
            pname_safe = pname or f"arg{i}"
            if pm.get("kind") in ("primitive", "record", "enum", "override", "vector"):
                cpp_param = self._cpp_type_for_map(pm)
                param_decls.append(f"{cpp_param} {pname_safe}")
            else:
                param_decls.append(
                    f"Variant {pname_safe} /* unsupported type: {getattr(ptype, 'spelling', 'unknown')} */"
                )

        arg_types = [getattr(pt, "spelling", "Variant") for (_, pt) in m.get("params", [])]
        suffix = "_" + sig_hash(arg_types) if arg_types else ""
        method_name = "construct" + suffix
        h.append(f"    void {method_name}({', '.join(param_decls)});")
        return method_name

    # ---------------- Small utilities ----------------

    @staticmethod
    def _default_return_for(ret_cpp: str) -> str:
        """
        Compute a conservative default return expression for a given C++ type name.
        This is used when forwarding is not possible or return conversions are TODO.
        """
        t = ret_cpp.replace("const ", "").strip()
        if t.endswith("*"):
            return "nullptr"
        if t == "void":
            return ""
        # Basic types
        if t in ("bool",):
            return "false"
        if t in ("float", "double"):
            return "0.0"
        if t.startswith("unsigned ") or t in ("int", "long", "short", "char", "size_t", "ptrdiff_t", "long long"):
            return "0"
        if "Variant" in t:
            return "Variant()"
        if t == "void *":
            return "nullptr"
        # Fallback
        return "0"
