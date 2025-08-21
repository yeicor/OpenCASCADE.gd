#ifndef _ocgd_GeomAbs_JoinType_h
#define _ocgd_GeomAbs_JoinType_h

/**
 * ocgd_GeomAbs_JoinType.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE GeomAbs_JoinType as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/GeomAbs_JoinType.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/GeomAbs_JoinType.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_GeomAbs_JoinType
 *
 * Wrapper class to expose the OpenCASCADE GeomAbs_JoinType enumeration to Godot.
 *
 * Characterizes the type of a join, built by an algorithm for constructing 
 * parallel curves, between two consecutive arcs of a contour parallel to a 
 * given contour.
 *
 * In GDScript, use constants like:
 *   ocgd_GeomAbs_JoinType.Arc
 *
 * Or treat them as an enum within this class.
 */
class ocgd_GeomAbs_JoinType : public Object {
    GDCLASS(ocgd_GeomAbs_JoinType, Object);

public:
    //! Definition of join types (mirrors OpenCASCADE GeomAbs_JoinType)
    enum Type {
        Arc = ::GeomAbs_Arc,
        Tangent = ::GeomAbs_Tangent,
        Intersection = ::GeomAbs_Intersection
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_GeomAbs_JoinType.*
        BIND_ENUM_CONSTANT(Arc);
        BIND_ENUM_CONSTANT(Tangent);
        BIND_ENUM_CONSTANT(Intersection);
    }
};

VARIANT_ENUM_CAST(ocgd_GeomAbs_JoinType::Type);

#endif // _ocgd_GeomAbs_JoinType_h