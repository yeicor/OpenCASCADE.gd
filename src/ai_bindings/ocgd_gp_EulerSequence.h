#ifndef _ocgd_gp_EulerSequence_h
#define _ocgd_gp_EulerSequence_h

/**
 * ocgd_gp_EulerSequence.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_EulerSequence as a Godot class enum.
 *
 * Documentation for values is preserved from the original OpenCASCADE header.
 *
 * Original OCCT header: <opencascade/gp_EulerSequence.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_EulerSequence.hxx>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_EulerSequence
 *
 * Wrapper class to expose the OpenCASCADE gp_EulerSequence enumeration to Godot.
 *
 * Enumerates all 24 possible variants of generalized Euler angles, defining general 3d rotation 
 * by three rotations around main axes of coordinate system, in different possible orders.
 *
 * The name of each enum corresponds to order of rotations, prefixed by type
 * of coordinate system used:
 * - Intrinsic: rotations are made around axes of rotating coordinate system associated with the object
 * - Extrinsic: rotations are made around axes of fixed (static) coordinate system
 *
 * Two specific values are provided for most frequently used conventions: 
 * - Classic Euler angles (intrinsic ZXZ) via EulerAngles
 * - Yaw-pitch-roll (intrinsic ZYX) via YawPitchRoll
 *
 * In GDScript, use constants like:
 *   ocgd_gp_EulerSequence.EulerAngles
 *
 * Or treat them as an enum within this class.
 */
class ocgd_gp_EulerSequence : public Object {
    GDCLASS(ocgd_gp_EulerSequence, Object);

public:
    //! Definition of Euler angle sequences (mirrors OpenCASCADE gp_EulerSequence)
    enum Type {
        //! Classic Euler angles, alias to Intrinsic_ZXZ
        EulerAngles = ::gp_EulerAngles,

        //! Yaw Pitch Roll (or nautical) angles, alias to Intrinsic_ZYX
        YawPitchRoll = ::gp_YawPitchRoll,

        // Tait-Bryan angles (using three different axes)
        Extrinsic_XYZ = ::gp_Extrinsic_XYZ,
        Extrinsic_XZY = ::gp_Extrinsic_XZY,
        Extrinsic_YZX = ::gp_Extrinsic_YZX,
        Extrinsic_YXZ = ::gp_Extrinsic_YXZ,
        Extrinsic_ZXY = ::gp_Extrinsic_ZXY,
        Extrinsic_ZYX = ::gp_Extrinsic_ZYX,

        Intrinsic_XYZ = ::gp_Intrinsic_XYZ,
        Intrinsic_XZY = ::gp_Intrinsic_XZY,
        Intrinsic_YZX = ::gp_Intrinsic_YZX,
        Intrinsic_YXZ = ::gp_Intrinsic_YXZ,
        Intrinsic_ZXY = ::gp_Intrinsic_ZXY,
        Intrinsic_ZYX = ::gp_Intrinsic_ZYX,

        // Proper Euler angles (using two different axes, first and third the same)
        Extrinsic_XYX = ::gp_Extrinsic_XYX,
        Extrinsic_XZX = ::gp_Extrinsic_XZX,
        Extrinsic_YZY = ::gp_Extrinsic_YZY,
        Extrinsic_YXY = ::gp_Extrinsic_YXY,
        Extrinsic_ZYZ = ::gp_Extrinsic_ZYZ,
        Extrinsic_ZXZ = ::gp_Extrinsic_ZXZ,

        Intrinsic_XYX = ::gp_Intrinsic_XYX,
        Intrinsic_XZX = ::gp_Intrinsic_XZX,
        Intrinsic_YZY = ::gp_Intrinsic_YZY,
        Intrinsic_YXY = ::gp_Intrinsic_YXY,
        Intrinsic_ZXZ = ::gp_Intrinsic_ZXZ,
        Intrinsic_ZYZ = ::gp_Intrinsic_ZYZ
    };

protected:
    static void _bind_methods() {
        // Bind the enum constants so they are visible in GDScript as:
        // ocgd_gp_EulerSequence.*
        BIND_ENUM_CONSTANT(EulerAngles);
        BIND_ENUM_CONSTANT(YawPitchRoll);

        BIND_ENUM_CONSTANT(Extrinsic_XYZ);
        BIND_ENUM_CONSTANT(Extrinsic_XZY);
        BIND_ENUM_CONSTANT(Extrinsic_YZX);
        BIND_ENUM_CONSTANT(Extrinsic_YXZ);
        BIND_ENUM_CONSTANT(Extrinsic_ZXY);
        BIND_ENUM_CONSTANT(Extrinsic_ZYX);

        BIND_ENUM_CONSTANT(Intrinsic_XYZ);
        BIND_ENUM_CONSTANT(Intrinsic_XZY);
        BIND_ENUM_CONSTANT(Intrinsic_YZX);
        BIND_ENUM_CONSTANT(Intrinsic_YXZ);
        BIND_ENUM_CONSTANT(Intrinsic_ZXY);
        BIND_ENUM_CONSTANT(Intrinsic_ZYX);

        BIND_ENUM_CONSTANT(Extrinsic_XYX);
        BIND_ENUM_CONSTANT(Extrinsic_XZX);
        BIND_ENUM_CONSTANT(Extrinsic_YZY);
        BIND_ENUM_CONSTANT(Extrinsic_YXY);
        BIND_ENUM_CONSTANT(Extrinsic_ZYZ);
        BIND_ENUM_CONSTANT(Extrinsic_ZXZ);

        BIND_ENUM_CONSTANT(Intrinsic_XYX);
        BIND_ENUM_CONSTANT(Intrinsic_XZX);
        BIND_ENUM_CONSTANT(Intrinsic_YZY);
        BIND_ENUM_CONSTANT(Intrinsic_YXY);
        BIND_ENUM_CONSTANT(Intrinsic_ZXZ);
        BIND_ENUM_CONSTANT(Intrinsic_ZYZ);
    }
};

VARIANT_ENUM_CAST(ocgd_gp_EulerSequence::Type);

#endif // _ocgd_gp_EulerSequence_h