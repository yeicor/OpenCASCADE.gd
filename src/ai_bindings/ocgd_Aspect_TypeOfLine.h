/**
 * ocgd_Aspect_TypeOfLine.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE Aspect_TypeOfLine as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/Aspect_TypeOfLine.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#pragma once

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/Aspect_TypeOfLine.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_Aspect_TypeOfLine
 *
 * Wrapper class to expose the OpenCASCADE Aspect_TypeOfLine enumeration to Godot.
 *
 * In GDScript, use constants like:
 *   ocgd_Aspect_TypeOfLine.Aspect_TOL_SOLID
 *
 * Or treat them as an enum within this class.
 */
class ocgd_Aspect_TypeOfLine : public Object {
    GDCLASS(ocgd_Aspect_TypeOfLine, Object);

public:
    //! Definition of line types (mirrors OpenCASCADE Aspect_TypeOfLine)
    enum Type {
        Aspect_TOL_EMPTY = ::Aspect_TOL_EMPTY,         //!< hidden
        Aspect_TOL_SOLID = ::Aspect_TOL_SOLID,         //!< continuous
        Aspect_TOL_DASH = ::Aspect_TOL_DASH,           //!< dashed 2.0,1.0 (MM)
        Aspect_TOL_DOT = ::Aspect_TOL_DOT,             //!< dotted 0.2,0.5 (MM)
        Aspect_TOL_DOTDASH = ::Aspect_TOL_DOTDASH,     //!< mixed  10.0,1.0,2.0,1.0 (MM)
        Aspect_TOL_USERDEFINED = ::Aspect_TOL_USERDEFINED //!< defined by Users
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_Aspect_TypeOfLine.Aspect_TOL_*
        BIND_ENUM_CONSTANT(Aspect_TOL_EMPTY);
        BIND_ENUM_CONSTANT(Aspect_TOL_SOLID);
        BIND_ENUM_CONSTANT(Aspect_TOL_DASH);
        BIND_ENUM_CONSTANT(Aspect_TOL_DOT);
        BIND_ENUM_CONSTANT(Aspect_TOL_DOTDASH);
        BIND_ENUM_CONSTANT(Aspect_TOL_USERDEFINED);
    }
};

VARIANT_ENUM_CAST(ocgd_Aspect_TypeOfLine::Type);
