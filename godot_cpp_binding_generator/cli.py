#!/usr/bin/env python3
"""
CLI entry point for the Godot C++ binding generator.

This module wires together:
- Configuration parsing (config.GeneratorConfig)
- Multi-pass type indexing (indexer.MultiPassIndexer)
- Public API scanning (scanner.APIScanner)
- Code emission (emitter.GDEmitter)

Usage:
    python -m godot_cpp_binding_generator.cli --headers path/**/*.h -I include_dir -o out --prefix mylib
"""

from __future__ import annotations

import glob
import logging
import os
from typing import List, Optional, Sequence

from .config import parse_argv, GeneratorConfig
from .indexer import MultiPassIndexer
from .scanner import APIScanner
from .emitter import GDEmitter

try:
    from clang import cindex
except Exception as exc:
    raise RuntimeError(
        "clang.cindex (libclang) is required. Install the Python bindings for clang and ensure libclang is available."
    ) from exc


logger = logging.getLogger("gdocc.cli")


def _expanded_includes_from_headers(headers: List[str], base: Optional[str] = None) -> List[str]:
    """
    Expand provided header globs into a unique, sorted list of header file paths.
    This is useful when the user asks to include original headers in the generated .cpp.
    """
    files = set()
    base_norm = os.path.normpath(base) if base else None
    for pattern in headers or []:
        for path in glob.glob(pattern, recursive=True):
            if os.path.isfile(path):
                inc = path
                if base_norm:
                    try:
                        rel = os.path.relpath(path, base_norm)
                        if not rel.startswith(".."):
                            inc = rel.replace(os.sep, "/")
                    except Exception:
                        inc = path
                files.add(inc)
    return sorted(files)


def main(argv: Optional[Sequence[str]] = None) -> int:
    """
    Run the generator CLI with the provided argv (or sys.argv if None).
    Returns a standard process exit code (0 on success, non-zero on failure).
    """
    try:
        cfg: GeneratorConfig = parse_argv(argv)
    except SystemExit:
        # argparse already printed help or error; propagate exit code
        raise
    except Exception as e:
        print(f"[ERROR] Failed to parse arguments: {e}")
        return 2

    logger.info("Starting binding generation with prefix=%s, out_dir=%s", cfg.prefix, cfg.out_dir)

    # Initialize libclang (path applied by parse_argv via cfg.apply_libclang_path())
    try:
        index = cindex.Index.create()
    except Exception as e:
        logger.error("Failed to create clang index: %s", e)
        return 2

    # Construct full clang args using config (includes -x/-std, include paths, and extra args)
    clang_args: List[str] = cfg.to_clang_args()

    # Pass 1: Index / discover types
    indexer = MultiPassIndexer(index, clang_args, cfg.mapping_overrides)
    indexer.discover(cfg.headers)

    # Pass 2: Extract public API
    scanner = APIScanner(indexer, clang_args)
    classes = scanner.scan(cfg.headers)
    logger.info("Discovered %d class(es) for emission.", len(classes))

    # Pass 3: Emit wrappers and registration
    emitter = GDEmitter(indexer, prefix=cfg.prefix, godot_base=cfg.godot_base)
    impl_includes = _expanded_includes_from_headers(cfg.headers, cfg.include_rel_base) if cfg.include_originals else None
    try:
        emitter.emit_all(scanner, cfg.out_dir, impl_includes)
    except Exception as e:
        logger.error("Emission failed: %s", e)
        return 1

    logger.info("Generation complete. Files written to %s", cfg.out_dir)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
