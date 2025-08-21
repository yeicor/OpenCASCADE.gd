#ifndef _ocgd_GeomAbs_IsoType_h
#define _ocgd_GeomAbs_IsoType_h

/**
 * ocgd_GeomAbs_IsoType.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE GeomAbs_IsoType as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/GeomAbs_IsoType.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/GeomAbs_IsoType.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_GeomAbs_IsoType
 *
 * Wrapper class to expose the OpenCASCADE GeomAbs_IsoType enumeration to Godot.
 *
 * Describes if a curve is a U isoparametric or V isoparametric.
 *
 * In GDScript, use constants like:
 *   ocgd_GeomAbs_IsoType.IsoU
 *
 * Or treat them as an enum within this class.
 */
class ocgd_GeomAbs_IsoType : public Object {
    GDCLASS(ocgd_GeomAbs_IsoType, Object);

public:
    //! Definition of iso types (mirrors OpenCASCADE GeomAbs_IsoType)
    enum Type {
        IsoU = ::GeomAbs_IsoU,
        IsoV = ::GeomAbs_IsoV,
        NoneIso = ::GeomAbs_NoneIso
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_GeomAbs_IsoType.*
        BIND_ENUM_CONSTANT(IsoU);
        BIND_ENUM_CONSTANT(IsoV);
        BIND_ENUM_CONSTANT(NoneIso);
    }
};

VARIANT_ENUM_CAST(ocgd_GeomAbs_IsoType::Type);

#endif // _ocgd_GeomAbs_IsoType_h