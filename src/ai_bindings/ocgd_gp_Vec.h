#ifndef _ocgd_gp_Vec_h
#define _ocgd_gp_Vec_h

/**
 * ocgd_gp_Vec.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Vec as a Godot class.
 * 
 * A gp_Vec represents a vector in 3D space. Unlike gp_Dir, it has magnitude.
 * Used to represent geometric vectors, translations, and other vector operations.
 * 
 * Original OCCT header: <opencascade/gp_Vec.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Vec.hxx>

#include "ocgd_gp_XYZ.h"
#include "ocgd_gp_Dir.h"
#include "ocgd_gp_Pnt.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Vec
 *
 * Wrapper class to expose the OpenCASCADE gp_Vec class to Godot.
 * 
 * Defines a geometric vector in 3D space.
 * 
 * The class maps closely to Godot's Vector3 but maintains the OpenCASCADE API
 * for consistency with the rest of the OpenCASCADE bindings.
 */
class ocgd_gp_Vec : public RefCounted {
    GDCLASS(ocgd_gp_Vec, RefCounted)

private:
    gp_Vec vec; // The actual OpenCASCADE vector

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Vec", D_METHOD("new"), &ocgd_gp_Vec::_new);

        // Conversion methods
        ClassDB::bind_method(D_METHOD("to_vector3"), &ocgd_gp_Vec::to_vector3);
        ClassDB::bind_method(D_METHOD("GetDir"), &ocgd_gp_Vec::GetDir);
        
        // Coordinate access
        ClassDB::bind_method(D_METHOD("SetCoord", "Index", "Xi"), static_cast<void (ocgd_gp_Vec::*)(const Standard_Integer, const Standard_Real)>(&ocgd_gp_Vec::SetCoord));
        ClassDB::bind_method(D_METHOD("SetCoordXYZ", "Xv", "Yv", "Zv"), static_cast<void (ocgd_gp_Vec::*)(const Standard_Real, const Standard_Real, const Standard_Real)>(&ocgd_gp_Vec::SetCoord));
        ClassDB::bind_method(D_METHOD("SetX", "X"), &ocgd_gp_Vec::SetX);
        ClassDB::bind_method(D_METHOD("SetY", "Y"), &ocgd_gp_Vec::SetY);
        ClassDB::bind_method(D_METHOD("SetZ", "Z"), &ocgd_gp_Vec::SetZ);
        ClassDB::bind_method(D_METHOD("SetXYZ", "Coord"), &ocgd_gp_Vec::SetXYZ);
        ClassDB::bind_method(D_METHOD("Coord", "Index"), static_cast<Standard_Real (ocgd_gp_Vec::*)(const Standard_Integer) const>(&ocgd_gp_Vec::Coord));
        ClassDB::bind_method(D_METHOD("X"), &ocgd_gp_Vec::X);
        ClassDB::bind_method(D_METHOD("Y"), &ocgd_gp_Vec::Y);
        ClassDB::bind_method(D_METHOD("Z"), &ocgd_gp_Vec::Z);
        ClassDB::bind_method(D_METHOD("XYZ"), &ocgd_gp_Vec::XYZ);
        
        // Vector comparison operations
        ClassDB::bind_method(D_METHOD("IsEqual", "Other", "LinearTolerance", "AngularTolerance"), &ocgd_gp_Vec::IsEqual);
        ClassDB::bind_method(D_METHOD("IsNormal", "Other", "AngularTolerance"), &ocgd_gp_Vec::IsNormal);
        ClassDB::bind_method(D_METHOD("IsOpposite", "Other", "AngularTolerance"), &ocgd_gp_Vec::IsOpposite);
        ClassDB::bind_method(D_METHOD("IsParallel", "Other", "AngularTolerance"), &ocgd_gp_Vec::IsParallel);
        ClassDB::bind_method(D_METHOD("Angle", "Other"), &ocgd_gp_Vec::Angle);
        ClassDB::bind_method(D_METHOD("AngleWithRef", "Other", "VRef"), &ocgd_gp_Vec::AngleWithRef);
        
        // Magnitude operations
        ClassDB::bind_method(D_METHOD("Magnitude"), &ocgd_gp_Vec::Magnitude);
        ClassDB::bind_method(D_METHOD("SquareMagnitude"), &ocgd_gp_Vec::SquareMagnitude);
        
        // Vector arithmetic
        ClassDB::bind_method(D_METHOD("Add", "Other"), &ocgd_gp_Vec::Add);
        ClassDB::bind_method(D_METHOD("Added", "Other"), &ocgd_gp_Vec::Added);
        ClassDB::bind_method(D_METHOD("Subtract", "Right"), &ocgd_gp_Vec::Subtract);
        ClassDB::bind_method(D_METHOD("Subtracted", "Right"), &ocgd_gp_Vec::Subtracted);
        ClassDB::bind_method(D_METHOD("Multiply", "Scalar"), &ocgd_gp_Vec::Multiply);
        ClassDB::bind_method(D_METHOD("Multiplied", "Scalar"), &ocgd_gp_Vec::Multiplied);
        ClassDB::bind_method(D_METHOD("Divide", "Scalar"), &ocgd_gp_Vec::Divide);
        ClassDB::bind_method(D_METHOD("Divided", "Scalar"), &ocgd_gp_Vec::Divided);
        
        // Vector products
        ClassDB::bind_method(D_METHOD("Cross", "Right"), &ocgd_gp_Vec::Cross);
        ClassDB::bind_method(D_METHOD("Crossed", "Right"), &ocgd_gp_Vec::Crossed);
        ClassDB::bind_method(D_METHOD("CrossMagnitude", "Right"), &ocgd_gp_Vec::CrossMagnitude);
        ClassDB::bind_method(D_METHOD("CrossSquareMagnitude", "Right"), &ocgd_gp_Vec::CrossSquareMagnitude);
        ClassDB::bind_method(D_METHOD("CrossCross", "V1", "V2"), &ocgd_gp_Vec::CrossCross);
        ClassDB::bind_method(D_METHOD("CrossCrossed", "V1", "V2"), &ocgd_gp_Vec::CrossCrossed);
        ClassDB::bind_method(D_METHOD("Dot", "Other"), &ocgd_gp_Vec::Dot);
        ClassDB::bind_method(D_METHOD("DotCross", "V1", "V2"), &ocgd_gp_Vec::DotCross);
        
        // Vector operations
        ClassDB::bind_method(D_METHOD("Normalize"), &ocgd_gp_Vec::Normalize);
        ClassDB::bind_method(D_METHOD("Normalized"), &ocgd_gp_Vec::Normalized);
        ClassDB::bind_method(D_METHOD("Reverse"), &ocgd_gp_Vec::Reverse);
        ClassDB::bind_method(D_METHOD("Reversed"), &ocgd_gp_Vec::Reversed);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "X"), "SetX", "X");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Y"), "SetY", "Y");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Z"), "SetZ", "Z");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Vec> _new() { return memnew(ocgd_gp_Vec); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Vec() : vec() {} // Creates a vector with zero coordinates
    ocgd_gp_Vec(const Standard_Real Xv, const Standard_Real Yv, const Standard_Real Zv) : vec(Xv, Yv, Zv) {} // Creates vector from coordinates
    ocgd_gp_Vec(const Ref<ocgd_gp_Pnt>& P1, const Ref<ocgd_gp_Pnt>& P2) : vec(P1->get_pnt(), P2->get_pnt()) {} // Creates vector from two points
    
    // Convert from/to Godot Vector3 (additional convenience methods)
    ocgd_gp_Vec(const Vector3 &v) : vec(v.x, v.y, v.z) {}
    Vector3 to_vector3() const { return Vector3(vec.X(), vec.Y(), vec.Z()); }
    
    // Access methods - matching original API names
    void SetCoord(const Standard_Integer Index, const Standard_Real Xi) { vec.SetCoord(Index, Xi); }
    void SetCoord(const Standard_Real Xv, const Standard_Real Yv, const Standard_Real Zv) { vec.SetCoord(Xv, Yv, Zv); }
    void SetX(const Standard_Real X) { vec.SetX(X); }
    void SetY(const Standard_Real Y) { vec.SetY(Y); }
    void SetZ(const Standard_Real Z) { vec.SetZ(Z); }
    void SetXYZ(const Ref<ocgd_gp_XYZ>& Coord) { vec.SetXYZ(Coord->get_xyz()); }
    Standard_Real Coord(const Standard_Integer Index) const { return vec.Coord(Index); }
    void Coord(Standard_Real& Xv, Standard_Real& Yv, Standard_Real& Zv) const { vec.Coord(Xv, Yv, Zv); }
    Standard_Real X() const { return vec.X(); }
    Standard_Real Y() const { return vec.Y(); }
    Standard_Real Z() const { return vec.Z(); }
    Ref<ocgd_gp_XYZ> XYZ() const { 
        Ref<ocgd_gp_XYZ> xyz;
        xyz.instantiate();
        xyz->get_xyz() = vec.XYZ();
        return xyz;
    }
    
    // Vector operations - matching original API names
    Standard_Boolean IsEqual(const Ref<ocgd_gp_Vec>& Other, const Standard_Real LinearTolerance, const Standard_Real AngularTolerance) const {
        return vec.IsEqual(Other->get_vec(), LinearTolerance, AngularTolerance);
    }
    Standard_Boolean IsNormal(const Ref<ocgd_gp_Vec>& Other, const Standard_Real AngularTolerance) const {
        return vec.IsNormal(Other->get_vec(), AngularTolerance);
    }
    Standard_Boolean IsOpposite(const Ref<ocgd_gp_Vec>& Other, const Standard_Real AngularTolerance) const {
        return vec.IsOpposite(Other->get_vec(), AngularTolerance);
    }
    Standard_Boolean IsParallel(const Ref<ocgd_gp_Vec>& Other, const Standard_Real AngularTolerance) const {
        return vec.IsParallel(Other->get_vec(), AngularTolerance);
    }
    Standard_Real Angle(const Ref<ocgd_gp_Vec>& Other) const {
        return vec.Angle(Other->get_vec());
    }
    Standard_Real AngleWithRef(const Ref<ocgd_gp_Vec>& Other, const Ref<ocgd_gp_Vec>& VRef) const {
        return vec.AngleWithRef(Other->get_vec(), VRef->get_vec());
    }
    
    Standard_Real Magnitude() const { return vec.Magnitude(); }
    Standard_Real SquareMagnitude() const { return vec.SquareMagnitude(); }
    
    void Add(const Ref<ocgd_gp_Vec>& Other) { vec.Add(Other->get_vec()); }
    Ref<ocgd_gp_Vec> Added(const Ref<ocgd_gp_Vec>& Other) const {
        Ref<ocgd_gp_Vec> result;
        result.instantiate();
        result->get_vec() = vec.Added(Other->get_vec());
        return result;
    }
    
    void Subtract(const Ref<ocgd_gp_Vec>& Right) { vec.Subtract(Right->get_vec()); }
    Ref<ocgd_gp_Vec> Subtracted(const Ref<ocgd_gp_Vec>& Right) const {
        Ref<ocgd_gp_Vec> result;
        result.instantiate();
        result->get_vec() = vec.Subtracted(Right->get_vec());
        return result;
    }
    
    void Multiply(const Standard_Real Scalar) { vec.Multiply(Scalar); }
    Ref<ocgd_gp_Vec> Multiplied(const Standard_Real Scalar) const {
        Ref<ocgd_gp_Vec> result;
        result.instantiate();
        result->get_vec() = vec.Multiplied(Scalar);
        return result;
    }
    
    void Divide(const Standard_Real Scalar) { vec.Divide(Scalar); }
    Ref<ocgd_gp_Vec> Divided(const Standard_Real Scalar) const {
        Ref<ocgd_gp_Vec> result;
        result.instantiate();
        result->get_vec() = vec.Divided(Scalar);
        return result;
    }
    
    void Cross(const Ref<ocgd_gp_Vec>& Right) { vec.Cross(Right->get_vec()); }
    Ref<ocgd_gp_Vec> Crossed(const Ref<ocgd_gp_Vec>& Right) const {
        Ref<ocgd_gp_Vec> result;
        result.instantiate();
        result->get_vec() = vec.Crossed(Right->get_vec());
        return result;
    }
    
    Standard_Real CrossMagnitude(const Ref<ocgd_gp_Vec>& Right) const {
        return vec.CrossMagnitude(Right->get_vec());
    }
    Standard_Real CrossSquareMagnitude(const Ref<ocgd_gp_Vec>& Right) const {
        return vec.CrossSquareMagnitude(Right->get_vec());
    }
    
    void CrossCross(const Ref<ocgd_gp_Vec>& V1, const Ref<ocgd_gp_Vec>& V2) {
        vec.CrossCross(V1->get_vec(), V2->get_vec());
    }
    Ref<ocgd_gp_Vec> CrossCrossed(const Ref<ocgd_gp_Vec>& V1, const Ref<ocgd_gp_Vec>& V2) const {
        Ref<ocgd_gp_Vec> result;
        result.instantiate();
        result->get_vec() = vec.CrossCrossed(V1->get_vec(), V2->get_vec());
        return result;
    }
    
    Standard_Real Dot(const Ref<ocgd_gp_Vec>& Other) const {
        return vec.Dot(Other->get_vec());
    }
    
    Standard_Real DotCross(const Ref<ocgd_gp_Vec>& V1, const Ref<ocgd_gp_Vec>& V2) const {
        return vec.DotCross(V1->get_vec(), V2->get_vec());
    }
    
    void Normalize() { vec.Normalize(); }
    Ref<ocgd_gp_Vec> Normalized() const {
        Ref<ocgd_gp_Vec> result;
        result.instantiate();
        result->get_vec() = vec.Normalized();
        return result;
    }
    
    void Reverse() { vec.Reverse(); }
    Ref<ocgd_gp_Vec> Reversed() const {
        Ref<ocgd_gp_Vec> result;
        result.instantiate();
        result->get_vec() = vec.Reversed();
        return result;
    }
    
    // Conversion methods
    Ref<ocgd_gp_Dir> GetDir() const {
        Ref<ocgd_gp_Dir> result;
        result.instantiate();
        result->get_dir() = gp_Dir(vec);
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Vec& get_vec() const { return vec; }
    gp_Vec& get_vec() { return vec; }
};

#endif // _ocgd_gp_Vec_h