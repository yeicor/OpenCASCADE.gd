/**
 * ocgd_TopAbs_ShapeEnum.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE TopAbs_ShapeEnum as a Godot class enum.
 *
 * This mirrors the values from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/TopAbs_ShapeEnum.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#pragma once

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/TopAbs_ShapeEnum.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_TopAbs_ShapeEnum
 *
 * Wrapper class to expose the OpenCASCADE TopAbs_ShapeEnum enumeration to Godot.
 *
 * In GDScript, use constants like:
 *   ocgd_TopAbs_ShapeEnum.TopAbs_VERTEX
 *
 * Or treat them as an enum within this class.
 */
class ocgd_TopAbs_ShapeEnum : public Object {
    GDCLASS(ocgd_TopAbs_ShapeEnum, Object);

public:
    //! Definition of topological shape types (mirrors OpenCASCADE TopAbs_ShapeEnum)
    enum Type {
        TopAbs_COMPOUND = ::TopAbs_COMPOUND,
        TopAbs_COMPSOLID = ::TopAbs_COMPSOLID,
        TopAbs_SOLID = ::TopAbs_SOLID,
        TopAbs_SHELL = ::TopAbs_SHELL,
        TopAbs_FACE = ::TopAbs_FACE,
        TopAbs_WIRE = ::TopAbs_WIRE,
        TopAbs_EDGE = ::TopAbs_EDGE,
        TopAbs_VERTEX = ::TopAbs_VERTEX,
        TopAbs_SHAPE = ::TopAbs_SHAPE
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_TopAbs_ShapeEnum.TopAbs_*
        BIND_ENUM_CONSTANT(TopAbs_COMPOUND);
        BIND_ENUM_CONSTANT(TopAbs_COMPSOLID);
        BIND_ENUM_CONSTANT(TopAbs_SOLID);
        BIND_ENUM_CONSTANT(TopAbs_SHELL);
        BIND_ENUM_CONSTANT(TopAbs_FACE);
        BIND_ENUM_CONSTANT(TopAbs_WIRE);
        BIND_ENUM_CONSTANT(TopAbs_EDGE);
        BIND_ENUM_CONSTANT(TopAbs_VERTEX);
        BIND_ENUM_CONSTANT(TopAbs_SHAPE);
    }
};

VARIANT_ENUM_CAST(ocgd_TopAbs_ShapeEnum::Type);