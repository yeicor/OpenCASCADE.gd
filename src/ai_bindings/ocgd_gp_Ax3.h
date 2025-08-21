#ifndef _ocgd_gp_Ax3_h
#define _ocgd_gp_Ax3_h

/**
 * ocgd_gp_Ax3.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Ax3 as a Godot class.
 * 
 * Unlike gp_Ax2, a gp_Ax3 coordinate system can be right-handed ("direct sense") 
 * or left-handed ("indirect sense"). A coordinate system is defined by:
 * - its origin (Location point)
 * - three orthogonal unit vectors (X Direction, Y Direction and main Direction/Z)
 * 
 * The main Direction is always parallel to X^Y. If right-handed:
 *   main Direction = X Direction ^ Y Direction
 * If left-handed:
 *   main Direction = -(X Direction ^ Y Direction)
 * 
 * The main Direction is not modified when either X or Y Direction is changed,
 * but X and Y are recomputed when main Direction changes.
 * 
 * Original OCCT header: <opencascade/gp_Ax3.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Ax3.hxx>

#include "ocgd_gp_Dir.h"
#include "ocgd_gp_Pnt.h"
#include "ocgd_gp_Ax1.h"
#include "ocgd_gp_Ax2.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Ax3
 *
 * Wrapper class to expose the OpenCASCADE gp_Ax3 class to Godot.
 * 
 * Defines a coordinate system in 3D space that can be right-handed
 * or left-handed. Used for geometric constructions in 3D space that
 * require a complete reference system.
 */
class ocgd_gp_Ax3 : public RefCounted {
    GDCLASS(ocgd_gp_Ax3, RefCounted)

private:
    gp_Ax3 ax; // The actual OpenCASCADE coordinate system

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Ax3", D_METHOD("new"), &ocgd_gp_Ax3::_new);
        
        // Direction reversals
        ClassDB::bind_method(D_METHOD("XReverse"), &ocgd_gp_Ax3::XReverse);
        ClassDB::bind_method(D_METHOD("YReverse"), &ocgd_gp_Ax3::YReverse);
        ClassDB::bind_method(D_METHOD("ZReverse"), &ocgd_gp_Ax3::ZReverse);
        
        // Direction and position access
        ClassDB::bind_method(D_METHOD("SetAxis", "A"), &ocgd_gp_Ax3::SetAxis);
        ClassDB::bind_method(D_METHOD("SetDirection", "V"), &ocgd_gp_Ax3::SetDirection);
        ClassDB::bind_method(D_METHOD("SetLocation", "P"), &ocgd_gp_Ax3::SetLocation);
        ClassDB::bind_method(D_METHOD("SetXDirection", "V"), &ocgd_gp_Ax3::SetXDirection);
        ClassDB::bind_method(D_METHOD("SetYDirection", "V"), &ocgd_gp_Ax3::SetYDirection);

        ClassDB::bind_method(D_METHOD("Angle", "Other"), &ocgd_gp_Ax3::Angle);
        ClassDB::bind_method(D_METHOD("Axis"), &ocgd_gp_Ax3::Axis);
        ClassDB::bind_method(D_METHOD("Ax2"), &ocgd_gp_Ax3::Ax2);
        ClassDB::bind_method(D_METHOD("Direction"), &ocgd_gp_Ax3::Direction);
        ClassDB::bind_method(D_METHOD("Location"), &ocgd_gp_Ax3::Location);
        ClassDB::bind_method(D_METHOD("XDirection"), &ocgd_gp_Ax3::XDirection);
        ClassDB::bind_method(D_METHOD("YDirection"), &ocgd_gp_Ax3::YDirection);
        ClassDB::bind_method(D_METHOD("Direct"), &ocgd_gp_Ax3::Direct);

        // Coordinate system operations
        ClassDB::bind_method(D_METHOD("IsCoplanarAx3", "Other", "AngularTolerance", "LinearTolerance"), &ocgd_gp_Ax3::IsCoplanarAx3);
        ClassDB::bind_method(D_METHOD("IsCoplanarAx2", "A", "AngularTolerance", "LinearTolerance"), &ocgd_gp_Ax3::IsCoplanarAx2);
        
        // Basic transformations (mirror by point)
        ClassDB::bind_method(D_METHOD("Mirror", "P"), &ocgd_gp_Ax3::Mirror_Point);
        ClassDB::bind_method(D_METHOD("Mirrored", "P"), &ocgd_gp_Ax3::Mirrored_Point);
        ClassDB::bind_method(D_METHOD("Scale", "P", "S"), &ocgd_gp_Ax3::Scale);
        ClassDB::bind_method(D_METHOD("Scaled", "P", "S"), &ocgd_gp_Ax3::Scaled);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "location", PROPERTY_HINT_NONE, "ocgd_gp_Pnt"), "SetLocation", "Location");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir"), "SetDirection", "Direction");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "x_direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir"), "SetXDirection", "XDirection");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "y_direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir"), "SetYDirection", "YDirection");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Ax3> _new() { return memnew(ocgd_gp_Ax3); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Ax3() : ax() {} // Creates a coordinate system with +Z axis and origin (0,0,0)
    
    // Creates a coordinate system from a right-handed coordinate system
    ocgd_gp_Ax3(const Ref<ocgd_gp_Ax2>& A) : ax(A->get_ax()) {}
    
    // Creates a coordinate system with origin P and Z direction V
    ocgd_gp_Ax3(const Ref<ocgd_gp_Pnt>& P, const Ref<ocgd_gp_Dir>& V) : ax(P->get_pnt(), V->get_dir()) {}
    
    // Creates a coordinate system with origin P, Z direction N and X direction Vx
    ocgd_gp_Ax3(const Ref<ocgd_gp_Pnt>& P, const Ref<ocgd_gp_Dir>& N, const Ref<ocgd_gp_Dir>& Vx) 
        : ax(P->get_pnt(), N->get_dir(), Vx->get_dir()) {}
    
    // Direction reversals
    void XReverse() { ax.XReverse(); }
    void YReverse() { ax.YReverse(); }
    void ZReverse() { ax.ZReverse(); }
    
    // Direction and position access methods
    void SetAxis(const Ref<ocgd_gp_Ax1>& A) { ax.SetAxis(A->get_ax()); }
    void SetDirection(const Ref<ocgd_gp_Dir>& V) { ax.SetDirection(V->get_dir()); }
    void SetLocation(const Ref<ocgd_gp_Pnt>& P) { ax.SetLocation(P->get_pnt()); }
    void SetXDirection(const Ref<ocgd_gp_Dir>& V) { ax.SetXDirection(V->get_dir()); }
    void SetYDirection(const Ref<ocgd_gp_Dir>& V) { ax.SetYDirection(V->get_dir()); }
    
    Standard_Real Angle(const Ref<ocgd_gp_Ax3>& Other) const {
        return ax.Angle(Other->get_ax());
    }
    
    Ref<ocgd_gp_Ax1> Axis() const {
        Ref<ocgd_gp_Ax1> result;
        result.instantiate();
        result->get_ax() = ax.Axis();
        return result;
    }
    
    Ref<ocgd_gp_Ax2> Ax2() const {
        Ref<ocgd_gp_Ax2> result;
        result.instantiate();
        result->get_ax() = ax.Ax2();
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
    Standard_Boolean Direct() const { return ax.Direct(); }
    
    Standard_Boolean IsCoplanarAx3(const Ref<ocgd_gp_Ax3>& Other,
        const Standard_Real AngularTolerance, const Standard_Real LinearTolerance) const {
        return ax.IsCoplanar(Other->get_ax(), AngularTolerance, LinearTolerance);
    }
    
    Standard_Boolean IsCoplanarAx2(const Ref<ocgd_gp_Ax2>& A,
        const Standard_Real AngularTolerance, const Standard_Real LinearTolerance) const {
        return ax.IsCoplanar(A->get_ax(), AngularTolerance, LinearTolerance);
    }
    
    // Basic transformations (mirror by point and scale)
    void Mirror_Point(const Ref<ocgd_gp_Pnt>& P) { ax.Mirror(P->get_pnt()); }
    Ref<ocgd_gp_Ax3> Mirrored_Point(const Ref<ocgd_gp_Pnt>& P) const {
        Ref<ocgd_gp_Ax3> result;
        result.instantiate();
        result->get_ax() = ax.Mirrored(P->get_pnt());
        return result;
    }
    
    void Scale(const Ref<ocgd_gp_Pnt>& P, const Standard_Real S) { ax.Scale(P->get_pnt(), S); }
    Ref<ocgd_gp_Ax3> Scaled(const Ref<ocgd_gp_Pnt>& P, const Standard_Real S) const {
        Ref<ocgd_gp_Ax3> result;
        result.instantiate();
        result->get_ax() = ax.Scaled(P->get_pnt(), S);
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Ax3& get_ax() const { return ax; }
    gp_Ax3& get_ax() { return ax; }
};

#endif // _ocgd_gp_Ax3_h