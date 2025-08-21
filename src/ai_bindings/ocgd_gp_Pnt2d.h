#ifndef _ocgd_gp_Pnt2d_h
#define _ocgd_gp_Pnt2d_h

/**
 * ocgd_gp_Pnt2d.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Pnt2d as a Godot class.
 * 
 * This class represents a cartesian point in 2D space.
 * Used for geometric computations.
 * 
 * Original OCCT header: <opencascade/gp_Pnt2d.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Pnt2d.hxx>

#include "ocgd_gp_XY.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Pnt2d
 *
 * Wrapper class to expose the OpenCASCADE gp_Pnt2d class to Godot.
 * 
 * Defines a non-persistent 2D cartesian point.
 * 
 * The class maps closely to Godot's Vector2 but maintains the OpenCASCADE API
 * for consistency with the rest of the OpenCASCADE bindings.
 */
class ocgd_gp_Pnt2d : public RefCounted {
    GDCLASS(ocgd_gp_Pnt2d, RefCounted)

private:
    gp_Pnt2d pnt; // The actual OpenCASCADE point

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Pnt2d", D_METHOD("new"), &ocgd_gp_Pnt2d::_new);

        // Conversion methods
        ClassDB::bind_method(D_METHOD("to_vector2"), &ocgd_gp_Pnt2d::to_vector2);
        
        // Coordinate access
        ClassDB::bind_method(D_METHOD("SetCoord", "Index", "Xi"), static_cast<void (ocgd_gp_Pnt2d::*)(const Standard_Integer, const Standard_Real)>(&ocgd_gp_Pnt2d::SetCoord));
        ClassDB::bind_method(D_METHOD("SetCoordXY", "Xp", "Yp"), static_cast<void (ocgd_gp_Pnt2d::*)(const Standard_Real, const Standard_Real)>(&ocgd_gp_Pnt2d::SetCoord));
        ClassDB::bind_method(D_METHOD("SetX", "X"), &ocgd_gp_Pnt2d::SetX);
        ClassDB::bind_method(D_METHOD("SetY", "Y"), &ocgd_gp_Pnt2d::SetY);
        ClassDB::bind_method(D_METHOD("SetXY", "Coord"), &ocgd_gp_Pnt2d::SetXY);
        ClassDB::bind_method(D_METHOD("Coord", "Index"), static_cast<Standard_Real (ocgd_gp_Pnt2d::*)(const Standard_Integer) const>(&ocgd_gp_Pnt2d::Coord));
        ClassDB::bind_method(D_METHOD("X"), &ocgd_gp_Pnt2d::X);
        ClassDB::bind_method(D_METHOD("Y"), &ocgd_gp_Pnt2d::Y);
        ClassDB::bind_method(D_METHOD("XY"), &ocgd_gp_Pnt2d::XY);
        
        // Point operations
        ClassDB::bind_method(D_METHOD("IsEqual", "Other", "LinearTolerance"), &ocgd_gp_Pnt2d::IsEqual);
        ClassDB::bind_method(D_METHOD("Distance", "Other"), &ocgd_gp_Pnt2d::Distance);
        ClassDB::bind_method(D_METHOD("SquareDistance", "Other"), &ocgd_gp_Pnt2d::SquareDistance);
        
        ClassDB::bind_method(D_METHOD("Mirror", "P"), &ocgd_gp_Pnt2d::Mirror);
        ClassDB::bind_method(D_METHOD("Mirrored", "P"), &ocgd_gp_Pnt2d::Mirrored);
        
        ClassDB::bind_method(D_METHOD("Rotate", "P", "Ang"), &ocgd_gp_Pnt2d::Rotate);
        ClassDB::bind_method(D_METHOD("Rotated", "P", "Ang"), &ocgd_gp_Pnt2d::Rotated);
        
        ClassDB::bind_method(D_METHOD("Scale", "P", "S"), &ocgd_gp_Pnt2d::Scale);
        ClassDB::bind_method(D_METHOD("Scaled", "P", "S"), &ocgd_gp_Pnt2d::Scaled);
        
        ClassDB::bind_method(D_METHOD("Translate", "P1", "P2"), &ocgd_gp_Pnt2d::Translate);
        ClassDB::bind_method(D_METHOD("Translated", "P1", "P2"), &ocgd_gp_Pnt2d::Translated);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "X"), "SetX", "X");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Y"), "SetY", "Y");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Pnt2d> _new() { return memnew(ocgd_gp_Pnt2d); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Pnt2d() : pnt() {} // Creates a point with zero coordinates
    ocgd_gp_Pnt2d(const Standard_Real Xp, const Standard_Real Yp) : pnt(Xp, Yp) {} // Creates point from coordinates
    
    // Convert from/to Godot Vector2 (additional convenience methods)
    ocgd_gp_Pnt2d(const Vector2 &v) : pnt(v.x, v.y) {}
    Vector2 to_vector2() const { return Vector2(pnt.X(), pnt.Y()); }
    
    // Access methods - matching original API names
    void SetCoord(const Standard_Integer Index, const Standard_Real Xi) { pnt.SetCoord(Index, Xi); }
    void SetCoord(const Standard_Real Xp, const Standard_Real Yp) { pnt.SetCoord(Xp, Yp); }
    void SetX(const Standard_Real X) { pnt.SetX(X); }
    void SetY(const Standard_Real Y) { pnt.SetY(Y); }
    void SetXY(const Ref<ocgd_gp_XY>& Coord) { pnt.SetXY(Coord->get_xy()); }
    Standard_Real Coord(const Standard_Integer Index) const { return pnt.Coord(Index); }
    void Coord(Standard_Real& Xp, Standard_Real& Yp) const { pnt.Coord(Xp, Yp); }
    Standard_Real X() const { return pnt.X(); }
    Standard_Real Y() const { return pnt.Y(); }
    Ref<ocgd_gp_XY> XY() const { 
        Ref<ocgd_gp_XY> xy;
        xy.instantiate();
        xy->get_xy() = pnt.XY();
        return xy;
    }
    
    // Point operations - matching original API names
    Standard_Boolean IsEqual(const Ref<ocgd_gp_Pnt2d>& Other, const Standard_Real LinearTolerance) const {
        return pnt.IsEqual(Other->get_pnt(), LinearTolerance);
    }
    Standard_Real Distance(const Ref<ocgd_gp_Pnt2d>& Other) const {
        return pnt.Distance(Other->get_pnt());
    }
    Standard_Real SquareDistance(const Ref<ocgd_gp_Pnt2d>& Other) const {
        return pnt.SquareDistance(Other->get_pnt());
    }
    
    void Mirror(const Ref<ocgd_gp_Pnt2d>& P) { pnt.Mirror(P->get_pnt()); }
    Ref<ocgd_gp_Pnt2d> Mirrored(const Ref<ocgd_gp_Pnt2d>& P) const {
        Ref<ocgd_gp_Pnt2d> result;
        result.instantiate();
        result->get_pnt() = pnt.Mirrored(P->get_pnt());
        return result;
    }
    
    void Rotate(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real Ang) {
        pnt.Rotate(P->get_pnt(), Ang);
    }
    Ref<ocgd_gp_Pnt2d> Rotated(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real Ang) const {
        Ref<ocgd_gp_Pnt2d> result;
        result.instantiate();
        result->get_pnt() = pnt.Rotated(P->get_pnt(), Ang);
        return result;
    }
    
    void Scale(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real S) {
        pnt.Scale(P->get_pnt(), S);
    }
    Ref<ocgd_gp_Pnt2d> Scaled(const Ref<ocgd_gp_Pnt2d>& P, const Standard_Real S) const {
        Ref<ocgd_gp_Pnt2d> result;
        result.instantiate();
        result->get_pnt() = pnt.Scaled(P->get_pnt(), S);
        return result;
    }
    
    void Translate(const Ref<ocgd_gp_Pnt2d>& P1, const Ref<ocgd_gp_Pnt2d>& P2) {
        pnt.Translate(P1->get_pnt(), P2->get_pnt());
    }
    Ref<ocgd_gp_Pnt2d> Translated(const Ref<ocgd_gp_Pnt2d>& P1, const Ref<ocgd_gp_Pnt2d>& P2) const {
        Ref<ocgd_gp_Pnt2d> result;
        result.instantiate();
        result->get_pnt() = pnt.Translated(P1->get_pnt(), P2->get_pnt());
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Pnt2d& get_pnt() const { return pnt; }
    gp_Pnt2d& get_pnt() { return pnt; }
};

#endif // _ocgd_gp_Pnt2d_h