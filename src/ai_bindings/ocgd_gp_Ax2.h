#ifndef _ocgd_gp_Ax2_h
#define _ocgd_gp_Ax2_h

/**
 * ocgd_gp_Ax2.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Ax2 as a Godot class.
 * 
 * A gp_Ax2 represents a right-handed coordinate system in 3D space.
 * It is defined by:
 * - its origin (also referred to as its "Location point")
 * - its "main Direction" (also referred to as its "Axis" or "Z Direction")
 * - a "X Direction"
 * - a "Y Direction" (computed automatically to have a right-handed system)
 * 
 * Original OCCT header: <opencascade/gp_Ax2.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Ax2.hxx>

#include "ocgd_gp_Dir.h"
#include "ocgd_gp_Pnt.h"
#include "ocgd_gp_Ax1.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Ax2
 *
 * Wrapper class to expose the OpenCASCADE gp_Ax2 class to Godot.
 * 
 * Defines a right-handed coordinate system in 3D space.
 * Used for geometric constructions in 3D space that require
 * a complete reference system.
 */
class ocgd_gp_Ax2 : public RefCounted {
    GDCLASS(ocgd_gp_Ax2, RefCounted)

private:
    gp_Ax2 ax; // The actual OpenCASCADE coordinate system

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Ax2", D_METHOD("new"), &ocgd_gp_Ax2::_new);
        
        // Direction and position access
        ClassDB::bind_method(D_METHOD("SetAxis", "A"), &ocgd_gp_Ax2::SetAxis);
        ClassDB::bind_method(D_METHOD("SetDirection", "V"), &ocgd_gp_Ax2::SetDirection);
        ClassDB::bind_method(D_METHOD("SetLocation", "P"), &ocgd_gp_Ax2::SetLocation);
        ClassDB::bind_method(D_METHOD("SetXDirection", "V"), &ocgd_gp_Ax2::SetXDirection);
        ClassDB::bind_method(D_METHOD("SetYDirection", "V"), &ocgd_gp_Ax2::SetYDirection);

        ClassDB::bind_method(D_METHOD("Axis"), &ocgd_gp_Ax2::Axis);
        ClassDB::bind_method(D_METHOD("Direction"), &ocgd_gp_Ax2::Direction);
        ClassDB::bind_method(D_METHOD("Location"), &ocgd_gp_Ax2::Location);
        ClassDB::bind_method(D_METHOD("XDirection"), &ocgd_gp_Ax2::XDirection);
        ClassDB::bind_method(D_METHOD("YDirection"), &ocgd_gp_Ax2::YDirection);

        // Coordinate system operations
        ClassDB::bind_method(D_METHOD("Angle", "Other"), &ocgd_gp_Ax2::Angle);
        ClassDB::bind_method(D_METHOD("IsCoplanarAx2", "Other", "AngularTolerance", "LinearTolerance"), &ocgd_gp_Ax2::IsCoplanarAx2);
        ClassDB::bind_method(D_METHOD("IsCoplanarAx1", "A", "AngularTolerance", "LinearTolerance"), &ocgd_gp_Ax2::IsCoplanarAx1);
        
        // Basic transformations
        ClassDB::bind_method(D_METHOD("Mirror", "P"), &ocgd_gp_Ax2::Mirror_Point);
        ClassDB::bind_method(D_METHOD("Mirrored", "P"), &ocgd_gp_Ax2::Mirrored_Point);
        ClassDB::bind_method(D_METHOD("Scale", "P", "S"), &ocgd_gp_Ax2::Scale);
        ClassDB::bind_method(D_METHOD("Scaled", "P", "S"), &ocgd_gp_Ax2::Scaled);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "location", PROPERTY_HINT_NONE, "ocgd_gp_Pnt"), "SetLocation", "Location");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir"), "SetDirection", "Direction");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "x_direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir"), "SetXDirection", "XDirection");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "y_direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir"), "SetYDirection", "YDirection");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Ax2> _new() { return memnew(ocgd_gp_Ax2); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Ax2() : ax() {} // Creates an axis system with Z+ direction and origin (0,0,0)
    
    // Creates a coordinate system with origin P and Z direction V
    ocgd_gp_Ax2(const Ref<ocgd_gp_Pnt>& P, const Ref<ocgd_gp_Dir>& N) : ax(P->get_pnt(), N->get_dir()) {}
    
    // Creates a coordinate system with origin P, Z direction N and X direction Vx
    ocgd_gp_Ax2(const Ref<ocgd_gp_Pnt>& P, const Ref<ocgd_gp_Dir>& N, const Ref<ocgd_gp_Dir>& Vx) 
        : ax(P->get_pnt(), N->get_dir(), Vx->get_dir()) {}
    
    // Direction and position access methods
    void SetAxis(const Ref<ocgd_gp_Ax1>& A) { ax.SetAxis(A->get_ax()); }
    void SetDirection(const Ref<ocgd_gp_Dir>& V) { ax.SetDirection(V->get_dir()); }
    void SetLocation(const Ref<ocgd_gp_Pnt>& P) { ax.SetLocation(P->get_pnt()); }
    void SetXDirection(const Ref<ocgd_gp_Dir>& V) { ax.SetXDirection(V->get_dir()); }
    void SetYDirection(const Ref<ocgd_gp_Dir>& V) { ax.SetYDirection(V->get_dir()); }
    
    Ref<ocgd_gp_Ax1> Axis() const {
        Ref<ocgd_gp_Ax1> result;
        result.instantiate();
        result->get_ax() = ax.Axis();
        return result;
    }
    
    Ref<ocgd_gp_Dir> Direction() const {
        Ref<ocgd_gp_Dir> result;
        result.instantiate();
        result->get_dir() = ax.Direction();
        return result;
    }
    
    Ref<ocgd_gp_Pnt> Location() const {
        Ref<ocgd_gp_Pnt> result;
        result.instantiate();
        result->get_pnt() = ax.Location();
        return result;
    }
    
    Ref<ocgd_gp_Dir> XDirection() const {
        Ref<ocgd_gp_Dir> result;
        result.instantiate();
        result->get_dir() = ax.XDirection();
        return result;
    }
    
    Ref<ocgd_gp_Dir> YDirection() const {
        Ref<ocgd_gp_Dir> result;
        result.instantiate();
        result->get_dir() = ax.YDirection();
        return result;
    }
    
    // Coordinate system operations
    Standard_Real Angle(const Ref<ocgd_gp_Ax2>& Other) const {
        return ax.Angle(Other->get_ax());
    }
    
    Standard_Boolean IsCoplanarAx2(const Ref<ocgd_gp_Ax2>& Other,
        const Standard_Real AngularTolerance, const Standard_Real LinearTolerance) const {
        return ax.IsCoplanar(Other->get_ax(), AngularTolerance, LinearTolerance);
    }
    
    Standard_Boolean IsCoplanarAx1(const Ref<ocgd_gp_Ax1>& A,
        const Standard_Real AngularTolerance, const Standard_Real LinearTolerance) const {
        return ax.IsCoplanar(A->get_ax(), AngularTolerance, LinearTolerance);
    }
    
    // Basic transformations
    void Mirror_Point(const Ref<ocgd_gp_Pnt>& P) { ax.Mirror(P->get_pnt()); }
    Ref<ocgd_gp_Ax2> Mirrored_Point(const Ref<ocgd_gp_Pnt>& P) const {
        Ref<ocgd_gp_Ax2> result;
        result.instantiate();
        result->get_ax() = ax.Mirrored(P->get_pnt());
        return result;
    }
    
    void Scale(const Ref<ocgd_gp_Pnt>& P, const Standard_Real S) { ax.Scale(P->get_pnt(), S); }
    Ref<ocgd_gp_Ax2> Scaled(const Ref<ocgd_gp_Pnt>& P, const Standard_Real S) const {
        Ref<ocgd_gp_Ax2> result;
        result.instantiate();
        result->get_ax() = ax.Scaled(P->get_pnt(), S);
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Ax2& get_ax() const { return ax; }
    gp_Ax2& get_ax() { return ax; }
};

#endif // _ocgd_gp_Ax2_h