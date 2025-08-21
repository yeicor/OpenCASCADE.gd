/**
 * ocgd_GeomAbs_Shape.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE GeomAbs_Shape as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/GeomAbs_Shape.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#pragma once

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/GeomAbs_Shape.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_GeomAbs_Shape
 *
 * Wrapper class to expose the OpenCASCADE GeomAbs_Shape enumeration to Godot.
 *
 * Provides information about the continuity of a curve:
 * - C0: only geometric continuity.
 * - G1: for each point on the curve, the tangent vectors "on the right"
 *   and "on the left" are collinear with the same orientation.
 * - C1: continuity of the first derivative. The "C1" curve is also "G1"
 *   but, in addition, the tangent vectors "on the right" and "on the left" are equal.
 * - G2: for each point on the curve, the normalized normal vectors "on the right"
 *   and "on the left" are equal.
 * - C2: continuity of the second derivative.
 * - C3: continuity of the third derivative.
 * - CN: continuity of the N-th derivative, whatever is the value given for N
 *   (infinite order of continuity).
 *
 * Also provides information about the continuity of a surface:
 * - C0: only geometric continuity.
 * - C1: continuity of the first derivatives; any isoparametric (in U or V)
 *   of a surface "C1" is also "C1".
 * - G2: for BSpline curves only; "on the right" and "on the left" of a knot
 *   the computation of the "main curvature radii" and the "main directions"
 *   (when they exist) gives the same result.
 * - C2: continuity of the second derivative.
 * - C3: continuity of the third derivative.
 * - CN: continuity of any N-th derivative, whatever is the value given for N
 *   (infinite order of continuity).
 *
 * We may also say that a surface is "Ci" in u, and "Cj" in v to indicate the
 * continuity of its derivatives up to the order i in the u parametric direction,
 * and j in the v parametric direction.
 *
 * In GDScript, use constants like:
 *   ocgd_GeomAbs_Shape.GeomAbs_C1
 *
 * Or treat them as an enum within this class.
 */
class ocgd_GeomAbs_Shape : public Object {
    GDCLASS(ocgd_GeomAbs_Shape, Object);

public:
    //! Definition of continuity types (mirrors OpenCASCADE GeomAbs_Shape)
    enum Type {
        GeomAbs_C0 = ::GeomAbs_C0,
        GeomAbs_G1 = ::GeomAbs_G1,
        GeomAbs_C1 = ::GeomAbs_C1,
        GeomAbs_G2 = ::GeomAbs_G2,
        GeomAbs_C2 = ::GeomAbs_C2,
        GeomAbs_C3 = ::GeomAbs_C3,
        GeomAbs_CN = ::GeomAbs_CN
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_GeomAbs_Shape.GeomAbs_*
        BIND_ENUM_CONSTANT(GeomAbs_C0);
        BIND_ENUM_CONSTANT(GeomAbs_G1);
        BIND_ENUM_CONSTANT(GeomAbs_C1);
        BIND_ENUM_CONSTANT(GeomAbs_G2);
        BIND_ENUM_CONSTANT(GeomAbs_C2);
        BIND_ENUM_CONSTANT(GeomAbs_C3);
        BIND_ENUM_CONSTANT(GeomAbs_CN);
    }
};

VARIANT_ENUM_CAST(ocgd_GeomAbs_Shape::Type);
