/**
 * ocgd_Aspect_TypeOfMarker.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE Aspect_TypeOfMarker as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/Aspect_TypeOfMarker.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#pragma once

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/Aspect_TypeOfMarker.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_Aspect_TypeOfMarker
 *
 * Wrapper class to expose the OpenCASCADE Aspect_TypeOfMarker enumeration to Godot.
 *
 * In GDScript, use constants like:
 *   ocgd_Aspect_TypeOfMarker.Aspect_TOM_POINT
 *
 * Or treat them as an enum within this class.
 */
class ocgd_Aspect_TypeOfMarker : public Object {
    GDCLASS(ocgd_Aspect_TypeOfMarker, Object);

public:
    //! Definition of marker types (mirrors OpenCASCADE Aspect_TypeOfMarker)
    enum Type {
        Aspect_TOM_POINT       = ::Aspect_TOM_POINT,       //!< single pixel point
        Aspect_TOM_PLUS        = ::Aspect_TOM_PLUS,        //!< plus sign
        Aspect_TOM_STAR        = ::Aspect_TOM_STAR,        //!< star (plus + cross)
        Aspect_TOM_O           = ::Aspect_TOM_O,           //!< unfilled circle
        Aspect_TOM_X           = ::Aspect_TOM_X,           //!< cross
        Aspect_TOM_O_POINT     = ::Aspect_TOM_O_POINT,     //!< circle with point
        Aspect_TOM_O_PLUS      = ::Aspect_TOM_O_PLUS,      //!< circle with plus
        Aspect_TOM_O_STAR      = ::Aspect_TOM_O_STAR,      //!< circle with star
        Aspect_TOM_O_X         = ::Aspect_TOM_O_X,         //!< circle with cross
        Aspect_TOM_BALL        = ::Aspect_TOM_BALL,        //!< filled circle
        Aspect_TOM_USERDEFINED = ::Aspect_TOM_USERDEFINED  //!< defined by Users
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_Aspect_TypeOfMarker.Aspect_TOM_*
        BIND_ENUM_CONSTANT(Aspect_TOM_POINT);
        BIND_ENUM_CONSTANT(Aspect_TOM_PLUS);
        BIND_ENUM_CONSTANT(Aspect_TOM_STAR);
        BIND_ENUM_CONSTANT(Aspect_TOM_O);
        BIND_ENUM_CONSTANT(Aspect_TOM_X);
        BIND_ENUM_CONSTANT(Aspect_TOM_O_POINT);
        BIND_ENUM_CONSTANT(Aspect_TOM_O_PLUS);
        BIND_ENUM_CONSTANT(Aspect_TOM_O_STAR);
        BIND_ENUM_CONSTANT(Aspect_TOM_O_X);
        BIND_ENUM_CONSTANT(Aspect_TOM_BALL);
        BIND_ENUM_CONSTANT(Aspect_TOM_USERDEFINED);
    }
};

VARIANT_ENUM_CAST(ocgd_Aspect_TypeOfMarker::Type);