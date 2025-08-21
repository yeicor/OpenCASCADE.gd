/**
 * ocgd_Quantity_NameOfColor.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE Quantity_NameOfColor as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/Quantity_NameOfColor.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#pragma once

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/Quantity_NameOfColor.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_Quantity_NameOfColor
 *
 * Wrapper class to expose the OpenCASCADE Quantity_NameOfColor enumeration to Godot.
 *
 * Quantity_NameOfColor defines a set of commonly used named colors.
 *
 * In GDScript, use constants like:
 *   ocgd_Quantity_NameOfColor.Quantity_NOC_RED
 *
 * Or treat them as an enum within this class.
 */
class ocgd_Quantity_NameOfColor : public Object {
    GDCLASS(ocgd_Quantity_NameOfColor, Object);

public:
    //! Definition of named colors (mirrors OpenCASCADE Quantity_NameOfColor; partial selection of most common colors)
    enum Type {
        Quantity_NOC_BLACK   = ::Quantity_NOC_BLACK,   //!< Black color (0, 0, 0)
        Quantity_NOC_WHITE   = ::Quantity_NOC_WHITE,   //!< White color (255, 255, 255)
        Quantity_NOC_RED     = ::Quantity_NOC_RED,     //!< Red color
        Quantity_NOC_GREEN   = ::Quantity_NOC_GREEN,   //!< Green color
        Quantity_NOC_BLUE    = ::Quantity_NOC_BLUE,    //!< Blue color
        Quantity_NOC_YELLOW  = ::Quantity_NOC_YELLOW,  //!< Yellow color
        Quantity_NOC_CYAN    = ::Quantity_NOC_CYAN,    //!< Cyan color
        Quantity_NOC_MAGENTA = ::Quantity_NOC_MAGENTA  //!< Magenta color
        // Note: Quantity_NameOfColor defines many more named colors; add more bindings as needed.
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_Quantity_NameOfColor.Quantity_NOC_*
        BIND_ENUM_CONSTANT(Quantity_NOC_BLACK);
        BIND_ENUM_CONSTANT(Quantity_NOC_WHITE);
        BIND_ENUM_CONSTANT(Quantity_NOC_RED);
        BIND_ENUM_CONSTANT(Quantity_NOC_GREEN);
        BIND_ENUM_CONSTANT(Quantity_NOC_BLUE);
        BIND_ENUM_CONSTANT(Quantity_NOC_YELLOW);
        BIND_ENUM_CONSTANT(Quantity_NOC_CYAN);
        BIND_ENUM_CONSTANT(Quantity_NOC_MAGENTA);
    }
};

VARIANT_ENUM_CAST(ocgd_Quantity_NameOfColor::Type);