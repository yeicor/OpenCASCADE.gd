#ifndef _ocgd_gp_Ax2d_h
#define _ocgd_gp_Ax2d_h

/**
 * ocgd_gp_Ax2d.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Ax2d as a Godot class.
 * 
 * A gp_Ax2d represents a coordinate system in 2D space (origin + direction).
 * Used to describe coordinate systems for geometric constructions.
 * 
 * Original OCCT header: <opencascade/gp_Ax2d.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Ax2d.hxx>

#include "ocgd_gp_Dir2d.h"
#include "ocgd_gp_Pnt2d.h"
#include "ocgd_gp_Vec2d.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Ax2d
 *
 * Wrapper class to expose the OpenCASCADE gp_Ax2d class to Godot.
 * 
 * Defines a right-handed coordinate system in the plane (2D space).
 * A coordinate system is defined by:
 * - its origin (a point)
 * - its "X Direction"
 */
class ocgd_gp_Ax2d : public RefCounted {
    GDCLASS(ocgd_gp_Ax2d, RefCounted)

private:
    gp_Ax2d ax; // The actual OpenCASCADE axis

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Ax2d", D_METHOD("new"), &ocgd_gp_Ax2d::_new);
        
        // Direction and position access
        ClassDB::bind_method(D_METHOD("SetDirection", "V"), &ocgd_gp_Ax2d::SetDirection);
        ClassDB::bind_method(D_METHOD("SetLocation", "P"), &ocgd_gp_Ax2d::SetLocation);
        ClassDB::bind_method(D_METHOD("Direction"), &ocgd_gp_Ax2d::Direction);
        ClassDB::bind_method(D_METHOD("Location"), &ocgd_gp_Ax2d::Location);
        
        // Axis operations
        ClassDB::bind_method(D_METHOD("IsCoaxial", "Other", "AngularTolerance", "LinearTolerance"), &ocgd_gp_Ax2d::IsCoaxial);
        ClassDB::bind_method(D_METHOD("IsNormal", "Other", "AngularTolerance"), &ocgd_gp_Ax2d::IsNormal);
        ClassDB::bind_method(D_METHOD("IsOpposite", "Other", "AngularTolerance"), &ocgd_gp_Ax2d::IsOpposite);
        ClassDB::bind_method(D_METHOD("IsParallel", "Other", "AngularTolerance"), &ocgd_gp_Ax2d::IsParallel);
        ClassDB::bind_method(D_METHOD("Angle", "Other"), &ocgd_gp_Ax2d::Angle);
        ClassDB::bind_method(D_METHOD("Reverse"), &ocgd_gp_Ax2d::Reverse);
        ClassDB::bind_method(D_METHOD("Reversed"), &ocgd_gp_Ax2d::Reversed);
        
        // Transformations
        ClassDB::bind_method(D_METHOD("Rotate", "P", "Ang"), &ocgd_gp_Ax2d::Rotate);
        ClassDB::bind_method(D_METHOD("Rotated", "P", "Ang"), &ocgd_gp_Ax2d::Rotated);
        ClassDB::bind_method(D_METHOD("Scale", "P", "S"), &ocgd_gp_Ax2d::Scale);
        ClassDB::bind_method(D_METHOD("Scaled", "P", "S"), &ocgd_gp_Ax2d::Scaled);
        ClassDB::bind_method(D_METHOD("Translate", "V"), &ocgd_gp_Ax2d::Translate_Vec);
        ClassDB::bind_method(D_METHOD("Translated", "V"), &ocgd_gp_Ax2d::Translated_Vec);
        ClassDB::bind_method(D_METHOD("TranslatePoints", "P1", "P2"), &ocgd_gp_Ax2d::Translate_Points);
        ClassDB::bind_method(D_METHOD("TranslatedPoints", "P1", "P2"), &ocgd_gp_Ax2d::Translated_Points);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir2d"), "SetDirection", "Direction");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "location", PROPERTY_HINT_NONE, "ocgd_gp_Pnt2d"), "SetLocation", "Location");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Ax2d> _new() { return memnew(ocgd_gp_Ax2d); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Ax2d() : ax() {} // Creates an axis with origin (0,0) and direction (+X)
    
    // Creates an axis with given origin and direction
    ocgd_gp_Ax2d(const Ref<ocgd_gp_Pnt2d>& P, const Ref<ocgd_gp_Dir2d>& V) : ax(P->get_pnt(), V->get_dir()) {}
    
    // Direction and position access methods
    void SetDirection(const Ref<ocgd_gp_Dir2d>& V) { ax.SetDirection(V->get_dir()); }
    void SetLocation(const Ref<ocgd_gp_Pnt2d>& P) { ax.SetLocation(P->get_pnt()); }
    
    Ref<ocgd_gp_Dir2d> Direction() const {
        Ref<ocgd_gp_Dir2d> dir;
        dir.instantiate();
        dir->get_dir() = ax.Direction();
        return dir;
    }
    
    Ref<ocgd_gp_Pnt2d> Location() const {
        Ref<ocgd_gp_Pnt2d> pnt;
        pnt.instantiate();
        pnt->get_pnt() = ax.Location();
        return pnt;
    }
    
    // Axis operations
    Standard_Boolean IsCoaxial(const Ref<ocgd_gp_Ax2d>& Other, const Standard_Real AngularTolerance, const Standard_Real LinearTolerance) const {
        return ax.IsCoaxial(Other->get_ax(), AngularTolerance, LinearTolerance);
    }
    
    Standard_Boolean IsNormal(const Ref<ocgd_gp_Ax2d>& Other, const Standard_Real AngularTolerance) const {
        return ax.IsNormal(Other->get_ax(), AngularTolerance);
    }
    
    Standard_Boolean IsOpposite(const Ref<ocgd_gp_Ax2d>& Other, const Standard_Real AngularTolerance) const {
        return ax.IsOpposite(Other->get_ax(), AngularTolerance);
    }
    
    Standard_Boolean IsParallel(const Ref<ocgd_gp_Ax2d>& Other, const Standard_Real AngularTolerance) const {
        return ax.IsParallel(Other->get_ax(), AngularTolerance);
    }
    
    Standard_Real Angle(const Ref<ocgd_gp_Ax2d>& Other) const {
        return ax.Angle(Other->get_ax());
    }
    
    void Reverse() { ax.Reverse(); }
    Ref<ocgd_gp_Ax2d> Reversed() const {
        Ref<ocgd_gp_Ax2d> result;
        result.instantiate();
        result->get_ax() = ax.Reversed();
        return result;
    }
    
    // Transformations
    void Rotate(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real Ang) { ax.Rotate(P->get_pnt(), Ang); }
    Ref<ocgd_gp_Ax2d> Rotated(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real Ang) const {
        Ref<ocgd_gp_Ax2d> result;
        result.instantiate();
        result->get_ax() = ax.Rotated(P->get_pnt(), Ang);
        return result;
    }
    
    void Scale(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real S) { ax.Scale(P->get_pnt(), S); }
    Ref<ocgd_gp_Ax2d> Scaled(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real S) const {
        Ref<ocgd_gp_Ax2d> result;
        result.instantiate();
        result->get_ax() = ax.Scaled(P->get_pnt(), S);
        return result;
    }
    
    void Translate_Vec(const Ref<ocgd_gp_Vec2d>& V) { ax.Translate(V->get_vec()); }
    Ref<ocgd_gp_Ax2d> Translated_Vec(const Ref<ocgd_gp_Vec2d>& V) const {
        Ref<ocgd_gp_Ax2d> result;
        result.instantiate();
        result->get_ax() = ax.Translated(V->get_vec());
        return result;
    }
    
    void Translate_Points(const Ref<ocgd_gp_Pnt2d>& P1, const Ref<ocgd_gp_Pnt2d>& P2) {
        ax.Translate(P1->get_pnt(), P2->get_pnt());
    }
    Ref<ocgd_gp_Ax2d> Translated_Points(const Ref<ocgd_gp_Pnt2d>& P1, const Ref<ocgd_gp_Pnt2d>& P2) const {
        Ref<ocgd_gp_Ax2d> result;
        result.instantiate();
        result->get_ax() = ax.Translated(P1->get_pnt(), P2->get_pnt());
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Ax2d& get_ax() const { return ax; }
    gp_Ax2d& get_ax() { return ax; }
};

#endif // _ocgd_gp_Ax2d_h