#ifndef _ocgd_gp_Pnt_h
#define _ocgd_gp_Pnt_h

/**
 * ocgd_gp_Pnt.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Pnt as a Godot class.
 * 
 * This class represents a cartesian point in 3D space.
 * Used for geometric computations.
 * 
 * Original OCCT header: <opencascade/gp_Pnt.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Pnt.hxx>
#include <opencascade/gp_Ax1.hxx>
#include <opencascade/gp_Dir.hxx>

#include "ocgd_gp_XYZ.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Pnt
 *
 * Wrapper class to expose the OpenCASCADE gp_Pnt class to Godot.
 * 
 * Defines a 3D cartesian point.
 * 
 * The class maps closely to Godot's Vector3 but maintains the OpenCASCADE API
 * for consistency with the rest of the OpenCASCADE bindings.
 */
class ocgd_gp_Pnt : public RefCounted {
    GDCLASS(ocgd_gp_Pnt, RefCounted)

private:
    gp_Pnt pnt; // The actual OpenCASCADE point

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Pnt", D_METHOD("new"), &ocgd_gp_Pnt::_new);

        // Conversion methods
        ClassDB::bind_method(D_METHOD("to_vector3"), &ocgd_gp_Pnt::to_vector3);
        
        // Coordinate access
        ClassDB::bind_method(D_METHOD("SetCoord", "Index", "Xi"), static_cast<void (ocgd_gp_Pnt::*)(const Standard_Integer, const Standard_Real)>(&ocgd_gp_Pnt::SetCoord));
        ClassDB::bind_method(D_METHOD("SetCoordXYZ", "Xp", "Yp", "Zp"), static_cast<void (ocgd_gp_Pnt::*)(const Standard_Real, const Standard_Real, const Standard_Real)>(&ocgd_gp_Pnt::SetCoord));
        ClassDB::bind_method(D_METHOD("SetX", "X"), &ocgd_gp_Pnt::SetX);
        ClassDB::bind_method(D_METHOD("SetY", "Y"), &ocgd_gp_Pnt::SetY);
        ClassDB::bind_method(D_METHOD("SetZ", "Z"), &ocgd_gp_Pnt::SetZ);
        ClassDB::bind_method(D_METHOD("SetXYZ", "Coord"), &ocgd_gp_Pnt::SetXYZ);
        ClassDB::bind_method(D_METHOD("Coord", "Index"), static_cast<Standard_Real (ocgd_gp_Pnt::*)(const Standard_Integer) const>(&ocgd_gp_Pnt::Coord));
        ClassDB::bind_method(D_METHOD("X"), &ocgd_gp_Pnt::X);
        ClassDB::bind_method(D_METHOD("Y"), &ocgd_gp_Pnt::Y);
        ClassDB::bind_method(D_METHOD("Z"), &ocgd_gp_Pnt::Z);
        ClassDB::bind_method(D_METHOD("XYZ"), &ocgd_gp_Pnt::XYZ);
        
        // Point operations
        ClassDB::bind_method(D_METHOD("BaryCenter", "Alpha", "P", "Beta"), &ocgd_gp_Pnt::BaryCenter);
        ClassDB::bind_method(D_METHOD("IsEqual", "Other", "LinearTolerance"), &ocgd_gp_Pnt::IsEqual);
        ClassDB::bind_method(D_METHOD("Distance", "Other"), &ocgd_gp_Pnt::Distance);
        ClassDB::bind_method(D_METHOD("SquareDistance", "Other"), &ocgd_gp_Pnt::SquareDistance);
        
        ClassDB::bind_method(D_METHOD("Mirror", "P"), &ocgd_gp_Pnt::Mirror);
        ClassDB::bind_method(D_METHOD("Mirrored", "P"), &ocgd_gp_Pnt::Mirrored);
        
        ClassDB::bind_method(D_METHOD("Rotate", "P", "Ang"), &ocgd_gp_Pnt::Rotate);
        ClassDB::bind_method(D_METHOD("Rotated", "P", "Ang"), &ocgd_gp_Pnt::Rotated);
        
        ClassDB::bind_method(D_METHOD("Scale", "P", "S"), &ocgd_gp_Pnt::Scale);
        ClassDB::bind_method(D_METHOD("Scaled", "P", "S"), &ocgd_gp_Pnt::Scaled);
        
        ClassDB::bind_method(D_METHOD("Translate", "P1", "P2"), &ocgd_gp_Pnt::Translate);
        ClassDB::bind_method(D_METHOD("Translated", "P1", "P2"), &ocgd_gp_Pnt::Translated);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "X"), "SetX", "X");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Y"), "SetY", "Y");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Z"), "SetZ", "Z");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Pnt> _new() { return memnew(ocgd_gp_Pnt); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Pnt() : pnt() {} // Creates a point with zero coordinates
    ocgd_gp_Pnt(const Standard_Real Xp, const Standard_Real Yp, const Standard_Real Zp) : pnt(Xp, Yp, Zp) {} // Creates point from coordinates
    
    // Convert from/to Godot Vector3 (additional convenience methods)
    ocgd_gp_Pnt(const Vector3 &v) : pnt(v.x, v.y, v.z) {}
    Vector3 to_vector3() const { return Vector3(pnt.X(), pnt.Y(), pnt.Z()); }
    
    // Access methods - matching original API names
    void SetCoord(const Standard_Integer Index, const Standard_Real Xi) { pnt.SetCoord(Index, Xi); }
    void SetCoord(const Standard_Real Xp, const Standard_Real Yp, const Standard_Real Zp) { pnt.SetCoord(Xp, Yp, Zp); }
    void SetX(const Standard_Real X) { pnt.SetX(X); }
    void SetY(const Standard_Real Y) { pnt.SetY(Y); }
    void SetZ(const Standard_Real Z) { pnt.SetZ(Z); }
    void SetXYZ(const Ref<ocgd_gp_XYZ>& Coord) { pnt.SetXYZ(Coord->get_xyz()); }
    Standard_Real Coord(const Standard_Integer Index) const { return pnt.Coord(Index); }
    void Coord(Standard_Real& Xp, Standard_Real& Yp, Standard_Real& Zp) const { pnt.Coord(Xp, Yp, Zp); }
    Standard_Real X() const { return pnt.X(); }
    Standard_Real Y() const { return pnt.Y(); }
    Standard_Real Z() const { return pnt.Z(); }
    Ref<ocgd_gp_XYZ> XYZ() const { 
        Ref<ocgd_gp_XYZ> xyz;
        xyz.instantiate();
        xyz->get_xyz() = pnt.XYZ();
        return xyz;
    }
    
    // Point operations - matching original API names
    void BaryCenter(const Standard_Real Alpha, const Ref<ocgd_gp_Pnt>& P, const Standard_Real Beta) {
        pnt.BaryCenter(Alpha, P->get_pnt(), Beta);
    }
    
    Standard_Boolean IsEqual(const Ref<ocgd_gp_Pnt>& Other, const Standard_Real LinearTolerance) const {
        return pnt.IsEqual(Other->get_pnt(), LinearTolerance);
    }
    Standard_Real Distance(const Ref<ocgd_gp_Pnt>& Other) const {
        return pnt.Distance(Other->get_pnt());
    }
    Standard_Real SquareDistance(const Ref<ocgd_gp_Pnt>& Other) const {
        return pnt.SquareDistance(Other->get_pnt());
    }
    
    void Mirror(const Ref<ocgd_gp_Pnt>& P) { pnt.Mirror(P->get_pnt()); }
    Ref<ocgd_gp_Pnt> Mirrored(const Ref<ocgd_gp_Pnt>& P) const {
        Ref<ocgd_gp_Pnt> result;
        result.instantiate();
        result->get_pnt() = pnt.Mirrored(P->get_pnt());
        return result;
    }
    
    void Rotate(const Ref<ocgd_gp_Pnt>& P, const Standard_Real Ang) {
        pnt.Rotate(gp_Ax1(P->get_pnt(), gp_Dir(0.0, 0.0, 1.0)), Ang);
    }
    Ref<ocgd_gp_Pnt> Rotated(const Ref<ocgd_gp_Pnt>& P, const Standard_Real Ang) const {
        Ref<ocgd_gp_Pnt> result;
        result.instantiate();
        result->get_pnt() = pnt.Rotated(gp_Ax1(P->get_pnt(), gp_Dir(0.0, 0.0, 1.0)), Ang);
        return result;
    }
    
    void Scale(const Ref<ocgd_gp_Pnt>& P, const Standard_Real S) {
        pnt.Scale(P->get_pnt(), S);
    }
    Ref<ocgd_gp_Pnt> Scaled(const Ref<ocgd_gp_Pnt>& P, const Standard_Real S) const {
        Ref<ocgd_gp_Pnt> result;
        result.instantiate();
        result->get_pnt() = pnt.Scaled(P->get_pnt(), S);
        return result;
    }
    
    void Translate(const Ref<ocgd_gp_Pnt>& P1, const Ref<ocgd_gp_Pnt>& P2) {
        pnt.Translate(P1->get_pnt(), P2->get_pnt());
    }
    Ref<ocgd_gp_Pnt> Translated(const Ref<ocgd_gp_Pnt>& P1, const Ref<ocgd_gp_Pnt>& P2) const {
        Ref<ocgd_gp_Pnt> result;
        result.instantiate();
        result->get_pnt() = pnt.Translated(P1->get_pnt(), P2->get_pnt());
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Pnt& get_pnt() const { return pnt; }
    gp_Pnt& get_pnt() { return pnt; }
};

#endif // _ocgd_gp_Pnt_h