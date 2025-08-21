/**
 * ocgd_TopAbs_State.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE TopAbs_State as a Godot class enum.
 *
 * This mirrors the values from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/TopAbs_State.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#pragma once

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/TopAbs_State.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_TopAbs_State
 *
 * Wrapper class to expose the OpenCASCADE TopAbs_State enumeration to Godot.
 *
 * In GDScript, use constants like:
 *   ocgd_TopAbs_State.TopAbs_IN
 *
 * Or treat them as an enum within this class.
 */
class ocgd_TopAbs_State : public Object {
    GDCLASS(ocgd_TopAbs_State, Object);

public:
    //! Definition of classification states (mirrors OpenCASCADE TopAbs_State)
    enum Type {
        TopAbs_IN      = ::TopAbs_IN,      //!< The point is inside the shape
        TopAbs_OUT     = ::TopAbs_OUT,     //!< The point is outside the shape
        TopAbs_ON      = ::TopAbs_ON,      //!< The point lies on the boundary
        TopAbs_UNKNOWN = ::TopAbs_UNKNOWN  //!< The state is undetermined
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_TopAbs_State.TopAbs_*
        BIND_ENUM_CONSTANT(TopAbs_IN);
        BIND_ENUM_CONSTANT(TopAbs_OUT);
        BIND_ENUM_CONSTANT(TopAbs_ON);
        BIND_ENUM_CONSTANT(TopAbs_UNKNOWN);
    }
};

VARIANT_ENUM_CAST(ocgd_TopAbs_State::Type);