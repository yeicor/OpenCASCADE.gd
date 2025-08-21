/**
 * ocgd_GeomAbs_CurveType.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE GeomAbs_CurveType as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header where applicable.
 *
 * Original OCCT header: <opencascade/GeomAbs_CurveType.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#pragma once

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/GeomAbs_CurveType.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_GeomAbs_CurveType
 *
 * Wrapper class to expose the OpenCASCADE GeomAbs_CurveType enumeration to Godot.
 *
 * Identifies the specific type of a curve:
 * - Line
 * - Circle
 * - Ellipse
 * - Hyperbola
 * - Parabola
 * - Bezier curve
 * - BSpline curve
 * - Offset curve
 * - Other/unknown curve type
 *
 * In GDScript, use constants like:
 *   ocgd_GeomAbs_CurveType.GeomAbs_Line
 *
 * Or treat them as an enum within this class.
 */
class ocgd_GeomAbs_CurveType : public Object {
    GDCLASS(ocgd_GeomAbs_CurveType, Object);

public:
    //! Definition of curve types (mirrors OpenCASCADE GeomAbs_CurveType)
    enum Type {
        GeomAbs_Line         = ::GeomAbs_Line,         //!< A line.
        GeomAbs_Circle       = ::GeomAbs_Circle,       //!< A circle.
        GeomAbs_Ellipse      = ::GeomAbs_Ellipse,      //!< An ellipse.
        GeomAbs_Hyperbola    = ::GeomAbs_Hyperbola,    //!< A hyperbola.
        GeomAbs_Parabola     = ::GeomAbs_Parabola,     //!< A parabola.
        GeomAbs_BezierCurve  = ::GeomAbs_BezierCurve,  //!< A Bezier curve.
        GeomAbs_BSplineCurve = ::GeomAbs_BSplineCurve, //!< A BSpline curve.
        GeomAbs_OffsetCurve  = ::GeomAbs_OffsetCurve,  //!< An offset of a curve.
        GeomAbs_OtherCurve   = ::GeomAbs_OtherCurve    //!< Other/unknown curve type.
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_GeomAbs_CurveType.GeomAbs_*
        BIND_ENUM_CONSTANT(GeomAbs_Line);
        BIND_ENUM_CONSTANT(GeomAbs_Circle);
        BIND_ENUM_CONSTANT(GeomAbs_Ellipse);
        BIND_ENUM_CONSTANT(GeomAbs_Hyperbola);
        BIND_ENUM_CONSTANT(GeomAbs_Parabola);
        BIND_ENUM_CONSTANT(GeomAbs_BezierCurve);
        BIND_ENUM_CONSTANT(GeomAbs_BSplineCurve);
        BIND_ENUM_CONSTANT(GeomAbs_OffsetCurve);
        BIND_ENUM_CONSTANT(GeomAbs_OtherCurve);
    }
};

VARIANT_ENUM_CAST(ocgd_GeomAbs_CurveType::Type);