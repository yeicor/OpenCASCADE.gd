#!/usr/bin/env python3
"""
generate_gdextension_occ_multipass.py

Multi-pass, automated GDExtension binding generator designed to work
with large, template-heavy C++ libraries (example target: OpenCASCADE).

Goals (implemented):
 - Single-command automation: give it a list of headers (+ include dirs) and
   it will parse, index and classify types across all headers, then generate
   Godot 4.4 GDExtension wrapper classes that are as complete as reasonably
   possible without manual intervention.
 - Multi-pass approach:
     * Pass 1: parse headers, discover typedefs, classes, enums and using aliases
     * Pass 2: resolve typedef chains and classify types (primitive, enum, record,
       template, opaque)
     * Pass 3: extract API (public classes/methods) and map parameter/return types
     * Pass 4: emit wrapper code (headers + .cpp + registration)
 - Conservative defaults: unknown or complex types become opaque `void *`
   handles so generated code compiles and gives you a surface to iteratively
   improve by providing mapping overrides.
 - JSON mapping overrides: teach the generator about domain types (e.g. OCCT)
   for richer conversions.

Notes:
 - This generator is opinionated: it aims to produce compileable, safe
   skeletons that can be re-run as you gradually improve the mapping.json.
 - You should pass --include-originals to include the original headers in the
   generated implementation files (recommended for OCCT so the compiler can
   see the real definitions).

Usage example:
  python3 generate_gdextension_occ_multipass.py \
      --headers /path/to/occt/inc/*.h \
      -I /path/to/occt/inc -o out --prefix occ --include-originals \
      --map occ_mappings.json --libclang-path /usr/lib/llvm-14/lib

"""

from __future__ import annotations
import argparse
import os
import textwrap
import json
import re
import logging
import glob
import hashlib
from typing import List, Optional, Tuple, Dict, Any

try:
    from clang import cindex
    from clang.cindex import CursorKind, TypeKind
except Exception:
    raise RuntimeError("clang.cindex (libclang) required — install clang python bindings and libclang.")

# ---------------- logging ----------------
logging.basicConfig(level=logging.INFO, format='[%(levelname)s] %(message)s')
logger = logging.getLogger('gdocc')


# ---------------- utilities ----------------

def safe_ident(s: str) -> str:
    return ''.join(c if (c.isalnum() or c == '_') else '_' for c in s)


def collapse_name(name: str) -> str:
    return safe_ident(name.replace('::', '_'))


def sig_hash(list_of_types: List[str]) -> str:
    m = hashlib.sha1('\n'.join(list_of_types).encode('utf-8'))
    return m.hexdigest()[:8]


# ---------------- built-in primitive mapping (common for OCCT)
BUILTIN_TYPE_ALIASES = {
    'Standard_Integer': 'int',
    'Standard_Real': 'double',
    'Standard_Boolean': 'bool',
    'int': 'int', 'long': 'long', 'float': 'float', 'double': 'double', 'bool': 'bool',
}


# ---------------- Type index ----------------
class TypeInfo:
    def __init__(self, name: str, kind: str, cursor=None):
        self.name = name
        self.kind = kind  # 'typedef','record','enum','alias','template','unknown'
        self.cursor = cursor
        self.underlying: Optional[str] = None  # for typedef/alias
        self.is_definition = False
        self.enum_values: List[Tuple[str, int]] = []


class MultiPassIndexer:
    def __init__(self, index: cindex.Index, clang_args: List[str], mapping_overrides: Optional[Dict[str, Any]] = None):
        self.index = index
        self.clang_args = clang_args
        self.types: Dict[str, TypeInfo] = {}
        self.mapping_overrides = mapping_overrides or {}

    def discover(self, header_files: List[str]):
        """Pass 1: parse all headers and collect typedefs, records, enums and aliases."""
        for pattern in header_files:
            for path in glob.glob(pattern):
                logger.info('Parsing for discovery: %s', path)
                tu = self.index.parse(path, args=['-x', 'c++', '-std=c++17'] + self.clang_args)
                if not tu:
                    logger.warning('Failed to parse %s', path)
                    continue
                self._visit(tu.cursor)
        # inject builtin aliases
        for k, v in BUILTIN_TYPE_ALIASES.items():
            if k not in self.types:
                ti = TypeInfo(k, 'typedef')
                ti.underlying = v
                self.types[k] = ti
        # apply user overrides early
        if isinstance(self.mapping_overrides, dict):
            for k, v in self.mapping_overrides.items():
                if k not in self.types:
                    ti = TypeInfo(k, 'override')
                    ti.underlying = v.get('cpp') if isinstance(v, dict) and 'cpp' in v else None
                    self.types[k] = ti

    def _visit(self, cursor):
        for c in cursor.get_children():
            try:
                k = c.kind
            except Exception:
                continue
            # typedefs and using aliases
            if k == CursorKind.TYPEDEF_DECL or k == CursorKind.TYPE_ALIAS_DECL:
                name = c.spelling
                ti = self.types.get(name) or TypeInfo(name, 'typedef', c)
                # try to read underlying type spelling
                try:
                    underlying = c.typedef_type.spelling
                except Exception:
                    try:
                        underlying = c.type.spelling
                    except Exception:
                        underlying = None
                ti.underlying = underlying
                self.types[name] = ti
            # records
            elif k in (CursorKind.STRUCT_DECL, CursorKind.CLASS_DECL, CursorKind.CLASS_TEMPLATE):
                if not c.spelling:
                    # anonymous
                    continue
                name = fq = self._fq_name(c)
                ti = self.types.get(fq) or TypeInfo(fq, 'record', c)
                try:
                    ti.is_definition = c.is_definition()
                except Exception:
                    ti.is_definition = False
                self.types[fq] = ti
            elif k == CursorKind.ENUM_DECL:
                if not c.spelling:
                    continue
                name = c.spelling
                ti = self.types.get(name) or TypeInfo(name, 'enum', c)
                ti.enum_values = []
                for child in c.get_children():
                    if child.kind == CursorKind.ENUM_CONSTANT_DECL:
                        ti.enum_values.append((child.spelling, child.enum_value))
                self.types[name] = ti
            else:
                # descend
                self._visit(c)

    def _fq_name(self, cursor):
        parts = []
        cur = cursor
        while cur and cur.kind != CursorKind.TRANSLATION_UNIT:
            if cur.spelling:
                parts.append(cur.spelling)
            cur = cur.semantic_parent
        parts.reverse()
        return '::'.join(parts)

    # ---------------- resolution ----------------
    def resolve_typedef_chain(self, name: str, max_depth: int = 20) -> Optional[str]:
        """Follow typedef/alias chains to a terminal spelling (primitive/record/etc.)"""
        if not name:
            return None
        seen = set()
        cur = name
        depth = 0
        while depth < max_depth and cur and cur not in seen:
            seen.add(cur)
            ti = self.types.get(cur)
            if not ti:
                # not found in index: if it's a builtin type spelling, return it
                return cur
            if ti.kind in ('typedef', 'override') and ti.underlying:
                cur = ti.underlying
                depth += 1
                continue
            # enums or records or templates: terminal
            return ti.name
        return cur

    def classify_spelling(self, spelling: str) -> Dict[str, Any]:
        """Return a classification dict for a spelling.

        result keys: kind in {'primitive','enum','record','opaque','template','alias'}
        and optional 'target' for primitive mapping
        """
        if not spelling:
            return {'kind': 'opaque'}
        # exact overrides from user mapping
        if spelling in self.mapping_overrides:
            return {'kind': 'override', 'mapping': self.mapping_overrides[spelling]}
        # resolve chains
        resolved = self.resolve_typedef_chain(spelling) or spelling
        if resolved in BUILTIN_TYPE_ALIASES:
            return {'kind': 'primitive', 'target': BUILTIN_TYPE_ALIASES[resolved]}
        if resolved in self.types:
            ti = self.types[resolved]
            if ti.kind == 'enum':
                return {'kind': 'enum', 'name': resolved, 'values': ti.enum_values}
            if ti.kind == 'record':
                # if we have a definition, we can attempt to forward; otherwise treat as opaque
                if ti.is_definition:
                    return {'kind': 'record', 'name': resolved, 'def': True}
                else:
                    return {'kind': 'opaque', 'name': resolved}
            if ti.kind in ('typedef', 'override') and ti.underlying:
                # recursive classify underlying
                return self.classify_spelling(ti.underlying)
        # heuristics for templates / smart pointers
        if 'Handle<' in spelling or 'Handle_' in spelling or '::Handle<' in spelling:
            return {'kind': 'opaque', 'note': 'smartpointer'}
        if 'std::vector<' in spelling.replace(' ', ''):
            # try to extract inner
            m = re.search(r'std::vector\s*<\s*([^>]+)\s*>', spelling)
            if m:
                inner = m.group(1).strip()
                inner_cls = self.classify_spelling(inner)
                return {'kind': 'vector', 'inner': inner, 'inner_cls': inner_cls}
            return {'kind': 'opaque', 'note': 'vector'}
        # last resort: if spelling looks like builtin
        if spelling in ('int', 'float', 'double', 'bool', 'char', 'long'):
            return {'kind': 'primitive', 'target': spelling}
        # unknown -> opaque
        return {'kind': 'opaque', 'name': spelling}


# ---------------- API extraction ----------------
class APIScanner:
    def __init__(self, indexer: MultiPassIndexer, clang_args: List[str]):
        self.indexer = indexer
        self.clang_args = clang_args
        self.classes: Dict[str, Dict] = {}

    def scan(self, header_files: List[str]):
        for pattern in header_files:
            for path in glob.glob(pattern):
                logger.info('Scanning API in: %s', path)
                tu = self.indexer.index.parse(path, args=['-x', 'c++', '-std=c++17'] + self.clang_args)
                if not tu:
                    logger.warning('Failed to parse for API %s', path)
                    continue
                self._walk(tu.cursor)

    def _walk(self, cursor, namespace_prefix: Optional[str] = None):
        for c in cursor.get_children():
            try:
                k = c.kind
            except Exception:
                continue
            if k in (CursorKind.CLASS_DECL, CursorKind.STRUCT_DECL, CursorKind.CLASS_TEMPLATE):
                if not c.spelling:
                    continue
                fq = self._fq(c)
                logger.info('API candidate class: %s', fq)
                cls = {'fq': fq, 'methods': [], 'constructors': [], 'enums': []}
                for child in c.get_children():
                    if child.kind == CursorKind.CXX_METHOD and child.access_specifier.name == 'PUBLIC':
                        is_static = child.is_static_method()
                        is_const = False
                        try:
                            is_const = child.is_const_method()
                        except Exception:
                            is_const = False
                        params = [(p.spelling or f'arg{i}', p.type) for i, p in enumerate(child.get_arguments())]
                        cls['methods'].append(
                            {'name': child.spelling, 'ret': child.result_type, 'params': params, 'is_static': is_static,
                             'is_const': is_const})
                    elif child.kind == CursorKind.CONSTRUCTOR and child.access_specifier.name == 'PUBLIC':
                        params = [(p.spelling or f'arg{i}', p.type) for i, p in enumerate(child.get_arguments())]
                        cls['constructors'].append({'params': params})
                    elif child.kind == CursorKind.ENUM_DECL:
                        vals = []
                        for gc in child.get_children():
                            if gc.kind == CursorKind.ENUM_CONSTANT_DECL:
                                vals.append((gc.spelling, gc.enum_value))
                        cls['enums'].append({'name': child.spelling, 'values': vals})
                flat = collapse_name(fq)
                self.classes[flat] = cls
            else:
                self._walk(c, namespace_prefix)

    def _fq(self, cursor):
        parts = []
        cur = cursor
        while cur and cur.kind != CursorKind.TRANSLATION_UNIT:
            if cur.spelling:
                parts.append(cur.spelling)
            cur = cur.semantic_parent
        parts.reverse()
        return '::'.join(parts)


# ---------------- Emitter ----------------
class GDEmitter:
    def __init__(self, indexer: MultiPassIndexer, prefix: str = 'occ', godot_base: str = 'Object'):
        self.indexer = indexer
        self.prefix = prefix
        self.godot_base = godot_base

    def emit_all(self, api: APIScanner, out_dir: str, impl_includes: Optional[List[str]] = None):
        os.makedirs(out_dir, exist_ok=True)
        # api header
        with open(os.path.join(out_dir, f"{self.prefix}_api.h"), 'w', encoding='utf-8') as fh:
            fh.write(self._api_header())
        # register
        with open(os.path.join(out_dir, f"{self.prefix}_register.cpp"), 'w', encoding='utf-8') as fh:
            fh.write(self._register(api))
        # wrappers
        for flat, cls in api.classes.items():
            h, cpp = self._emit_wrapper(flat, cls, impl_includes)
            with open(os.path.join(out_dir, f"{self.prefix}_{flat}.hpp"), 'w', encoding='utf-8') as fh:
                fh.write(h)
            with open(os.path.join(out_dir, f"{self.prefix}_{flat}.cpp"), 'w', encoding='utf-8') as fh:
                fh.write(cpp)
            logger.info('Emitted wrapper for %s', cls['fq'])

    def _api_header(self) -> str:
        return textwrap.dedent(f"""
// {self.prefix}_api.h - generated helper for Godot GDExtension (Godot 4.4)
#pragma once

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
using namespace godot;

// Basic helpers: convert std::string <-> String
static String _string_to_godot(const std::string &s) {{ return String::utf8(s.c_str()); }}
static std::string _string_from_godot(const String &s) {{ return std::string(s.utf8().get_data()); }}

// TODO: add PackedArray <-> std::vector helpers here as you enable vector mappings

""")

    def _register(self, api: APIScanner) -> str:
        lines = [f'#include "{self.prefix}_api.h"']
        lines.append('// Generated registration')
        for flat, cls in api.classes.items():
            lines.append(f'#include "{self.prefix}_{flat}.hpp"')
        lines.append(textwrap.dedent('''
void initialize_gdextension_types([[maybe_unused]] ModuleInitializationLevel p_level) {
    if (p_level < MODULE_INITIALIZATION_LEVEL_SCENE) return; // Only initialize at the SCENE level or higher.
'''))
        for flat in api.classes.keys():
            cls_name = f"{self.prefix}_{flat}"
            lines.append(f'    GDREGISTER_CLASS({cls_name});')
        lines.append(textwrap.dedent('''

}

void uninitialize_gdextension_types([[maybe_unused]] ModuleInitializationLevel p_level) {
}

extern "C" {
// Initialization
GDExtensionBool GDE_EXPORT opencascade_gd_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                                       GDExtensionClassLibraryPtr p_library,
                                                       GDExtensionInitialization *r_initialization) {
    GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
    init_obj.register_initializer(initialize_gdextension_types);
    init_obj.register_terminator(uninitialize_gdextension_types);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}
'''))
        return '\n'.join(lines)

    def _map_clang_type(self, t: 'cindex.Type') -> Dict[str, Any]:
        spelling = t.spelling
        cls = self.indexer.classify_spelling(spelling)
        # if override provided by mapping, return it
        if cls.get('kind') == 'override':
            return {'kind': 'override', 'mapping': cls.get('mapping')}
        return cls

    def _cpp_type_for_map(self, mapped: Dict[str, Any]) -> str:
        kind = mapped.get('kind')
        if kind == 'primitive':
            return mapped.get('target', 'int')
        if kind == 'enum':
            return 'int'
        if kind == 'record':
            # represent as void* at wrapper ABI boundary; internal forwarding will cast
            return 'void *'
        if kind == 'vector':
            return 'Variant'
        if kind == 'override':
            return mapped.get('mapping', {}).get('cpp', 'Variant')
        return 'Variant'

    def _emit_wrapper(self, flat: str, cls: Dict, impl_includes: Optional[List[str]]) -> Tuple[str, str]:
        fq = cls['fq']
        wrapper = f"{self.prefix}_{flat}"
        # header
        h = [f'#pragma once', f'#include "{self.prefix}_api.h"']
        if impl_includes:
            for inc in impl_includes:
                h.append(f'#include "{inc}"')
        h.append('')
        h.append(f'class {wrapper} : public {self.godot_base} ' + '{')
        h.append(f'    GDCLASS({wrapper}, {self.godot_base})')
        h.append('public:')
        h.append(f'    {wrapper}();')
        h.append(f'    virtual ~{wrapper}();')
        h.append('')
        # native pointer holder
        h.append('    // native pointer to underlying C++ object (opaque).')
        h.append('    void *native = nullptr;')
        h.append('    bool owned = false;')
        h.append('')
        # methods declarations
        for m in cls['methods']:
            mapped = self._map_clang_type(m['ret'])
            if mapped.get('kind') in ('primitive', 'enum', 'record', 'override', 'vector'):
                cpp_ret = self._cpp_type_for_map(mapped)
            else:
                # unsupported -> return void and be explicit
                cpp_ret = 'void'
            param_decls = []
            for i, (pname, ptype) in enumerate(m['params']):
                pm = self._map_clang_type(ptype)
                if pm.get('kind') in ('primitive', 'record', 'enum', 'override', 'vector'):
                    cpp_param = self._cpp_type_for_map(pm)
                    pname_safe = pname or f'arg{i}'
                    param_decls.append(f'{cpp_param} {pname_safe}')
                else:
                    # make it generic Object* to ensure we can compile
                    pname_safe = pname or f'arg{i}'
                    param_decls.append(f'Object * {pname_safe} /* unsupported type: {ptype.spelling} */')
            # name disambiguation for overloads
            arg_types = [pt.spelling for (_, pt) in m['params']]
            suffix = '_' + sig_hash(arg_types) if arg_types else ''
            method_name = m['name'] + suffix
            h.append(f'    {cpp_ret} {method_name}({", ".join(param_decls)});')
        h.append('')
        # constructor overloads as "construct" methods
        for ctor in cls.get('constructors', []):
            param_decls = []
            for i, (pname, ptype) in enumerate(ctor['params']):
                pm = self._map_clang_type(ptype)
                pname_safe = pname or f'arg{i}'
                if pm.get('kind') in ('primitive', 'enum', 'record', 'override', 'vector'):
                    cpp_param = self._cpp_type_for_map(pm)
                    param_decls.append(f'{cpp_param} {pname_safe}')
                else:
                    param_decls.append(f'Object * {pname_safe} /* unsupported type: {ptype.spelling} */')
            arg_types = [pt.spelling for (_, pt) in ctor['params']]
            suffix = '_' + sig_hash(arg_types) if arg_types else ''
            h.append(f'    void construct{suffix}({", ".join(param_decls)});')
        h.append('')
        # explicit destroy method
        h.append('    void destroy();')
        h.append('')
        h.append('    static void _bind_methods();')
        h.append('};')

        header_text = '\n'.join(h)

        # cpp
        cpp = [f'#include "{wrapper}.hpp"', '', 'using namespace godot;', '']
        # bind methods
        cpp.append(f'void {wrapper}::_bind_methods() ' + '{')
        for m in cls['methods']:
            arg_names = [p[0] or f'arg{i}' for i, p in enumerate(m['params'])]
            dargs = ', '.join('"' + n + '"' for n in arg_names) if arg_names else ''
            arg_types = [pt.spelling for (_, pt) in m['params']]
            suffix = '_' + sig_hash(arg_types) if arg_types else ''
            method_name = m['name'] + suffix
            if dargs:
                cpp.append(f'    ClassDB::bind_method(D_METHOD("{m["name"]}", {dargs}), &{wrapper}::{method_name});')
            else:
                cpp.append(f'    ClassDB::bind_method(D_METHOD("{m["name"]}"), &{wrapper}::{method_name});')
        # bind constructor overloads as "construct"
        for ctor in cls.get('constructors', []):
            arg_names = [p[0] or f'arg{i}' for i, p in enumerate(ctor['params'])]
            dargs = ', '.join('"' + n + '"' for n in arg_names) if arg_names else ''
            arg_types = [pt.spelling for (_, pt) in ctor['params']]
            suffix = '_' + sig_hash(arg_types) if arg_types else ''
            method_name = 'construct' + suffix
            if dargs:
                cpp.append(f'    ClassDB::bind_method(D_METHOD("construct", {dargs}), &{wrapper}::{method_name});')
            else:
                cpp.append(f'    ClassDB::bind_method(D_METHOD("construct"), &{wrapper}::{method_name});')
        # bind explicit destroy
        cpp.append(f'    ClassDB::bind_method(D_METHOD("destroy"), &{wrapper}::destroy);')
        cpp.append('}')
        cpp.append('')
        # ctor/dtor
        cpp.append(f'{wrapper}::{wrapper}() {{}}')
        cpp.append(f'{wrapper}::~{wrapper}() {{')
        cpp.append('    if (this->native && this->owned) {')
        cpp.append(f'        delete reinterpret_cast<{cls["fq"]}*>(this->native);')
        cpp.append('        this->native = nullptr;')
        cpp.append('        this->owned = false;')
        cpp.append('    }')
        cpp.append('}')
        cpp.append('')

        # method bodies
        for m in cls['methods']:
            mapped = self._map_clang_type(m['ret'])
            arg_types = [pt.spelling for (_, pt) in m['params']]
            suffix = '_' + sig_hash(arg_types) if arg_types else ''
            method_name = m['name'] + suffix
            # create signature
            param_sig = []
            conv_lines = []
            call_args = []
            for i, (pname, ptype) in enumerate(m['params']):
                pm = self._map_clang_type(ptype)
                pname_safe = pname or f'arg{i}'
                if pm.get('kind') == 'primitive':
                    cpp_type = pm['target']
                    param_sig.append(f'{cpp_type} {pname_safe}')
                    call_args.append(pname_safe)
                elif pm.get('kind') == 'enum':
                    param_sig.append(f'int {pname_safe}')
                    call_args.append(pname_safe)
                elif pm.get('kind') == 'record':
                    # assume record is available; pass pointer
                    param_sig.append(f'void * {pname_safe}')
                    call_args.append(f'reinterpret_cast<{pm["name"]}*>({pname_safe})')
                elif pm.get('kind') == 'vector':
                    # accept PackedArray in Godot; conversion TODO
                    param_sig.append(f'Variant {pname_safe} /* PackedArray expected */')
                    conv_lines.append(
                        f'    // TODO: convert PackedArray to std::vector<{pm["inner"]}> for {pname_safe}')
                    call_args.append('/*converted_' + pname_safe + '*/')
                elif pm.get('kind') == 'override':
                    # user supplied mapping
                    param_sig.append(f'{pm.get("mapping", {}).get("cpp", "Variant")} {pname_safe}')
                    call_args.append(pname_safe)
                else:
                    param_sig.append(f'Object * {pname_safe} /* opaque */')
                    call_args.append(pname_safe)
            # return type
            if mapped.get('kind') == 'primitive':
                ret_cpp = mapped['target']
            elif mapped.get('kind') == 'enum':
                ret_cpp = 'int'
            elif mapped.get('kind') == 'record':
                ret_cpp = 'void *'
            elif mapped.get('kind') == 'vector':
                ret_cpp = 'Variant'
            else:
                ret_cpp = 'void'

            sig = f'{ret_cpp} {wrapper}::{method_name}({", ".join(param_sig)})'
            body = [sig + ' {']
            body.extend(conv_lines)
            # forward call if possible
            ti = None
            if cls.get('fq') in self.indexer.types:
                ti = self.indexer.types.get(cls['fq'])
            if ti and ti.kind == 'record' and ti.is_definition:
                # call underlying method if native pointer present
                call_target = f'reinterpret_cast<{cls["fq"]}*>(this->native)'
                body.append(f'    if (!this->native) {{ ERR_PRINT("Native pointer is null for {cls["fq"]}");')
                if ret_cpp != 'void':
                    default = '0' if 'int' in ret_cpp or 'long' in ret_cpp else 'false' if 'bool' in ret_cpp else 'Variant()'
                    body.append(f'        return {default};')
                else:
                    body.append('        return;')
                body.append('    }')
                body.append(f'    {call_target}->{m["name"]}({", ".join(call_args)});')
                if ret_cpp != 'void':
                    body.append(f'    return {default};')
            else:
                body.append(f'    ERR_PRINT("Automatic forwarding not available for {cls["fq"]}::{m["name"]}");')
                if ret_cpp != 'void':
                    default = '0' if 'int' in ret_cpp or 'long' in ret_cpp else 'false' if 'bool' in ret_cpp else 'Variant()'
                    body.append(f'    return {default};')
            body.append('}')
            cpp.append('\n'.join(body))
            cpp.append('')
        # constructor bodies (construct*)
        for ctor in cls.get('constructors', []):
            arg_types = [pt.spelling for (_, pt) in ctor['params']]
            suffix = '_' + sig_hash(arg_types) if arg_types else ''
            method_name = 'construct' + suffix
            # signature and conversions
            param_sig = []
            conv_lines = []
            call_args = []
            for i, (pname, ptype) in enumerate(ctor['params']):
                pm = self._map_clang_type(ptype)
                pname_safe = pname or f'arg{i}'
                if pm.get('kind') == 'primitive':
                    cpp_type = pm['target']
                    param_sig.append(f'{cpp_type} {pname_safe}')
                    call_args.append(pname_safe)
                elif pm.get('kind') == 'enum':
                    param_sig.append(f'int {pname_safe}')
                    call_args.append(pname_safe)
                elif pm.get('kind') == 'record':
                    param_sig.append(f'void * {pname_safe}')
                    call_args.append(f'reinterpret_cast<{pm["name"]}*>({pname_safe})')
                elif pm.get('kind') == 'vector':
                    param_sig.append(f'Variant {pname_safe} /* PackedArray expected */')
                    conv_lines.append(
                        f'    // TODO: convert PackedArray to std::vector<{pm["inner"]}> for {pname_safe}')
                    call_args.append('/*converted_' + pname_safe + '*/')
                elif pm.get('kind') == 'override':
                    param_sig.append(f'{pm.get("mapping", {}).get("cpp", "Variant")} {pname_safe}')
                    call_args.append(pname_safe)
                else:
                    param_sig.append(f'Object * {pname_safe} /* opaque */')
                    call_args.append(pname_safe)
        sig = f'void {wrapper}::{method_name}({", ".join(param_sig)})'
        body = [sig + ' {']
        body.extend(conv_lines)
        # destroy previous if owned
        body.append('    if (this->native && this->owned) {')
        body.append(f'        delete reinterpret_cast<{cls["fq"]}*>(this->native);')
        body.append('        this->native = nullptr;')
        body.append('        this->owned = false;')
        body.append('    }')
        # allocate new native instance
        ti = None
        if cls.get('fq') in self.indexer.types:
            ti = self.indexer.types.get(cls['fq'])
        if ti and ti.kind == 'record':
            body.append(f'    this->native = new {cls["fq"]}({", ".join(call_args)});')
            body.append('    this->owned = true;')
        else:
            body.append(f'    ERR_PRINT("Automatic construction not available for {cls["fq"]}");')
        body.append('}')
        cpp.append('\n'.join(body))
        cpp.append('')

        # explicit destroy()
        cpp.append(f'void {wrapper}::destroy() {{')
        cpp.append('    if (this->native && this->owned) {')
        cpp.append(f'        delete reinterpret_cast<{cls["fq"]}*>(this->native);')
        cpp.append('    }')
        cpp.append('    this->native = nullptr;')
        cpp.append('    this->owned = false;')
        cpp.append('}')
        cpp.append('')

        return header_text, '\n'.join(cpp)


# ---------------- CLI ----------------

def main(argv=None):
    ap = argparse.ArgumentParser(prog='generate_gdextension_occ_multipass.py')
    ap.add_argument('--headers', '-H', nargs='+', required=True,
                    help='Glob(s) for C++ header files to parse (supports glob patterns)')
    ap.add_argument('-I', dest='includes', action='append', default=[], help='include search paths')
    ap.add_argument('--out-dir', '-o', default='out', help='output directory')
    ap.add_argument('--prefix', default='occ', help='prefix for generated files and classes')
    ap.add_argument('--map', default=None, help='optional JSON mapping overrides (type -> mapping)')
    ap.add_argument('--libclang-path', default=None, help='optional libclang path')
    ap.add_argument('--include-originals', action='store_true', help='include original headers in generated impls')
    ap.add_argument('--clang-args', nargs='*', default=[], help='extra clang args')
    args = ap.parse_args(argv)

    if args.libclang_path:
        try:
            cindex.Config.set_library_path(args.libclang_path)
        except Exception as e:
            logger.warning('Failed to set libclang path: %s', e)

    index = cindex.Index.create()

    # load mapping overrides
    mapping_overrides = {}
    if args.map:
        try:
            with open(args.map, 'r', encoding='utf-8') as fh:
                mapping_overrides = json.load(fh)
            logger.info('Loaded mapping overrides from %s', args.map)
        except Exception as e:
            logger.error('Failed to load mapping file: %s', e)

    clang_args = []
    for inc in (args.includes or []):
        clang_args.append('-I' + inc)
    clang_args.extend(args.clang_args or [])

    indexer = MultiPassIndexer(index, clang_args, mapping_overrides)
    indexer.discover(args.headers)

    scanner = APIScanner(indexer, clang_args)
    scanner.scan(args.headers)

    emitter = GDEmitter(indexer, prefix=args.prefix, godot_base='Object')
    impl_includes = args.headers if args.include_originals else None
    emitter.emit_all(scanner, args.out_dir, impl_includes)

    logger.info('Generation complete. Files written to %s', args.out_dir)


if __name__ == '__main__':
    main()
