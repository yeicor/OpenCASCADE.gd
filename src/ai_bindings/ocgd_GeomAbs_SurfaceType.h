/**
 * ocgd_GeomAbs_SurfaceType.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE GeomAbs_SurfaceType as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header where applicable.
 *
 * Original OCCT header: <opencascade/GeomAbs_SurfaceType.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#pragma once

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/GeomAbs_SurfaceType.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_GeomAbs_SurfaceType
 *
 * Wrapper class to expose the OpenCASCADE GeomAbs_SurfaceType enumeration to Godot.
 *
 * Identifies the specific type of a surface:
 * - Plane
 * - Cylinder
 * - Cone
 * - Sphere
 * - Torus
 * - Bezier surface
 * - BSpline surface
 * - Surface of revolution
 * - Surface of extrusion
 * - Offset surface
 * - Other/unknown surface type
 *
 * In GDScript, use constants like:
 *   ocgd_GeomAbs_SurfaceType.GeomAbs_Plane
 *
 * Or treat them as an enum within this class.
 */
class ocgd_GeomAbs_SurfaceType : public Object {
    GDCLASS(ocgd_GeomAbs_SurfaceType, Object);

public:
    //! Definition of surface types (mirrors OpenCASCADE GeomAbs_SurfaceType)
    enum Type {
        GeomAbs_Plane                 = ::GeomAbs_Plane,                 //!< A plane.
        GeomAbs_Cylinder              = ::GeomAbs_Cylinder,              //!< A cylinder.
        GeomAbs_Cone                  = ::GeomAbs_Cone,                  //!< A cone.
        GeomAbs_Sphere                = ::GeomAbs_Sphere,                //!< A sphere.
        GeomAbs_Torus                 = ::GeomAbs_Torus,                 //!< A torus.
        GeomAbs_BezierSurface         = ::GeomAbs_BezierSurface,         //!< A Bezier surface.
        GeomAbs_BSplineSurface        = ::GeomAbs_BSplineSurface,        //!< A BSpline surface.
        GeomAbs_SurfaceOfRevolution   = ::GeomAbs_SurfaceOfRevolution,   //!< A surface of revolution.
        GeomAbs_SurfaceOfExtrusion    = ::GeomAbs_SurfaceOfExtrusion,    //!< A surface of extrusion.
        GeomAbs_OffsetSurface         = ::GeomAbs_OffsetSurface,         //!< An offset surface.
        GeomAbs_OtherSurface          = ::GeomAbs_OtherSurface           //!< Other/unknown surface type.
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_GeomAbs_SurfaceType.GeomAbs_*
        BIND_ENUM_CONSTANT(GeomAbs_Plane);
        BIND_ENUM_CONSTANT(GeomAbs_Cylinder);
        BIND_ENUM_CONSTANT(GeomAbs_Cone);
        BIND_ENUM_CONSTANT(GeomAbs_Sphere);
        BIND_ENUM_CONSTANT(GeomAbs_Torus);
        BIND_ENUM_CONSTANT(GeomAbs_BezierSurface);
        BIND_ENUM_CONSTANT(GeomAbs_BSplineSurface);
        BIND_ENUM_CONSTANT(GeomAbs_SurfaceOfRevolution);
        BIND_ENUM_CONSTANT(GeomAbs_SurfaceOfExtrusion);
        BIND_ENUM_CONSTANT(GeomAbs_OffsetSurface);
        BIND_ENUM_CONSTANT(GeomAbs_OtherSurface);
    }
};

VARIANT_ENUM_CAST(ocgd_GeomAbs_SurfaceType::Type);