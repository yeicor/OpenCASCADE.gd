"""
CPPTypes module for the Godot C++ binding generator.

This module defines data structures used across the indexing, scanning, and
emission phases. It focuses on being small and dependency-light to keep
imports cheap and make it easy to evolve.

Exports:
- TypeInfo: Describes a discovered C++ type (typedef, record, enum, etc.)
- TypeKind: Literal type representing supported type categories
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, List, Optional, Tuple, Literal

# Set of supported categories for discovered C++ types.
TypeKind = Literal[
    "typedef",   # typedef/using alias
    "alias",     # alternate name/category for using declarations (kept for clarity)
    "record",    # struct/class/class template
    "enum",      # enum or enum class
    "template",  # template types (generic category; most are treated as opaque initially)
    "override",  # user-provided mapping override entry
    "unknown",   # fallback category when classification is not available
]


@dataclass
class TypeInfo:
    """
    Represents a type discovered during the indexing pass.

    Fields:
    - name: Fully-qualified name where relevant (e.g., 'ns::Class'), or the type spelling.
    - kind: Category of the type (see TypeKind).
    - cursor: Clang cursor for the declaration (may be None if synthesized).
    - underlying: For typedef/alias, the underlying type spelling (terminal or otherwise).
    - is_definition: True if a full definition was found (useful for records).
    - enum_values: For enums, a list of (name, value) enumerators.
    """

    name: str
    kind: TypeKind
    cursor: Any = None

    underlying: Optional[str] = None
    is_definition: bool = False
    enum_values: List[Tuple[str, int]] = field(default_factory=list)

    def is_alias(self) -> bool:
        """
        Returns True if this type is an alias-like entity (typedef/using/override).
        """
        return self.kind in ("typedef", "alias", "override")

    def add_enum_value(self, name: str, value: int) -> None:
        """
        Append an enumerator to this enum type.
        """
        self.enum_values.append((name, value))


__all__ = ["TypeInfo", "TypeKind"]
