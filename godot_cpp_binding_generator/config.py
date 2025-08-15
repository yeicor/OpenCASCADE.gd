"""
Configuration module for the Godot C++ binding generator.

Centralizes CLI options, defaults, and helpers for constructing a complete
configuration for the generator. The goal is to keep the rest of the codebase
agnostic to argument parsing, enabling both CLI and programmatic use.

Typical usage (CLI):
    from godot_cpp_binding_generator.config import parse_argv
    cfg = parse_argv()
    # use cfg.headers, cfg.to_clang_args(), cfg.mapping_overrides, ...

Typical usage (programmatic):
    cfg = GeneratorConfig(
        headers=["/path/to/lib/**/*.h"],
        includes=["/path/to/lib/include"],
        out_dir="out",
        prefix="mylib",
    ).load_mapping()
"""

from __future__ import annotations

import argparse
import json
import logging
import os
from dataclasses import dataclass, field, replace
from typing import Any, Dict, List, Optional, Sequence

__all__ = [
    "GeneratorConfig",
    "create_arg_parser",
    "add_cli_arguments",
    "parse_argv",
]


@dataclass(frozen=True)
class GeneratorConfig:
    """
    Immutable configuration for a generator run.

    Check the CLI for documentation on each field.
    """

    headers: List[str]
    includes: List[str] = field(default_factory=list)
    include_rel_base: Optional[str] = None
    out_dir: str = "out"
    prefix: str = "ocgd"

    mapping_file: Optional[str] = None
    mapping_overrides: Dict[str, Any] = field(default_factory=dict)

    libclang_path: Optional[str] = None
    include_originals: bool = False

    clang_args_extra: List[str] = field(default_factory=list)
    cxx_std: str = "c++17"
    godot_base: str = "Object"

    log_level: str = "INFO"

    # ---------- Computed helpers ----------

    def to_clang_args(self) -> List[str]:
        """
        Construct the standard clang argument list for parsing.
        Does not include the input file path — callers pass it to libclang.
        """
        args: List[str] = ["-x", "c++", f"-std={self.cxx_std}"]
        # Normalize include paths and ensure uniqueness while preserving order.
        seen: set[str] = set()
        # Base directory for resolving relative includes should come first.
        if self.include_rel_base:
            base_norm = os.path.normpath(self.include_rel_base)
            if base_norm not in seen:
                seen.add(base_norm)
                args.append("-I" + base_norm)
        for inc in self.includes:
            norm = os.path.normpath(inc)
            if norm not in seen:
                seen.add(norm)
                args.append("-I" + norm)
        # Extra user-provided args last, to allow overriding defaults if needed.
        args.extend(self.clang_args_extra)
        return args

    # ---------- Mapping helpers ----------

    def load_mapping(self) -> "GeneratorConfig":
        """
        Load mapping overrides from `mapping_file` (if provided), returning a new config.
        If the mapping file cannot be read/parsed, returns config unchanged.
        """
        if not self.mapping_file:
            return self
        path = os.path.expanduser(self.mapping_file)
        try:
            with open(path, "r", encoding="utf-8") as fh:
                data = json.load(fh)
            if not isinstance(data, dict):
                raise ValueError("mapping file must contain a JSON object (type -> mapping)")
            return replace(self, mapping_overrides=data)
        except Exception as e:
            logging.getLogger("gdocc.config").warning("Failed to load mapping file %s: %s", path, e)
            return self

    # ---------- Environment setup ----------

    def apply_libclang_path(self) -> "GeneratorConfig":
        """
        Attempt to set the libclang path if provided. Returns self for chaining.
        """
        if not self.libclang_path:
            return self
        try:
            # Import locally to avoid hard dependency for users who only build config.
            from clang import cindex  # type: ignore
            cindex.Config.set_library_path(self.libclang_path)
        except Exception as e:
            logging.getLogger("gdocc.config").warning(
                "Failed to set libclang path (%s): %s", self.libclang_path, e
            )
        return self

    # ---------- Validation ----------

    def validate(self) -> None:
        """
        Validate essential fields; raise ValueError on critical issues.
        """
        if not self.headers:
            raise ValueError("No headers specified. Provide at least one header glob via --headers.")
        if not isinstance(self.includes, list):
            raise ValueError("--include/-I must be provided zero or more times and result in a list.")
        if not self.prefix or not isinstance(self.prefix, str):
            raise ValueError("--prefix must be a non-empty string.")
        # out_dir may not exist yet; no strict validation here.

    # ---------- Logging ----------

    def configure_logging(self) -> None:
        """
        Initialize the root logger format and level if not already configured.
        """
        level = getattr(logging, (self.log_level or "INFO").upper(), logging.INFO)
        logging.basicConfig(level=level, format="[%(levelname)s] %(message)s")


# ---------- CLI helpers ----------


def add_cli_arguments(ap: argparse.ArgumentParser) -> None:
    """
    Add generator CLI arguments to an existing ArgumentParser.
    """
    ap.add_argument(
        "--headers",
        "-H",
        nargs="+",
        required=True,
        help="Glob(s) for C++ header files to parse (supports glob patterns)",
    )
    ap.add_argument(
        "-I",
        dest="includes",
        action="append",
        default=[],
        help="Include search path (can be specified multiple times)",
    )
    ap.add_argument(
        "--include-rel-base",
        dest="include_rel_base",
        default=None,
        help="Base directory for resolving quoted/angle includes in headers (added first to -I search paths)",
    )
    ap.add_argument("--out-dir", "-o", default="out", help="Output directory")
    ap.add_argument("--prefix", default="ocgd", help="Prefix for generated files and classes")
    ap.add_argument(
        "--map", dest="mapping_file", default=None, help="Optional JSON mapping overrides (type -> mapping)"
    )
    ap.add_argument(
        "--libclang-path",
        default=None,
        help="Optional libclang path (directory containing the libclang shared library)",
    )
    ap.add_argument(
        "--include-originals",
        action="store_true",
        help="Include original headers in generated implementation files",
    )
    ap.add_argument(
        "--clang-args",
        nargs="*",
        default=[],
        help="Extra arguments passed directly to clang (e.g., -DDEF=1 -isystem /path)",
    )
    ap.add_argument(
        "--std",
        dest="cxx_std",
        default="c++17",
        help="C++ standard to use (e.g., c++17, c++20)",
    )
    ap.add_argument(
        "--godot-base",
        default="Object",
        help='Base Godot class for wrappers (e.g., "Object", "RefCounted")',
    )
    ap.add_argument(
        "--log-level",
        default="INFO",
        choices=["CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG"],
        help="Logging verbosity",
    )


def create_arg_parser(prog: Optional[str] = None, description: Optional[str] = None) -> argparse.ArgumentParser:
    """
    Create a standalone ArgumentParser for the generator CLI.
    """
    ap = argparse.ArgumentParser(prog=prog, description=description)
    add_cli_arguments(ap)
    return ap


def _namespace_to_config(ns: argparse.Namespace) -> GeneratorConfig:
    """
    Translate an argparse Namespace into a GeneratorConfig.
    """
    includes = list(ns.includes or [])
    # Ensure the relative-include base is the first include path (highest priority).
    include_rel_base = getattr(ns, "include_rel_base", None)
    if include_rel_base:
        includes.insert(0, include_rel_base)
    clang_args_extra = list(ns.clang_args or [])
    cfg = GeneratorConfig(
        headers=list(ns.headers or []),
        includes=includes,
        include_rel_base=include_rel_base,
        out_dir=ns.out_dir,
        prefix=ns.prefix,
        mapping_file=ns.mapping_file,
        libclang_path=ns.libclang_path,
        include_originals=bool(ns.include_originals),
        clang_args_extra=clang_args_extra,
        cxx_std=ns.cxx_std,
        godot_base=ns.godot_base,
        log_level=ns.log_level,
    )
    return cfg


def parse_argv(argv: Optional[Sequence[str]] = None) -> GeneratorConfig:
    """
    Parse CLI arguments into a fully-prepared GeneratorConfig:
    - Applies libclang path if provided.
    - Loads mapping overrides from file (if provided).
    - Configures logging to the desired level.
    - Validates core options.
    """
    ap = create_arg_parser(
        description="Multi-pass, automated GDExtension binding generator for C++ libraries."
    )
    ns = ap.parse_args(argv)
    cfg = _namespace_to_config(ns)
    cfg.configure_logging()
    cfg.apply_libclang_path()
    cfg = cfg.load_mapping()
    cfg.validate()
    return cfg
