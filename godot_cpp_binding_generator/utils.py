"""
Utility helpers for the Godot C++ binding generator.

This module centralizes small, widely used helpers and constants to keep the
rest of the codebase clean and focused.

Exports:
- safe_ident: sanitize strings into identifier-safe names
- collapse_name: convert fully-qualified C++ names into flat identifiers
- sig_hash: derive a short, stable hash from a sequence of type spellings
- BUILTIN_TYPE_ALIASES: common primitive/alias mappings (e.g., OCCT -> C++)
"""

from __future__ import annotations

import hashlib
from typing import Iterable

__all__ = [
    "safe_ident",
    "collapse_name",
    "sig_hash",
    "BUILTIN_TYPE_ALIASES",
    "SIG_HASH_LENGTH",
]

# Number of hex chars to keep from the SHA1 digest for signature hashing.
SIG_HASH_LENGTH = 8

# Built-in primitive/alias mapping (common for OCCT and general C++).
# These are conservative aliases intended to map common domain-specific typedefs
# to canonical primitive C++ spellings for simpler downstream handling.
BUILTIN_TYPE_ALIASES = {
    "Standard_Integer": "int",
    "Standard_Real": "double",
    "Standard_Boolean": "bool",
    "int": "int",
    "long": "long",
    "float": "float",
    "double": "double",
    "bool": "bool",
}


def safe_ident(s: str) -> str:
    """
    Convert an arbitrary string into a C/C++ identifier-safe name by replacing
    non-alphanumeric characters with underscores.

    Note: This function intentionally does not enforce language keyword checks
    or handle leading digits beyond replacement; callers may add further rules
    as needed for their specific target language/context.
    """
    return "".join(c if (c.isalnum() or c == "_") else "_" for c in s)


def collapse_name(name: str) -> str:
    """
    Collapse a fully-qualified C++ name (e.g., 'ns::Class::Method') into a
    flat identifier-safe name by replacing scope operators with underscores,
    and sanitizing the result.
    """
    return safe_ident(name.replace("::", "_"))


def sig_hash(list_of_types: Iterable[str]) -> str:
    """
    Produce a short, stable hash from a sequence of type spellings. Useful for
    disambiguating overloaded methods in generated bindings while keeping names short.

    The implementation uses SHA1 over the joined sequence (with newline separators)
    and truncates the hex digest to SIG_HASH_LENGTH characters for readability.
    """
    joined = "\n".join(list_of_types if not isinstance(list_of_types, list) else list_of_types)
    digest = hashlib.sha1(joined.encode("utf-8")).hexdigest()
    return digest[:SIG_HASH_LENGTH]
