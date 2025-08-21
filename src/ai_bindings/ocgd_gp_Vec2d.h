#ifndef _ocgd_gp_Vec2d_h
#define _ocgd_gp_Vec2d_h

/**
 * ocgd_gp_Vec2d.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Vec2d as a Godot class.
 * 
 * A gp_Vec2d represents a vector in 2D space. Unlike gp_Dir2d, it has magnitude.
 * Used to represent geometric vectors, translations, and other vector operations.
 * 
 * Original OCCT header: <opencascade/gp_Vec2d.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Vec2d.hxx>

#include "ocgd_gp_XY.h"
#include "ocgd_gp_Dir2d.h"
#include "ocgd_gp_Pnt2d.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Vec2d
 *
 * Wrapper class to expose the OpenCASCADE gp_Vec2d class to Godot.
 * 
 * Defines a geometric vector in 2D space.
 * 
 * The class maps closely to Godot's Vector2 but maintains the OpenCASCADE API
 * for consistency with the rest of the OpenCASCADE bindings.
 */
class ocgd_gp_Vec2d : public RefCounted {
    GDCLASS(ocgd_gp_Vec2d, RefCounted)

private:
    gp_Vec2d vec; // The actual OpenCASCADE vector

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Vec2d", D_METHOD("new"), &ocgd_gp_Vec2d::_new);

        // Conversion methods
        ClassDB::bind_method(D_METHOD("to_vector2"), &ocgd_gp_Vec2d::to_vector2);
        ClassDB::bind_method(D_METHOD("GetDir"), &ocgd_gp_Vec2d::GetDir);
        
        // Coordinate access
        ClassDB::bind_method(D_METHOD("SetCoord", "Index", "Xi"), &ocgd_gp_Vec2d::SetCoord_Index);
        ClassDB::bind_method(D_METHOD("SetCoordXY", "Xv", "Yv"), &ocgd_gp_Vec2d::SetCoordXY);
        ClassDB::bind_method(D_METHOD("SetX", "X"), &ocgd_gp_Vec2d::SetX);
        ClassDB::bind_method(D_METHOD("SetY", "Y"), &ocgd_gp_Vec2d::SetY);
        ClassDB::bind_method(D_METHOD("SetXY", "Coord"), &ocgd_gp_Vec2d::SetXY);
        ClassDB::bind_method(D_METHOD("Coord", "Index"), &ocgd_gp_Vec2d::Coord_Index);
        ClassDB::bind_method(D_METHOD("X"), &ocgd_gp_Vec2d::X);
        ClassDB::bind_method(D_METHOD("Y"), &ocgd_gp_Vec2d::Y);
        ClassDB::bind_method(D_METHOD("XY"), &ocgd_gp_Vec2d::XY);
        
        // Vector comparison operations
        ClassDB::bind_method(D_METHOD("IsEqual", "Other", "LinearTolerance", "AngularTolerance"), &ocgd_gp_Vec2d::IsEqual);
        ClassDB::bind_method(D_METHOD("IsNormal", "Other", "AngularTolerance"), &ocgd_gp_Vec2d::IsNormal);
        ClassDB::bind_method(D_METHOD("IsOpposite", "Other", "AngularTolerance"), &ocgd_gp_Vec2d::IsOpposite);
        ClassDB::bind_method(D_METHOD("IsParallel", "Other", "AngularTolerance"), &ocgd_gp_Vec2d::IsParallel);
        ClassDB::bind_method(D_METHOD("Angle", "Other"), &ocgd_gp_Vec2d::Angle);
        
        // Magnitude operations
        ClassDB::bind_method(D_METHOD("Magnitude"), &ocgd_gp_Vec2d::Magnitude);
        ClassDB::bind_method(D_METHOD("SquareMagnitude"), &ocgd_gp_Vec2d::SquareMagnitude);
        
        // Vector arithmetic
        ClassDB::bind_method(D_METHOD("Add", "Other"), &ocgd_gp_Vec2d::Add);
        ClassDB::bind_method(D_METHOD("Added", "Other"), &ocgd_gp_Vec2d::Added);
        ClassDB::bind_method(D_METHOD("Subtract", "Right"), &ocgd_gp_Vec2d::Subtract);
        ClassDB::bind_method(D_METHOD("Subtracted", "Right"), &ocgd_gp_Vec2d::Subtracted);
        ClassDB::bind_method(D_METHOD("MultiplyScalar", "Scalar"), &ocgd_gp_Vec2d::Multiply_Scalar);
        ClassDB::bind_method(D_METHOD("MultipliedScalar", "Scalar"), &ocgd_gp_Vec2d::Multiplied_Scalar);
        ClassDB::bind_method(D_METHOD("DivideScalar", "Scalar"), &ocgd_gp_Vec2d::Divide);
        ClassDB::bind_method(D_METHOD("DividedScalar", "Scalar"), &ocgd_gp_Vec2d::Divided);
        
        // Vector products
        ClassDB::bind_method(D_METHOD("Crossed", "Right"), &ocgd_gp_Vec2d::Crossed);
        ClassDB::bind_method(D_METHOD("CrossMagnitude", "Right"), &ocgd_gp_Vec2d::CrossMagnitude);
        ClassDB::bind_method(D_METHOD("CrossSquareMagnitude", "Right"), &ocgd_gp_Vec2d::CrossSquareMagnitude);
        ClassDB::bind_method(D_METHOD("Dot", "Other"), &ocgd_gp_Vec2d::Dot);
        
        // Vector operations
        ClassDB::bind_method(D_METHOD("Normalize"), &ocgd_gp_Vec2d::Normalize);
        ClassDB::bind_method(D_METHOD("Normalized"), &ocgd_gp_Vec2d::Normalized);
        ClassDB::bind_method(D_METHOD("Reverse"), &ocgd_gp_Vec2d::Reverse);
        ClassDB::bind_method(D_METHOD("Reversed"), &ocgd_gp_Vec2d::Reversed);
        ClassDB::bind_method(D_METHOD("Rotate", "Ang"), &ocgd_gp_Vec2d::Rotate);
        ClassDB::bind_method(D_METHOD("Rotated", "Ang"), &ocgd_gp_Vec2d::Rotated);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "X"), "SetX", "X");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Y"), "SetY", "Y");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Vec2d> _new() { return memnew(ocgd_gp_Vec2d); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Vec2d() : vec() {} // Creates a vector with zero coordinates
    ocgd_gp_Vec2d(const Standard_Real Xv, const Standard_Real Yv) : vec(Xv, Yv) {} // Creates vector from coordinates
    ocgd_gp_Vec2d(const Ref<ocgd_gp_Pnt2d>& P1, const Ref<ocgd_gp_Pnt2d>& P2) 
        : vec(P1->get_pnt(), P2->get_pnt()) {} // Creates vector from two points
    
    // Convert from/to Godot Vector2 (additional convenience methods)
    ocgd_gp_Vec2d(const Vector2 &v) : vec(v.x, v.y) {}
    Vector2 to_vector2() const { return Vector2(vec.X(), vec.Y()); }
    
    // Access methods - matching original API names
    void SetCoord_Index(const Standard_Integer Index, const Standard_Real Xi) { vec.SetCoord(Index, Xi); }
    void SetCoordXY(const Standard_Real Xv, const Standard_Real Yv) { vec.SetCoord(Xv, Yv); }
    void SetX(const Standard_Real X) { vec.SetX(X); }
    void SetY(const Standard_Real Y) { vec.SetY(Y); }
    void SetXY(const Ref<ocgd_gp_XY>& Coord) { vec.SetXY(Coord->get_xy()); }
    Standard_Real Coord_Index(const Standard_Integer Index) const { return vec.Coord(Index); }
    Standard_Real X() const { return vec.X(); }
    Standard_Real Y() const { return vec.Y(); }
    Ref<ocgd_gp_XY> XY() const { 
        Ref<ocgd_gp_XY> xy;
        xy.instantiate();
        xy->get_xy() = vec.XY();
        return xy;
    }
    
    // Vector operations - matching original API names
    Standard_Boolean IsEqual(const Ref<ocgd_gp_Vec2d>& Other, const Standard_Real LinearTolerance, const Standard_Real AngularTolerance) const {
        return vec.IsEqual(Other->get_vec(), LinearTolerance, AngularTolerance);
    }
    Standard_Boolean IsNormal(const Ref<ocgd_gp_Vec2d>& Other, const Standard_Real AngularTolerance) const {
        return vec.IsNormal(Other->get_vec(), AngularTolerance);
    }
    Standard_Boolean IsOpposite(const Ref<ocgd_gp_Vec2d>& Other, const Standard_Real AngularTolerance) const {
        return vec.IsOpposite(Other->get_vec(), AngularTolerance);
    }
    Standard_Boolean IsParallel(const Ref<ocgd_gp_Vec2d>& Other, const Standard_Real AngularTolerance) const {
        return vec.IsParallel(Other->get_vec(), AngularTolerance);
    }
    Standard_Real Angle(const Ref<ocgd_gp_Vec2d>& Other) const {
        return vec.Angle(Other->get_vec());
    }
    
    Standard_Real Magnitude() const { return vec.Magnitude(); }
    Standard_Real SquareMagnitude() const { return vec.SquareMagnitude(); }
    
    void Add(const Ref<ocgd_gp_Vec2d>& Other) { vec.Add(Other->get_vec()); }
    Ref<ocgd_gp_Vec2d> Added(const Ref<ocgd_gp_Vec2d>& Other) const {
        Ref<ocgd_gp_Vec2d> result;
        result.instantiate();
        result->get_vec() = vec.Added(Other->get_vec());
        return result;
    }
    
    void Subtract(const Ref<ocgd_gp_Vec2d>& Right) { vec.Subtract(Right->get_vec()); }
    Ref<ocgd_gp_Vec2d> Subtracted(const Ref<ocgd_gp_Vec2d>& Right) const {
        Ref<ocgd_gp_Vec2d> result;
        result.instantiate();
        result->get_vec() = vec.Subtracted(Right->get_vec());
        return result;
    }
    
    void Multiply_Scalar(const Standard_Real Scalar) { vec.Multiply(Scalar); }
    Ref<ocgd_gp_Vec2d> Multiplied_Scalar(const Standard_Real Scalar) const {
        Ref<ocgd_gp_Vec2d> result;
        result.instantiate();
        result->get_vec() = vec.Multiplied(Scalar);
        return result;
    }
    
    void Divide(const Standard_Real Scalar) { vec.Divide(Scalar); }
    Ref<ocgd_gp_Vec2d> Divided(const Standard_Real Scalar) const {
        Ref<ocgd_gp_Vec2d> result;
        result.instantiate();
        result->get_vec() = vec.Divided(Scalar);
        return result;
    }
    
    Standard_Real Crossed(const Ref<ocgd_gp_Vec2d>& Right) const {
        return vec.Crossed(Right->get_vec());
    }
    
    Standard_Real CrossMagnitude(const Ref<ocgd_gp_Vec2d>& Right) const {
        return vec.CrossMagnitude(Right->get_vec());
    }
    
    Standard_Real CrossSquareMagnitude(const Ref<ocgd_gp_Vec2d>& Right) const {
        return vec.CrossSquareMagnitude(Right->get_vec());
    }
    
    Standard_Real Dot(const Ref<ocgd_gp_Vec2d>& Other) const {
        return vec.Dot(Other->get_vec());
    }
    
    void Normalize() { vec.Normalize(); }
    Ref<ocgd_gp_Vec2d> Normalized() const {
        Ref<ocgd_gp_Vec2d> result;
        result.instantiate();
        result->get_vec() = vec.Normalized();
        return result;
    }
    
    void Reverse() { vec.Reverse(); }
    Ref<ocgd_gp_Vec2d> Reversed() const {
        Ref<ocgd_gp_Vec2d> result;
        result.instantiate();
        result->get_vec() = vec.Reversed();
        return result;
    }
    
    void Rotate(const Standard_Real Ang) { vec.Rotate(Ang); }
    Ref<ocgd_gp_Vec2d> Rotated(const Standard_Real Ang) const {
        Ref<ocgd_gp_Vec2d> result;
        result.instantiate();
        result->get_vec() = vec.Rotated(Ang);
        return result;
    }
    
    // Conversion methods
    Ref<ocgd_gp_Dir2d> GetDir() const {
        Ref<ocgd_gp_Dir2d> result;
        result.instantiate();
        result->get_dir() = gp_Dir2d(vec);
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Vec2d& get_vec() const { return vec; }
    gp_Vec2d& get_vec() { return vec; }
};

#endif // _ocgd_gp_Vec2d_h