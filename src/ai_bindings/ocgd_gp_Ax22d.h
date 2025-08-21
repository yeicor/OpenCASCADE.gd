#ifndef _ocgd_gp_Ax22d_h
#define _ocgd_gp_Ax22d_h

/**
 * ocgd_gp_Ax22d.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Ax22d as a Godot class.
 * 
 * A gp_Ax22d represents a complete coordinate system in 2D space (origin + 2 directions).
 * Used to describe right-handed coordinate systems for geometric constructions.
 * 
 * Original OCCT header: <opencascade/gp_Ax22d.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Ax22d.hxx>

#include "ocgd_gp_Dir2d.h"
#include "ocgd_gp_Pnt2d.h"
#include "ocgd_gp_Ax2d.h"
#include "ocgd_gp_Vec2d.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Ax22d
 *
 * Wrapper class to expose the OpenCASCADE gp_Ax22d class to Godot.
 * 
 * Defines a right-handed coordinate system in the plane (2D space).
 * A coordinate system is defined by:
 * - its origin (also referred to as its "Location")
 * - its "X Direction" and "Y Direction" (also referred to as its "XAxis" and "YAxis")
 */
class ocgd_gp_Ax22d : public RefCounted {
    GDCLASS(ocgd_gp_Ax22d, RefCounted)

private:
    gp_Ax22d ax; // The actual OpenCASCADE axis system

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Ax22d", D_METHOD("new"), &ocgd_gp_Ax22d::_new);
        
        // Direction and position access
        ClassDB::bind_method(D_METHOD("SetAxis", "A"), &ocgd_gp_Ax22d::SetAxis);
        ClassDB::bind_method(D_METHOD("SetXAxis", "A"), &ocgd_gp_Ax22d::SetXAxis);
        ClassDB::bind_method(D_METHOD("SetYAxis", "A"), &ocgd_gp_Ax22d::SetYAxis);
        ClassDB::bind_method(D_METHOD("SetLocation", "P"), &ocgd_gp_Ax22d::SetLocation);
        ClassDB::bind_method(D_METHOD("SetXDirection", "V"), &ocgd_gp_Ax22d::SetXDirection);
        ClassDB::bind_method(D_METHOD("SetYDirection", "V"), &ocgd_gp_Ax22d::SetYDirection);

        ClassDB::bind_method(D_METHOD("Axis"), &ocgd_gp_Ax22d::Axis);
        ClassDB::bind_method(D_METHOD("XAxis"), &ocgd_gp_Ax22d::XAxis);
        ClassDB::bind_method(D_METHOD("YAxis"), &ocgd_gp_Ax22d::YAxis);
        ClassDB::bind_method(D_METHOD("Location"), &ocgd_gp_Ax22d::Location);
        ClassDB::bind_method(D_METHOD("XDirection"), &ocgd_gp_Ax22d::XDirection);
        ClassDB::bind_method(D_METHOD("YDirection"), &ocgd_gp_Ax22d::YDirection);

        // Axis system operations
        
        // Transformations
        ClassDB::bind_method(D_METHOD("Mirror", "P"), &ocgd_gp_Ax22d::Mirror_Point);
        ClassDB::bind_method(D_METHOD("Mirrored", "P"), &ocgd_gp_Ax22d::Mirrored_Point);
        ClassDB::bind_method(D_METHOD("MirrorAxis", "A"), &ocgd_gp_Ax22d::Mirror_Axis);
        ClassDB::bind_method(D_METHOD("MirroredAxis", "A"), &ocgd_gp_Ax22d::Mirrored_Axis);
        ClassDB::bind_method(D_METHOD("Rotate", "P", "Ang"), &ocgd_gp_Ax22d::Rotate);
        ClassDB::bind_method(D_METHOD("Rotated", "P", "Ang"), &ocgd_gp_Ax22d::Rotated);
        ClassDB::bind_method(D_METHOD("Scale", "P", "S"), &ocgd_gp_Ax22d::Scale);
        ClassDB::bind_method(D_METHOD("Scaled", "P", "S"), &ocgd_gp_Ax22d::Scaled);
        ClassDB::bind_method(D_METHOD("Translate", "V"), &ocgd_gp_Ax22d::Translate_Vec);
        ClassDB::bind_method(D_METHOD("Translated", "V"), &ocgd_gp_Ax22d::Translated_Vec);
        ClassDB::bind_method(D_METHOD("TranslatePoints", "P1", "P2"), &ocgd_gp_Ax22d::Translate_Points);
        ClassDB::bind_method(D_METHOD("TranslatedPoints", "P1", "P2"), &ocgd_gp_Ax22d::Translated_Points);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "location", PROPERTY_HINT_NONE, "ocgd_gp_Pnt2d"), "SetLocation", "Location");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "x_direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir2d"), "SetXDirection", "XDirection");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "y_direction", PROPERTY_HINT_NONE, "ocgd_gp_Dir2d"), "SetYDirection", "YDirection");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Ax22d> _new() { return memnew(ocgd_gp_Ax22d); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Ax22d() : ax() {} // Creates an axis system with origin (0,0) and axes +X, +Y
    
    // Creates an axis system from another axis
    ocgd_gp_Ax22d(const Ref<ocgd_gp_Ax2d>& A) : ax(A->get_ax()) {}
    
    // Creates an axis system with origin P and X direction V
    ocgd_gp_Ax22d(const Ref<ocgd_gp_Pnt2d>& P, const Ref<ocgd_gp_Dir2d>& V) : ax(P->get_pnt(), V->get_dir()) {}
    
    // Creates an axis system with origin P, X direction Vx and Y direction Vy
    ocgd_gp_Ax22d(const Ref<ocgd_gp_Pnt2d>& P, const Ref<ocgd_gp_Dir2d>& Vx, const Ref<ocgd_gp_Dir2d>& Vy) 
        : ax(P->get_pnt(), Vx->get_dir(), Vy->get_dir()) {}
    
    // Direction and position access methods
    void SetAxis(const Ref<ocgd_gp_Ax22d>& A) { ax.SetAxis(A->get_ax()); }
    void SetXAxis(const Ref<ocgd_gp_Ax2d>& A) { ax.SetXAxis(A->get_ax()); }
    void SetYAxis(const Ref<ocgd_gp_Ax2d>& A) { ax.SetYAxis(A->get_ax()); }
    void SetLocation(const Ref<ocgd_gp_Pnt2d>& P) { ax.SetLocation(P->get_pnt()); }
    void SetXDirection(const Ref<ocgd_gp_Dir2d>& V) { ax.SetXDirection(V->get_dir()); }
    void SetYDirection(const Ref<ocgd_gp_Dir2d>& V) { ax.SetYDirection(V->get_dir()); }
    
    Ref<ocgd_gp_Ax22d> Axis() const {
        Ref<ocgd_gp_Ax22d> result;
        result.instantiate();
        result->get_ax() = ax;
        return result;
    }
    
    Ref<ocgd_gp_Ax2d> XAxis() const {
        Ref<ocgd_gp_Ax2d> result;
        result.instantiate();
        result->get_ax() = ax.XAxis();
        return result;
    }
    
    Ref<ocgd_gp_Ax2d> YAxis() const {
        Ref<ocgd_gp_Ax2d> result;
        result.instantiate();
        result->get_ax() = ax.YAxis();
        return result;
    }
    
    Ref<ocgd_gp_Pnt2d> Location() const {
        Ref<ocgd_gp_Pnt2d> result;
        result.instantiate();
        result->get_pnt() = ax.Location();
        return result;
    }
    
    Ref<ocgd_gp_Dir2d> XDirection() const {
        Ref<ocgd_gp_Dir2d> result;
        result.instantiate();
        result->get_dir() = ax.XDirection();
        return result;
    }
    
    Ref<ocgd_gp_Dir2d> YDirection() const {
        Ref<ocgd_gp_Dir2d> result;
        result.instantiate();
        result->get_dir() = ax.YDirection();
        return result;
    }
    
    // Axis system operations
    
    // Transformations
    void Mirror_Point(const Ref<ocgd_gp_Pnt2d>& P) { ax.Mirror(P->get_pnt()); }
    Ref<ocgd_gp_Ax22d> Mirrored_Point(const Ref<ocgd_gp_Pnt2d>& P) const {
        Ref<ocgd_gp_Ax22d> result;
        result.instantiate();
        result->get_ax() = ax.Mirrored(P->get_pnt());
        return result;
    }
    
    void Mirror_Axis(const Ref<ocgd_gp_Ax2d>& A) { ax.Mirror(A->get_ax()); }
    Ref<ocgd_gp_Ax22d> Mirrored_Axis(const Ref<ocgd_gp_Ax2d>& A) const {
        Ref<ocgd_gp_Ax22d> result;
        result.instantiate();
        result->get_ax() = ax.Mirrored(A->get_ax());
        return result;
    }
    
    void Rotate(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real Ang) { ax.Rotate(P->get_pnt(), Ang); }
    Ref<ocgd_gp_Ax22d> Rotated(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real Ang) const {
        Ref<ocgd_gp_Ax22d> result;
        result.instantiate();
        result->get_ax() = ax.Rotated(P->get_pnt(), Ang);
        return result;
    }
    
    void Scale(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real S) { ax.Scale(P->get_pnt(), S); }
    Ref<ocgd_gp_Ax22d> Scaled(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real S) const {
        Ref<ocgd_gp_Ax22d> result;
        result.instantiate();
        result->get_ax() = ax.Scaled(P->get_pnt(), S);
        return result;
    }
    
    void Translate_Vec(const Ref<ocgd_gp_Vec2d>& V) { ax.Translate(V->get_vec()); }
    Ref<ocgd_gp_Ax22d> Translated_Vec(const Ref<ocgd_gp_Vec2d>& V) const {
        Ref<ocgd_gp_Ax22d> result;
        result.instantiate();
        result->get_ax() = ax.Translated(V->get_vec());
        return result;
    }
    
    void Translate_Points(const Ref<ocgd_gp_Pnt2d>& P1, const Ref<ocgd_gp_Pnt2d>& P2) {
        ax.Translate(P1->get_pnt(), P2->get_pnt());
    }
    Ref<ocgd_gp_Ax22d> Translated_Points(const Ref<ocgd_gp_Pnt2d>& P1, const Ref<ocgd_gp_Pnt2d>& P2) const {
        Ref<ocgd_gp_Ax22d> result;
        result.instantiate();
        result->get_ax() = ax.Translated(P1->get_pnt(), P2->get_pnt());
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Ax22d& get_ax() const { return ax; }
    gp_Ax22d& get_ax() { return ax; }
};

#endif // _ocgd_gp_Ax22d_h