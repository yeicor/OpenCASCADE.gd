/**
 * ocgd_TopAbs_Orientation.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE TopAbs_Orientation as a Godot class enum.
 *
 * This mirrors the values from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/TopAbs_Orientation.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#pragma once

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/TopAbs_Orientation.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_TopAbs_Orientation
 *
 * Wrapper class to expose the OpenCASCADE TopAbs_Orientation enumeration to Godot.
 *
 * In GDScript, use constants like:
 *   ocgd_TopAbs_Orientation.TopAbs_FORWARD
 *
 * Or treat them as an enum within this class.
 */
class ocgd_TopAbs_Orientation : public Object {
    GDCLASS(ocgd_TopAbs_Orientation, Object);

public:
    //! Definition of orientation types (mirrors OpenCASCADE TopAbs_Orientation)
    enum Type {
        TopAbs_FORWARD = ::TopAbs_FORWARD,
        TopAbs_REVERSED = ::TopAbs_REVERSED,
        TopAbs_INTERNAL = ::TopAbs_INTERNAL,
        TopAbs_EXTERNAL = ::TopAbs_EXTERNAL
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_TopAbs_Orientation.TopAbs_*
        BIND_ENUM_CONSTANT(TopAbs_FORWARD);
        BIND_ENUM_CONSTANT(TopAbs_REVERSED);
        BIND_ENUM_CONSTANT(TopAbs_INTERNAL);
        BIND_ENUM_CONSTANT(TopAbs_EXTERNAL);
    }
};

VARIANT_ENUM_CAST(ocgd_TopAbs_Orientation::Type);