#ifndef _ocgd_gp_Ax1_h
#define _ocgd_gp_Ax1_h

/**
 * ocgd_gp_Ax1.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Ax1 as a Godot class.
 * 
 * A gp_Ax1 represents an axis in 3D space. An axis is defined by:
 * - a point (origin)
 * - a direction
 * 
 * Original OCCT header: <opencascade/gp_Ax1.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Ax1.hxx>

#include "ocgd_gp_Dir.h"
#include "ocgd_gp_Pnt.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Ax1
 *
 * Wrapper class to expose the OpenCASCADE gp_Ax1 class to Godot.
 * 
 * Describes an axis in 3D space (a line with a direction).
 * Used for geometric transformations such as rotations and
 * for the definition of other geometric entities.
 */
class ocgd_gp_Ax1 : public RefCounted {
    GDCLASS(ocgd_gp_Ax1, RefCounted)

private:
    gp_Ax1 ax; // The actual OpenCASCADE axis

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Ax1", D_METHOD("new"), &ocgd_gp_Ax1::_new);
        
        // Direction and position access
        ClassDB::bind_method(D_METHOD("SetDirection", "V"), &ocgd_gp_Ax1::SetDirection);
        ClassDB::bind_method(D_METHOD("SetLocation", "P"), &ocgd_gp_Ax1::SetLocation);
        ClassDB::bind_method(D_METHOD("Direction"), &ocgd_gp_Ax1::Direction);
        ClassDB::bind_method(D_METHOD("Location"), &ocgd_gp_Ax1::Location);
        
        // Axis operations
        ClassDB::bind_method(D_METHOD("IsCoaxial", "Other", "AngularTolerance", "LinearTolerance"), &ocgd_gp_Ax1::IsCoaxial);
        ClassDB::bind_method(D_METHOD("IsNormal", "Other", "AngularTolerance"), &ocgd_gp_Ax1::IsNormal);
        ClassDB::bind_method(D_METHOD("IsOpposite", "Other", "AngularTolerance"), &ocgd_gp_Ax1::IsOpposite);
        ClassDB::bind_method(D_METHOD("IsParallel", "Other", "AngularTolerance"), &ocgd_gp_Ax1::IsParallel);
        ClassDB::bind_method(D_METHOD("Angle", "Other"), &ocgd_gp_Ax1::Angle);
        ClassDB::bind_method(D_METHOD("Reverse"), &ocgd_gp_Ax1::Reverse);
        ClassDB::bind_method(D_METHOD("Reversed"), &ocgd_gp_Ax1::Reversed);
        
        // Basic transformations
        ClassDB::bind_method(D_METHOD("Mirror", "P"), &ocgd_gp_Ax1::Mirror_Point);
        ClassDB::bind_method(D_METHOD("Mirrored", "P"), &ocgd_gp_Ax1::Mirrored_Point);
        ClassDB::bind_method(D_METHOD("Scale", "P", "S"), &ocgd_gp_Ax1::Scale);
        ClassDB::bind_method(D_METHOD("Scaled", "P", "S"), &ocgd_gp_Ax1::Scaled);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "location", PROPERTY_HINT_NONE, "ocgd_gp_Pnt"), "SetLocation", "Location");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir"), "SetDirection", "Direction");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Ax1> _new() { return memnew(ocgd_gp_Ax1); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Ax1() : ax() {} // Creates an axis with origin (0,0,0) and direction +Z
    
    // Creates an axis with given origin and direction
    ocgd_gp_Ax1(const Ref<ocgd_gp_Pnt>& P, const Ref<ocgd_gp_Dir>& V) : ax(P->get_pnt(), V->get_dir()) {}
    
    // Direction and position access methods
    void SetDirection(const Ref<ocgd_gp_Dir>& V) { ax.SetDirection(V->get_dir()); }
    void SetLocation(const Ref<ocgd_gp_Pnt>& P) { ax.SetLocation(P->get_pnt()); }
    
    Ref<ocgd_gp_Dir> Direction() const {
        Ref<ocgd_gp_Dir> dir;
        dir.instantiate();
        dir->get_dir() = ax.Direction();
        return dir;
    }
    
    Ref<ocgd_gp_Pnt> Location() const {
        Ref<ocgd_gp_Pnt> pnt;
        pnt.instantiate();
        pnt->get_pnt() = ax.Location();
        return pnt;
    }
    
    // Axis operations
    Standard_Boolean IsCoaxial(const Ref<ocgd_gp_Ax1>& Other, const Standard_Real AngularTolerance, const Standard_Real LinearTolerance) const {
        return ax.IsCoaxial(Other->get_ax(), AngularTolerance, LinearTolerance);
    }
    
    Standard_Boolean IsNormal(const Ref<ocgd_gp_Ax1>& Other, const Standard_Real AngularTolerance) const {
        return ax.IsNormal(Other->get_ax(), AngularTolerance);
    }
    
    Standard_Boolean IsOpposite(const Ref<ocgd_gp_Ax1>& Other, const Standard_Real AngularTolerance) const {
        return ax.IsOpposite(Other->get_ax(), AngularTolerance);
    }
    
    Standard_Boolean IsParallel(const Ref<ocgd_gp_Ax1>& Other, const Standard_Real AngularTolerance) const {
        return ax.IsParallel(Other->get_ax(), AngularTolerance);
    }
    
    Standard_Real Angle(const Ref<ocgd_gp_Ax1>& Other) const {
        return ax.Angle(Other->get_ax());
    }
    
    void Reverse() { ax.Reverse(); }
    Ref<ocgd_gp_Ax1> Reversed() const {
        Ref<ocgd_gp_Ax1> result;
        result.instantiate();
        result->get_ax() = ax.Reversed();
        return result;
    }
    
    // Basic transformations
    void Mirror_Point(const Ref<ocgd_gp_Pnt>& P) { ax.Mirror(P->get_pnt()); }
    Ref<ocgd_gp_Ax1> Mirrored_Point(const Ref<ocgd_gp_Pnt>& P) const {
        Ref<ocgd_gp_Ax1> result;
        result.instantiate();
        result->get_ax() = ax.Mirrored(P->get_pnt());
        return result;
    }
    
    void Scale(const Ref<ocgd_gp_Pnt>& P, const Standard_Real S) { ax.Scale(P->get_pnt(), S); }
    Ref<ocgd_gp_Ax1> Scaled(const Ref<ocgd_gp_Pnt>& P, const Standard_Real S) const {
        Ref<ocgd_gp_Ax1> result;
        result.instantiate();
        result->get_ax() = ax.Scaled(P->get_pnt(), S);
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Ax1& get_ax() const { return ax; }
    gp_Ax1& get_ax() { return ax; }
};

#endif // _ocgd_gp_Ax1_h