#ifndef _ocgd_gp_XYZ_h
#define _ocgd_gp_XYZ_h

/**
 * ocgd_gp_XYZ.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_XYZ as a Godot class.
 * 
 * This class represents a cartesian coordinate entity in 3D space {X,Y,Z}.
 * Used for algebraic calculation and vectorial computations.
 * 
 * Original OCCT header: <opencascade/gp_XYZ.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_XYZ.hxx>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_XYZ
 *
 * Wrapper class to expose the OpenCASCADE gp_XYZ class to Godot.
 * 
 * This class describes a cartesian coordinate entity in 3D space {X,Y,Z}.
 * Used for vectorial computations or for holding this type of information 
 * in data structures.
 * 
 * The class maps closely to Godot's Vector3 but maintains the OpenCASCADE API
 * for consistency with the rest of the OpenCASCADE bindings.
 */
class ocgd_gp_XYZ : public RefCounted {
    GDCLASS(ocgd_gp_XYZ, RefCounted)

private:
    gp_XYZ xyz; // The actual OpenCASCADE point

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_XYZ", D_METHOD("new"), &ocgd_gp_XYZ::_new);

        // Conversion methods
        ClassDB::bind_method(D_METHOD("to_vector3"), &ocgd_gp_XYZ::to_vector3);
        
        // Coordinate access
        ClassDB::bind_method(D_METHOD("SetCoord", "Index", "Xi"), &ocgd_gp_XYZ::SetCoord_Index);
        ClassDB::bind_method(D_METHOD("SetCoordXYZ", "X", "Y", "Z"), &ocgd_gp_XYZ::SetCoordXYZ);
        ClassDB::bind_method(D_METHOD("SetX", "X"), &ocgd_gp_XYZ::SetX);
        ClassDB::bind_method(D_METHOD("SetY", "Y"), &ocgd_gp_XYZ::SetY);
        ClassDB::bind_method(D_METHOD("SetZ", "Z"), &ocgd_gp_XYZ::SetZ);
        ClassDB::bind_method(D_METHOD("Coord", "Index"), &ocgd_gp_XYZ::Coord_Index);
        ClassDB::bind_method(D_METHOD("X"), &ocgd_gp_XYZ::X);
        ClassDB::bind_method(D_METHOD("Y"), &ocgd_gp_XYZ::Y);
        ClassDB::bind_method(D_METHOD("Z"), &ocgd_gp_XYZ::Z);
        
        // Vector operations
        ClassDB::bind_method(D_METHOD("Add", "Other"), &ocgd_gp_XYZ::Add);
        ClassDB::bind_method(D_METHOD("Added", "Other"), &ocgd_gp_XYZ::Added);
        ClassDB::bind_method(D_METHOD("Cross", "Right"), &ocgd_gp_XYZ::Cross);
        ClassDB::bind_method(D_METHOD("Crossed", "Right"), &ocgd_gp_XYZ::Crossed);
        ClassDB::bind_method(D_METHOD("CrossMagnitude", "Right"), &ocgd_gp_XYZ::CrossMagnitude);
        ClassDB::bind_method(D_METHOD("CrossSquareMagnitude", "Right"), &ocgd_gp_XYZ::CrossSquareMagnitude);
        ClassDB::bind_method(D_METHOD("Divide", "Scalar"), &ocgd_gp_XYZ::Divide);
        ClassDB::bind_method(D_METHOD("Divided", "Scalar"), &ocgd_gp_XYZ::Divided);
        ClassDB::bind_method(D_METHOD("Dot", "Other"), &ocgd_gp_XYZ::Dot);
        ClassDB::bind_method(D_METHOD("DotCross", "V1", "V2"), &ocgd_gp_XYZ::DotCross);
        ClassDB::bind_method(D_METHOD("IsEqual", "Other", "Tolerance"), &ocgd_gp_XYZ::IsEqual);
        ClassDB::bind_method(D_METHOD("Modulus"), &ocgd_gp_XYZ::Modulus);
        ClassDB::bind_method(D_METHOD("Multiplied", "Scalar"), &ocgd_gp_XYZ::Multiplied_Scalar);
        ClassDB::bind_method(D_METHOD("MultipliedXYZ", "Other"), &ocgd_gp_XYZ::Multiplied_XYZ);
        ClassDB::bind_method(D_METHOD("MultiplyScalar", "Scalar"), &ocgd_gp_XYZ::Multiply_Scalar);
        ClassDB::bind_method(D_METHOD("MultiplyXYZ", "Other"), &ocgd_gp_XYZ::Multiply_XYZ);
        ClassDB::bind_method(D_METHOD("Normalize"), &ocgd_gp_XYZ::Normalize);
        ClassDB::bind_method(D_METHOD("Normalized"), &ocgd_gp_XYZ::Normalized);
        ClassDB::bind_method(D_METHOD("Reverse"), &ocgd_gp_XYZ::Reverse);
        ClassDB::bind_method(D_METHOD("Reversed"), &ocgd_gp_XYZ::Reversed);
        ClassDB::bind_method(D_METHOD("SquareModulus"), &ocgd_gp_XYZ::SquareModulus);
        ClassDB::bind_method(D_METHOD("Subtract", "Right"), &ocgd_gp_XYZ::Subtract);
        ClassDB::bind_method(D_METHOD("Subtracted", "Right"), &ocgd_gp_XYZ::Subtracted);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "X"), "SetX", "X");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Y"), "SetY", "Y");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Z"), "SetZ", "Z");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_XYZ> _new() { return memnew(ocgd_gp_XYZ); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_XYZ() : xyz() {} // Creates XYZ with zero coordinates (0,0,0)
    ocgd_gp_XYZ(const Standard_Real X, const Standard_Real Y, const Standard_Real Z) : xyz(X, Y, Z) {} // Creates XYZ with given coordinates
    
    // Convert from/to Godot Vector3 (additional convenience methods)
    ocgd_gp_XYZ(const Vector3 &v) : xyz(v.x, v.y, v.z) {}
    Vector3 to_vector3() const { return Vector3(xyz.X(), xyz.Y(), xyz.Z()); }
    
    // Access methods - matching original API names
    void SetCoord_Index(const Standard_Integer Index, const Standard_Real Xi) { xyz.SetCoord(Index, Xi); }
    void SetCoordXYZ(const Standard_Real X, const Standard_Real Y, const Standard_Real Z) { xyz.SetCoord(X, Y, Z); }
    void SetX(const Standard_Real X) { xyz.SetX(X); }
    void SetY(const Standard_Real Y) { xyz.SetY(Y); }
    void SetZ(const Standard_Real Z) { xyz.SetZ(Z); }
    Standard_Real Coord_Index(const Standard_Integer Index) const { return xyz.Coord(Index); }
    Standard_Real X() const { return xyz.X(); }
    Standard_Real Y() const { return xyz.Y(); }
    Standard_Real Z() const { return xyz.Z(); }
    
    // Mathematical operations - matching original API names
    Standard_Real Modulus() const { return xyz.Modulus(); }
    Standard_Real SquareModulus() const { return xyz.SquareModulus(); }
    Standard_Boolean IsEqual(const Ref<ocgd_gp_XYZ>& Other, const Standard_Real Tolerance) const { return xyz.IsEqual(Other->get_xyz(), Tolerance); }
    
    // Vector operations - matching original API names
    void Add(const Ref<ocgd_gp_XYZ>& Other) { xyz.Add(Other->get_xyz()); }
    Ref<ocgd_gp_XYZ> Added(const Ref<ocgd_gp_XYZ>& Other) const {
        gp_XYZ result = xyz.Added(Other->get_xyz());
        return memnew(ocgd_gp_XYZ(result.X(), result.Y(), result.Z()));
    }

    void Cross(const Ref<ocgd_gp_XYZ>& Right) { xyz.Cross(Right->get_xyz()); }
    Ref<ocgd_gp_XYZ> Crossed(const Ref<ocgd_gp_XYZ>& Right) const {
        gp_XYZ result = xyz.Crossed(Right->get_xyz());
        return memnew(ocgd_gp_XYZ(result.X(), result.Y(), result.Z()));
    }
    
    Standard_Real CrossMagnitude(const Ref<ocgd_gp_XYZ>& Right) const { return xyz.CrossMagnitude(Right->get_xyz()); }
    Standard_Real CrossSquareMagnitude(const Ref<ocgd_gp_XYZ>& Right) const { return xyz.CrossSquareMagnitude(Right->get_xyz()); }
    
    void CrossCross(const Ref<ocgd_gp_XYZ>& Coord1, const Ref<ocgd_gp_XYZ>& Coord2) { xyz.CrossCross(Coord1->get_xyz(), Coord2->get_xyz()); }
    Ref<ocgd_gp_XYZ> CrossCrossed(const Ref<ocgd_gp_XYZ>& Coord1, const Ref<ocgd_gp_XYZ>& Coord2) const {
        gp_XYZ result = xyz.CrossCrossed(Coord1->get_xyz(), Coord2->get_xyz());
        return memnew(ocgd_gp_XYZ(result.X(), result.Y(), result.Z()));
    }
    
    void Divide(const Standard_Real Scalar) { xyz.Divide(Scalar); }
    Ref<ocgd_gp_XYZ> Divided(const Standard_Real Scalar) const {
        gp_XYZ result = xyz.Divided(Scalar);
        return memnew(ocgd_gp_XYZ(result.X(), result.Y(), result.Z()));
    }
    
    Standard_Real Dot(const Ref<ocgd_gp_XYZ>& Other) const { return xyz.Dot(Other->get_xyz()); }
    Standard_Real DotCross(const Ref<ocgd_gp_XYZ>& Coord1, const Ref<ocgd_gp_XYZ>& Coord2) const { return xyz.DotCross(Coord1->get_xyz(), Coord2->get_xyz()); }
    
    void Multiply_Scalar(const Standard_Real Scalar) { xyz.Multiply(Scalar); }
    void Multiply_XYZ(const Ref<ocgd_gp_XYZ>& Other) { xyz.Multiply(Other->get_xyz()); }
    Ref<ocgd_gp_XYZ> Multiplied_Scalar(const Standard_Real Scalar) const {
        gp_XYZ result = xyz.Multiplied(Scalar);
        return memnew(ocgd_gp_XYZ(result.X(), result.Y(), result.Z()));
    }
    Ref<ocgd_gp_XYZ> Multiplied_XYZ(const Ref<ocgd_gp_XYZ>& Other) const {
        gp_XYZ result = xyz.Multiplied(Other->get_xyz());
        return memnew(ocgd_gp_XYZ(result.X(), result.Y(), result.Z()));
    }
    
    void Normalize() { xyz.Normalize(); }
    Ref<ocgd_gp_XYZ> Normalized() const {
        gp_XYZ result = xyz.Normalized();
        return memnew(ocgd_gp_XYZ(result.X(), result.Y(), result.Z()));
    }
    
    void Reverse() { xyz.Reverse(); }
    Ref<ocgd_gp_XYZ> Reversed() const {
        gp_XYZ result = xyz.Reversed();
        return memnew(ocgd_gp_XYZ(result.X(), result.Y(), result.Z()));
    }
    
    void Subtract(const Ref<ocgd_gp_XYZ>& Right) { xyz.Subtract(Right->get_xyz()); }
    Ref<ocgd_gp_XYZ> Subtracted(const Ref<ocgd_gp_XYZ>& Right) const {
        gp_XYZ result = xyz.Subtracted(Right->get_xyz());
        return memnew(ocgd_gp_XYZ(result.X(), result.Y(), result.Z()));
    }
    
    // Linear combinations - keeping original method names and signatures
    void SetLinearForm(const Standard_Real A1, const ocgd_gp_XYZ *XYZ1,
                      const Standard_Real A2, const ocgd_gp_XYZ *XYZ2,
                      const Standard_Real A3, const ocgd_gp_XYZ *XYZ3,
                      const ocgd_gp_XYZ *XYZ4) {
        xyz.SetLinearForm(A1, XYZ1->xyz, A2, XYZ2->xyz, A3, XYZ3->xyz, XYZ4->xyz);
    }
    
    void SetLinearForm(const Standard_Real A1, const ocgd_gp_XYZ *XYZ1,
                      const Standard_Real A2, const ocgd_gp_XYZ *XYZ2,
                      const Standard_Real A3, const ocgd_gp_XYZ *XYZ3) {
        xyz.SetLinearForm(A1, XYZ1->xyz, A2, XYZ2->xyz, A3, XYZ3->xyz);
    }
    
    void SetLinearForm(const Standard_Real A1, const ocgd_gp_XYZ *XYZ1,
                      const Standard_Real A2, const ocgd_gp_XYZ *XYZ2,
                      const ocgd_gp_XYZ *XYZ3) {
        xyz.SetLinearForm(A1, XYZ1->xyz, A2, XYZ2->xyz, XYZ3->xyz);
    }
    
    void SetLinearForm(const Standard_Real A1, const ocgd_gp_XYZ *XYZ1,
                      const Standard_Real A2, const ocgd_gp_XYZ *XYZ2) {
        xyz.SetLinearForm(A1, XYZ1->xyz, A2, XYZ2->xyz);
    }
    
    void SetLinearForm(const Standard_Real A1, const ocgd_gp_XYZ *XYZ1,
                      const ocgd_gp_XYZ *XYZ2) {
        xyz.SetLinearForm(A1, XYZ1->xyz, XYZ2->xyz);
    }
    
    void SetLinearForm(const ocgd_gp_XYZ *XYZ1, const ocgd_gp_XYZ *XYZ2) {
        xyz.SetLinearForm(XYZ1->xyz, XYZ2->xyz);
    }
    
    // Internal access for other wrappers
    const gp_XYZ& get_xyz() const { return xyz; }
    gp_XYZ& get_xyz() { return xyz; }
};

#endif // _ocgd_gp_XYZ_h