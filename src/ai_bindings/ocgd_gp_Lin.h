#ifndef _ocgd_gp_Lin_h
#define _ocgd_gp_Lin_h

/**
 * ocgd_gp_Lin.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Lin as a Godot class.
 * 
 * A gp_Lin represents a line in 3D space. A line is positioned in space with an
 * axis (a gp_Ax1 object) which gives it an origin and a unit vector.
 * 
 * Original OCCT header: <opencascade/gp_Lin.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Lin.hxx>

#include "ocgd_gp_Dir.h"
#include "ocgd_gp_Pnt.h"
#include "ocgd_gp_Vec.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Lin
 *
 * Wrapper class to expose the OpenCASCADE gp_Lin class to Godot.
 * 
 * Defines a line in 3D space. A line is positioned with an origin point
 * and a unit vector direction.
 */
class ocgd_gp_Lin : public RefCounted {
    GDCLASS(ocgd_gp_Lin, RefCounted)

private:
    gp_Lin lin; // The actual OpenCASCADE line

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Lin", D_METHOD("new"), &ocgd_gp_Lin::_new);
        
        // Direction and position access
        ClassDB::bind_method(D_METHOD("SetDirection", "V"), &ocgd_gp_Lin::SetDirection);
        ClassDB::bind_method(D_METHOD("SetLocation", "P"), &ocgd_gp_Lin::SetLocation);
        ClassDB::bind_method(D_METHOD("Direction"), &ocgd_gp_Lin::Direction);
        ClassDB::bind_method(D_METHOD("Location"), &ocgd_gp_Lin::Location);
        
        // Line operations
        ClassDB::bind_method(D_METHOD("Reverse"), &ocgd_gp_Lin::Reverse);
        ClassDB::bind_method(D_METHOD("Reversed"), &ocgd_gp_Lin::Reversed);
        
        // Geometric queries
        ClassDB::bind_method(D_METHOD("Angle", "Other"), &ocgd_gp_Lin::Angle);
        ClassDB::bind_method(D_METHOD("Contains", "P", "LinearTolerance"), &ocgd_gp_Lin::Contains);
        ClassDB::bind_method(D_METHOD("Distance", "P"), static_cast<Standard_Real (ocgd_gp_Lin::*)(const Ref<ocgd_gp_Pnt>&) const>(&ocgd_gp_Lin::Distance));
        ClassDB::bind_method(D_METHOD("DistanceToLine", "Other"), static_cast<Standard_Real (ocgd_gp_Lin::*)(const Ref<ocgd_gp_Lin>&) const>(&ocgd_gp_Lin::Distance));
        ClassDB::bind_method(D_METHOD("SquareDistance", "P"), static_cast<Standard_Real (ocgd_gp_Lin::*)(const Ref<ocgd_gp_Pnt>&) const>(&ocgd_gp_Lin::SquareDistance));
        ClassDB::bind_method(D_METHOD("SquareDistanceToLine", "Other"), static_cast<Standard_Real (ocgd_gp_Lin::*)(const Ref<ocgd_gp_Lin>&) const>(&ocgd_gp_Lin::SquareDistance));
        ClassDB::bind_method(D_METHOD("Normal", "P"), &ocgd_gp_Lin::Normal);
        
        // Transformations
        ClassDB::bind_method(D_METHOD("Translate", "V"), static_cast<void (ocgd_gp_Lin::*)(const Ref<ocgd_gp_Vec>&)>(&ocgd_gp_Lin::Translate));
        ClassDB::bind_method(D_METHOD("Translated", "V"), static_cast<Ref<ocgd_gp_Lin> (ocgd_gp_Lin::*)(const Ref<ocgd_gp_Vec>&) const>(&ocgd_gp_Lin::Translated));
        ClassDB::bind_method(D_METHOD("TranslatePoints", "P1", "P2"), static_cast<void (ocgd_gp_Lin::*)(const Ref<ocgd_gp_Pnt>&, const Ref<ocgd_gp_Pnt>&)>(&ocgd_gp_Lin::Translate));
        ClassDB::bind_method(D_METHOD("TranslatedPoints", "P1", "P2"), static_cast<Ref<ocgd_gp_Lin> (ocgd_gp_Lin::*)(const Ref<ocgd_gp_Pnt>&, const Ref<ocgd_gp_Pnt>&) const>(&ocgd_gp_Lin::Translated));
        ClassDB::bind_method(D_METHOD("Scale", "P", "S"), &ocgd_gp_Lin::Scale);
        ClassDB::bind_method(D_METHOD("Scaled", "P", "S"), &ocgd_gp_Lin::Scaled);
        ClassDB::bind_method(D_METHOD("Rotate", "P", "Ang"), &ocgd_gp_Lin::Rotate);
        ClassDB::bind_method(D_METHOD("Rotated", "P", "Ang"), &ocgd_gp_Lin::Rotated);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir"), "SetDirection", "Direction");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "location", PROPERTY_HINT_NONE, "ocgd_gp_Pnt"), "SetLocation", "Location");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Lin> _new() { return memnew(ocgd_gp_Lin); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Lin() : lin() {} // Creates a line corresponding to Z axis
    
    // Creates a line passing through point P and parallel to direction V
    ocgd_gp_Lin(const Ref<ocgd_gp_Pnt>& P, const Ref<ocgd_gp_Dir>& V) : lin(P->get_pnt(), V->get_dir()) {}
    
    // Direction and position access methods
    void SetDirection(const Ref<ocgd_gp_Dir>& V) { lin.SetDirection(V->get_dir()); }
    void SetLocation(const Ref<ocgd_gp_Pnt>& P) { lin.SetLocation(P->get_pnt()); }
    
    Ref<ocgd_gp_Dir> Direction() const {
        Ref<ocgd_gp_Dir> dir;
        dir.instantiate();
        dir->get_dir() = lin.Direction();
        return dir;
    }
    
    Ref<ocgd_gp_Pnt> Location() const {
        Ref<ocgd_gp_Pnt> pnt;
        pnt.instantiate();
        pnt->get_pnt() = lin.Location();
        return pnt;
    }
    
    // Line operations
    void Reverse() { lin.Reverse(); }
    Ref<ocgd_gp_Lin> Reversed() const {
        Ref<ocgd_gp_Lin> result;
        result.instantiate();
        result->get_lin() = lin.Reversed();
        return result;
    }
    
    // Geometric queries
    Standard_Real Angle(const Ref<ocgd_gp_Lin>& Other) const {
        return lin.Angle(Other->get_lin());
    }
    
    Standard_Boolean Contains(const Ref<ocgd_gp_Pnt>& P, const Standard_Real LinearTolerance) const {
        return lin.Contains(P->get_pnt(), LinearTolerance);
    }
    
    Standard_Real Distance(const Ref<ocgd_gp_Pnt>& P) const {
        return lin.Distance(P->get_pnt());
    }
    
    Standard_Real Distance(const Ref<ocgd_gp_Lin>& Other) const {
        return lin.Distance(Other->get_lin());
    }
    
    Standard_Real SquareDistance(const Ref<ocgd_gp_Pnt>& P) const {
        return lin.SquareDistance(P->get_pnt());
    }
    
    Standard_Real SquareDistance(const Ref<ocgd_gp_Lin>& Other) const {
        return lin.SquareDistance(Other->get_lin());
    }
    
    Ref<ocgd_gp_Lin> Normal(const Ref<ocgd_gp_Pnt>& P) const {
        Ref<ocgd_gp_Lin> result;
        result.instantiate();
        result->get_lin() = lin.Normal(P->get_pnt());
        return result;
    }
    
    // Transformations
    void Translate(const Ref<ocgd_gp_Vec>& V) { lin.Translate(V->get_vec()); }
    Ref<ocgd_gp_Lin> Translated(const Ref<ocgd_gp_Vec>& V) const {
        Ref<ocgd_gp_Lin> result;
        result.instantiate();
        result->get_lin() = lin.Translated(V->get_vec());
        return result;
    }
    
    void Translate(const Ref<ocgd_gp_Pnt>& P1, const Ref<ocgd_gp_Pnt>& P2) {
        lin.Translate(P1->get_pnt(), P2->get_pnt());
    }
    Ref<ocgd_gp_Lin> Translated(const Ref<ocgd_gp_Pnt>& P1, const Ref<ocgd_gp_Pnt>& P2) const {
        Ref<ocgd_gp_Lin> result;
        result.instantiate();
        result->get_lin() = lin.Translated(P1->get_pnt(), P2->get_pnt());
        return result;
    }
    
    void Scale(const Ref<ocgd_gp_Pnt>& P, const Standard_Real S) { lin.Scale(P->get_pnt(), S); }
    Ref<ocgd_gp_Lin> Scaled(const Ref<ocgd_gp_Pnt>& P, const Standard_Real S) const {
        Ref<ocgd_gp_Lin> result;
        result.instantiate();
        result->get_lin() = lin.Scaled(P->get_pnt(), S);
        return result;
    }
    
    void Rotate(const Ref<ocgd_gp_Pnt>& P, const Standard_Real Ang) { 
        gp_Dir zDir(0.0, 0.0, 1.0);
        lin.Rotate(gp_Ax1(P->get_pnt(), zDir), Ang); 
    }
    Ref<ocgd_gp_Lin> Rotated(const Ref<ocgd_gp_Pnt>& P, const Standard_Real Ang) const {
        Ref<ocgd_gp_Lin> result;
        result.instantiate();
        gp_Dir zDir(0.0, 0.0, 1.0);
        result->get_lin() = lin.Rotated(gp_Ax1(P->get_pnt(), zDir), Ang);
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Lin& get_lin() const { return lin; }
    gp_Lin& get_lin() { return lin; }
};

#endif // _ocgd_gp_Lin_h