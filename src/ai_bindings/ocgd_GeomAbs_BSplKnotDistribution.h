#ifndef _ocgd_GeomAbs_BSplKnotDistribution_h
#define _ocgd_GeomAbs_BSplKnotDistribution_h

/**
 * ocgd_GeomAbs_BSplKnotDistribution.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE GeomAbs_BSplKnotDistribution as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/GeomAbs_BSplKnotDistribution.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/GeomAbs_BSplKnotDistribution.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_GeomAbs_BSplKnotDistribution
 *
 * Wrapper class to expose the OpenCASCADE GeomAbs_BSplKnotDistribution enumeration to Godot.
 *
 * This enumeration is used in the classes BSplineCurve and BSplineSurface 
 * to describe the repartition of set of knots.
 *
 * In GDScript, use constants like:
 *   ocgd_GeomAbs_BSplKnotDistribution.NonUniform
 *
 * Or treat them as an enum within this class.
 */
class ocgd_GeomAbs_BSplKnotDistribution : public Object {
    GDCLASS(ocgd_GeomAbs_BSplKnotDistribution, Object);

public:
    //! Definition of BSpline knot distribution types (mirrors OpenCASCADE GeomAbs_BSplKnotDistribution)
    enum Type {
        NonUniform = ::GeomAbs_NonUniform,
        Uniform = ::GeomAbs_Uniform,
        QuasiUniform = ::GeomAbs_QuasiUniform,
        PiecewiseBezier = ::GeomAbs_PiecewiseBezier
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_GeomAbs_BSplKnotDistribution.*
        BIND_ENUM_CONSTANT(NonUniform);
        BIND_ENUM_CONSTANT(Uniform);
        BIND_ENUM_CONSTANT(QuasiUniform);
        BIND_ENUM_CONSTANT(PiecewiseBezier);
    }
};

VARIANT_ENUM_CAST(ocgd_GeomAbs_BSplKnotDistribution::Type);

#endif // _ocgd_GeomAbs_BSplKnotDistribution_h