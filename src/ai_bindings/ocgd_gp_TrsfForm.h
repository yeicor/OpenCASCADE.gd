#ifndef _ocgd_gp_TrsfForm_h
#define _ocgd_gp_TrsfForm_h

/**
 * ocgd_gp_TrsfForm.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_TrsfForm as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/gp_TrsfForm.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_TrsfForm.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_TrsfForm
 *
 * Wrapper class to expose the OpenCASCADE gp_TrsfForm enumeration to Godot.
 *
 * Identifies the type of a geometric transformation.
 *
 * In GDScript, use constants like:
 *   ocgd_gp_TrsfForm.Identity
 *
 * Or treat them as an enum within this class.
 */
class ocgd_gp_TrsfForm : public Object {
    GDCLASS(ocgd_gp_TrsfForm, Object);

public:
    //! Definition of transformation types (mirrors OpenCASCADE gp_TrsfForm)
    enum Type {
        Identity = ::gp_Identity,         //!< No transformation (matrix is identity)
        Rotation = ::gp_Rotation,         //!< Rotation
        Translation = ::gp_Translation,    //!< Translation
        PntMirror = ::gp_PntMirror,      //!< Central symmetry
        Ax1Mirror = ::gp_Ax1Mirror,       //!< Rotational symmetry
        Ax2Mirror = ::gp_Ax2Mirror,       //!< Bilateral symmetry
        Scale = ::gp_Scale,               //!< Scale
        CompoundTrsf = ::gp_CompoundTrsf, //!< Combination of the above transformations
        Other = ::gp_Other                //!< Transformation with not-orthogonal matrix
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_gp_TrsfForm.*
        BIND_ENUM_CONSTANT(Identity);
        BIND_ENUM_CONSTANT(Rotation);
        BIND_ENUM_CONSTANT(Translation);
        BIND_ENUM_CONSTANT(PntMirror);
        BIND_ENUM_CONSTANT(Ax1Mirror);
        BIND_ENUM_CONSTANT(Ax2Mirror);
        BIND_ENUM_CONSTANT(Scale);
        BIND_ENUM_CONSTANT(CompoundTrsf);
        BIND_ENUM_CONSTANT(Other);
    }
};

VARIANT_ENUM_CAST(ocgd_gp_TrsfForm::Type);

#endif // _ocgd_gp_TrsfForm_h