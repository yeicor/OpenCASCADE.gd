#ifndef _ocgd_gp_Lin2d_h
#define _ocgd_gp_Lin2d_h

/**
 * ocgd_gp_Lin2d.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Lin2d as a Godot class.
 * 
 * A gp_Lin2d represents a line in 2D space. A line is positioned in space with an
 * origin point and a unit vector direction.
 * 
 * Original OCCT header: <opencascade/gp_Lin2d.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Lin2d.hxx>

#include "ocgd_gp_Dir2d.h"
#include "ocgd_gp_Pnt2d.h"
#include "ocgd_gp_Vec2d.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Lin2d
 *
 * Wrapper class to expose the OpenCASCADE gp_Lin2d class to Godot.
 * 
 * Defines a line in 2D space. A line is positioned with an origin point
 * and a unit vector direction.
 */
class ocgd_gp_Lin2d : public RefCounted {
    GDCLASS(ocgd_gp_Lin2d, RefCounted)

private:
    gp_Lin2d lin; // The actual OpenCASCADE line

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Lin2d", D_METHOD("new"), &ocgd_gp_Lin2d::_new);
        
        // Direction and position access
        ClassDB::bind_method(D_METHOD("SetDirection", "V"), &ocgd_gp_Lin2d::SetDirection);
        ClassDB::bind_method(D_METHOD("SetLocation", "P"), &ocgd_gp_Lin2d::SetLocation);
        ClassDB::bind_method(D_METHOD("Direction"), &ocgd_gp_Lin2d::Direction);
        ClassDB::bind_method(D_METHOD("Location"), &ocgd_gp_Lin2d::Location);
        
        // Line operations
        ClassDB::bind_method(D_METHOD("Reverse"), &ocgd_gp_Lin2d::Reverse);
        ClassDB::bind_method(D_METHOD("Reversed"), &ocgd_gp_Lin2d::Reversed);
        
        // Geometric queries
        ClassDB::bind_method(D_METHOD("Angle", "Other"), &ocgd_gp_Lin2d::Angle);
        ClassDB::bind_method(D_METHOD("Contains", "P", "LinearTolerance"), &ocgd_gp_Lin2d::Contains);
        ClassDB::bind_method(D_METHOD("Distance", "P"), &ocgd_gp_Lin2d::Distance_Point);
        ClassDB::bind_method(D_METHOD("DistanceToLine", "Other"), &ocgd_gp_Lin2d::Distance_Line);
        ClassDB::bind_method(D_METHOD("SquareDistance", "P"), &ocgd_gp_Lin2d::SquareDistance_Point);
        ClassDB::bind_method(D_METHOD("SquareDistanceToLine", "Other"), &ocgd_gp_Lin2d::SquareDistance_Line);
        ClassDB::bind_method(D_METHOD("Normal", "P"), &ocgd_gp_Lin2d::Normal);
        
        // Transformations
        ClassDB::bind_method(D_METHOD("Translate", "V"), &ocgd_gp_Lin2d::Translate_Vec);
        ClassDB::bind_method(D_METHOD("Translated", "V"), &ocgd_gp_Lin2d::Translated_Vec);
        ClassDB::bind_method(D_METHOD("TranslatePoints", "P1", "P2"), &ocgd_gp_Lin2d::Translate_Points);
        ClassDB::bind_method(D_METHOD("TranslatedPoints", "P1", "P2"), &ocgd_gp_Lin2d::Translated_Points);
        ClassDB::bind_method(D_METHOD("Scale", "P", "S"), &ocgd_gp_Lin2d::Scale);
        ClassDB::bind_method(D_METHOD("Scaled", "P", "S"), &ocgd_gp_Lin2d::Scaled);
        ClassDB::bind_method(D_METHOD("Rotate", "P", "Ang"), &ocgd_gp_Lin2d::Rotate);
        ClassDB::bind_method(D_METHOD("Rotated", "P", "Ang"), &ocgd_gp_Lin2d::Rotated);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir2d"), "SetDirection", "Direction");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "location", PROPERTY_HINT_NONE, "ocgd_gp_Pnt2d"), "SetLocation", "Location");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Lin2d> _new() { return memnew(ocgd_gp_Lin2d); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Lin2d() : lin() {} // Creates a line corresponding to X axis
    
    // Creates a line passing through point P and parallel to direction V
    ocgd_gp_Lin2d(const Ref<ocgd_gp_Pnt2d>& P, const Ref<ocgd_gp_Dir2d>& V) : lin(P->get_pnt(), V->get_dir()) {}
    
    // Direction and position access methods
    void SetDirection(const Ref<ocgd_gp_Dir2d>& V) { lin.SetDirection(V->get_dir()); }
    void SetLocation(const Ref<ocgd_gp_Pnt2d>& P) { lin.SetLocation(P->get_pnt()); }
    
    Ref<ocgd_gp_Dir2d> Direction() const {
        Ref<ocgd_gp_Dir2d> dir;
        dir.instantiate();
        dir->get_dir() = lin.Direction();
        return dir;
    }
    
    Ref<ocgd_gp_Pnt2d> Location() const {
        Ref<ocgd_gp_Pnt2d> pnt;
        pnt.instantiate();
        pnt->get_pnt() = lin.Location();
        return pnt;
    }
    
    // Line operations
    void Reverse() { lin.Reverse(); }
    Ref<ocgd_gp_Lin2d> Reversed() const {
        Ref<ocgd_gp_Lin2d> result;
        result.instantiate();
        result->get_lin() = lin.Reversed();
        return result;
    }
    
    // Geometric queries
    Standard_Real Angle(const Ref<ocgd_gp_Lin2d>& Other) const {
        return lin.Angle(Other->get_lin());
    }
    
    Standard_Boolean Contains(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real LinearTolerance) const {
        return lin.Contains(P->get_pnt(), LinearTolerance);
    }
    
    Standard_Real Distance_Point(const Ref<ocgd_gp_Pnt2d>& P) const {
        return lin.Distance(P->get_pnt());
    }
    
    Standard_Real Distance_Line(const Ref<ocgd_gp_Lin2d>& Other) const {
        return lin.Distance(Other->get_lin());
    }
    
    Standard_Real SquareDistance_Point(const Ref<ocgd_gp_Pnt2d>& P) const {
        return lin.SquareDistance(P->get_pnt());
    }
    
    Standard_Real SquareDistance_Line(const Ref<ocgd_gp_Lin2d>& Other) const {
        return lin.SquareDistance(Other->get_lin());
    }
    
    Ref<ocgd_gp_Lin2d> Normal(const Ref<ocgd_gp_Pnt2d>& P) const {
        Ref<ocgd_gp_Lin2d> result;
        result.instantiate();
        result->get_lin() = lin.Normal(P->get_pnt());
        return result;
    }
    
    // Transformations
    void Translate_Vec(const Ref<ocgd_gp_Vec2d>& V) { lin.Translate(V->get_vec()); }
    Ref<ocgd_gp_Lin2d> Translated_Vec(const Ref<ocgd_gp_Vec2d>& V) const {
        Ref<ocgd_gp_Lin2d> result;
        result.instantiate();
        result->get_lin() = lin.Translated(V->get_vec());
        return result;
    }
    
    void Translate_Points(const Ref<ocgd_gp_Pnt2d>& P1, const Ref<ocgd_gp_Pnt2d>& P2) {
        lin.Translate(P1->get_pnt(), P2->get_pnt());
    }
    Ref<ocgd_gp_Lin2d> Translated_Points(const Ref<ocgd_gp_Pnt2d>& P1, const Ref<ocgd_gp_Pnt2d>& P2) const {
        Ref<ocgd_gp_Lin2d> result;
        result.instantiate();
        result->get_lin() = lin.Translated(P1->get_pnt(), P2->get_pnt());
        return result;
    }
    
    void Scale(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real S) { lin.Scale(P->get_pnt(), S); }
    Ref<ocgd_gp_Lin2d> Scaled(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real S) const {
        Ref<ocgd_gp_Lin2d> result;
        result.instantiate();
        result->get_lin() = lin.Scaled(P->get_pnt(), S);
        return result;
    }
    
    void Rotate(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real Ang) { lin.Rotate(P->get_pnt(), Ang); }
    Ref<ocgd_gp_Lin2d> Rotated(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real Ang) const {
        Ref<ocgd_gp_Lin2d> result;
        result.instantiate();
        result->get_lin() = lin.Rotated(P->get_pnt(), Ang);
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Lin2d& get_lin() const { return lin; }
    gp_Lin2d& get_lin() { return lin; }
};

#endif // _ocgd_gp_Lin2d_h