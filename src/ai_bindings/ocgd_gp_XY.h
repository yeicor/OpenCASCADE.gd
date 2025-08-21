#ifndef _ocgd_gp_XY_h
#define _ocgd_gp_XY_h

/**
 * ocgd_gp_XY.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_XY as a Godot class.
 * 
 * This class represents a cartesian coordinate entity in 2D space {X,Y}.
 * Used for algebraic calculation and vectorial computations.
 * 
 * Original OCCT header: <opencascade/gp_XY.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_XY.hxx>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_XY
 *
 * Wrapper class to expose the OpenCASCADE gp_XY class to Godot.
 * 
 * This class describes a cartesian coordinate entity in 2D space {X,Y}.
 * Used for vectorial computations or for holding this type of information 
 * in data structures.
 * 
 * The class maps closely to Godot's Vector2 but maintains the OpenCASCADE API
 * for consistency with the rest of the OpenCASCADE bindings.
 */
class ocgd_gp_XY : public RefCounted {
    GDCLASS(ocgd_gp_XY, RefCounted)

private:
    gp_XY xy; // The actual OpenCASCADE point

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_XY", D_METHOD("new"), &ocgd_gp_XY::_new);

        // Conversion methods
        ClassDB::bind_method(D_METHOD("to_vector2"), &ocgd_gp_XY::to_vector2);
        
        // Coordinate access
        ClassDB::bind_method(D_METHOD("SetCoord", "Index", "Xi"), &ocgd_gp_XY::SetCoord_Index);
        ClassDB::bind_method(D_METHOD("SetCoordXY", "X", "Y"), &ocgd_gp_XY::SetCoordXY);
        ClassDB::bind_method(D_METHOD("SetX", "X"), &ocgd_gp_XY::SetX);
        ClassDB::bind_method(D_METHOD("SetY", "Y"), &ocgd_gp_XY::SetY);
        ClassDB::bind_method(D_METHOD("Coord", "Index"), &ocgd_gp_XY::Coord_Index);
        ClassDB::bind_method(D_METHOD("X"), &ocgd_gp_XY::X);
        ClassDB::bind_method(D_METHOD("Y"), &ocgd_gp_XY::Y);
        
        // Vector operations
        ClassDB::bind_method(D_METHOD("Add", "Other"), &ocgd_gp_XY::Add);
        ClassDB::bind_method(D_METHOD("Added", "Other"), &ocgd_gp_XY::Added);
        ClassDB::bind_method(D_METHOD("Crossed", "Right"), &ocgd_gp_XY::Crossed);
        ClassDB::bind_method(D_METHOD("CrossMagnitude", "Right"), &ocgd_gp_XY::CrossMagnitude);
        ClassDB::bind_method(D_METHOD("CrossSquareMagnitude", "Right"), &ocgd_gp_XY::CrossSquareMagnitude);
        ClassDB::bind_method(D_METHOD("Divide", "Scalar"), &ocgd_gp_XY::Divide);
        ClassDB::bind_method(D_METHOD("Divided", "Scalar"), &ocgd_gp_XY::Divided);
        ClassDB::bind_method(D_METHOD("Dot", "Other"), &ocgd_gp_XY::Dot);
        ClassDB::bind_method(D_METHOD("IsEqual", "Other", "Tolerance"), &ocgd_gp_XY::IsEqual);
        ClassDB::bind_method(D_METHOD("Modulus"), &ocgd_gp_XY::Modulus);
        ClassDB::bind_method(D_METHOD("MultipliedScalar", "Scalar"), &ocgd_gp_XY::Multiplied_Scalar);
        ClassDB::bind_method(D_METHOD("MultipliedXY", "Other"), &ocgd_gp_XY::Multiplied_XY);
        ClassDB::bind_method(D_METHOD("MultiplyScalar", "Scalar"), &ocgd_gp_XY::Multiply_Scalar);
        ClassDB::bind_method(D_METHOD("MultiplyXY", "Other"), &ocgd_gp_XY::Multiply_XY);
        ClassDB::bind_method(D_METHOD("Normalize"), &ocgd_gp_XY::Normalize);
        ClassDB::bind_method(D_METHOD("Normalized"), &ocgd_gp_XY::Normalized);
        ClassDB::bind_method(D_METHOD("Reverse"), &ocgd_gp_XY::Reverse);
        ClassDB::bind_method(D_METHOD("Reversed"), &ocgd_gp_XY::Reversed);
        ClassDB::bind_method(D_METHOD("SquareModulus"), &ocgd_gp_XY::SquareModulus);
        ClassDB::bind_method(D_METHOD("Subtract", "Right"), &ocgd_gp_XY::Subtract);
        ClassDB::bind_method(D_METHOD("Subtracted", "Right"), &ocgd_gp_XY::Subtracted);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "X"), "SetX", "X");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Y"), "SetY", "Y");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_XY> _new() { return memnew(ocgd_gp_XY); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_XY() : xy() {} // Creates XY with zero coordinates (0,0)
    ocgd_gp_XY(const Standard_Real X, const Standard_Real Y) : xy(X, Y) {} // Creates XY from coordinates
    
    // Convert from/to Godot Vector2 (additional convenience methods)
    ocgd_gp_XY(const Vector2 &v) : xy(v.x, v.y) {}
    Vector2 to_vector2() const { return Vector2(xy.X(), xy.Y()); }
    
    // Access methods - matching original API names
    void SetCoord_Index(const Standard_Integer Index, const Standard_Real Xi) { xy.SetCoord(Index, Xi); }
    void SetCoordXY(const Standard_Real X, const Standard_Real Y) { xy.SetCoord(X, Y); }
    void SetX(const Standard_Real X) { xy.SetX(X); }
    void SetY(const Standard_Real Y) { xy.SetY(Y); }
    Standard_Real Coord_Index(const Standard_Integer Index) const { return xy.Coord(Index); }
    Standard_Real X() const { return xy.X(); }
    Standard_Real Y() const { return xy.Y(); }
    
    // Mathematical operations - matching original API names
    Standard_Real Modulus() const { return xy.Modulus(); }
    Standard_Real SquareModulus() const { return xy.SquareModulus(); }
    Standard_Boolean IsEqual(const Ref<ocgd_gp_XY>& Other, const Standard_Real Tolerance) const { return xy.IsEqual(Other->get_xy(), Tolerance); }
    
    // Vector operations - matching original API names
    void Add(const Ref<ocgd_gp_XY>& Other) { xy.Add(Other->get_xy()); }
    Ref<ocgd_gp_XY> Added(const Ref<ocgd_gp_XY>& Other) const {
        gp_XY result = xy.Added(Other->get_xy());
        return memnew(ocgd_gp_XY(result.X(), result.Y()));
    }
    
    Standard_Real Crossed(const Ref<ocgd_gp_XY>& Other) const { return xy.Crossed(Other->get_xy()); }
    Standard_Real CrossMagnitude(const Ref<ocgd_gp_XY>& Right) const { return xy.CrossMagnitude(Right->get_xy()); }
    Standard_Real CrossSquareMagnitude(const Ref<ocgd_gp_XY>& Right) const { return xy.CrossSquareMagnitude(Right->get_xy()); }
    
    void Divide(const Standard_Real Scalar) { xy.Divide(Scalar); }
    Ref<ocgd_gp_XY> Divided(const Standard_Real Scalar) const {
        gp_XY result = xy.Divided(Scalar);
        return memnew(ocgd_gp_XY(result.X(), result.Y()));
    }
    
    Standard_Real Dot(const Ref<ocgd_gp_XY>& Other) const { return xy.Dot(Other->get_xy()); }
    
    void Multiply_Scalar(const Standard_Real Scalar) { xy.Multiply(Scalar); }
    void Multiply_XY(const Ref<ocgd_gp_XY>& Other) { xy.Multiply(Other->get_xy()); }
    Ref<ocgd_gp_XY> Multiplied_Scalar(const Standard_Real Scalar) const {
        gp_XY result = xy.Multiplied(Scalar);
        return memnew(ocgd_gp_XY(result.X(), result.Y()));
    }
    Ref<ocgd_gp_XY> Multiplied_XY(const Ref<ocgd_gp_XY>& Other) const {
        gp_XY result = xy.Multiplied(Other->get_xy());
        return memnew(ocgd_gp_XY(result.X(), result.Y()));
    }
    
    void Normalize() { xy.Normalize(); }
    Ref<ocgd_gp_XY> Normalized() const {
        gp_XY result = xy.Normalized();
        return memnew(ocgd_gp_XY(result.X(), result.Y()));
    }
    
    void Reverse() { xy.Reverse(); }
    Ref<ocgd_gp_XY> Reversed() const {
        gp_XY result = xy.Reversed();
        return memnew(ocgd_gp_XY(result.X(), result.Y()));
    }
    
    void Subtract(const Ref<ocgd_gp_XY>& Other) { xy.Subtract(Other->get_xy()); }
    Ref<ocgd_gp_XY> Subtracted(const Ref<ocgd_gp_XY>& Other) const {
        gp_XY result = xy.Subtracted(Other->get_xy());
        return memnew(ocgd_gp_XY(result.X(), result.Y()));
    }
    
    // Linear combinations
    // Linear combinations - keeping original overloaded method names
    void SetLinearForm(double a1, const Ref<ocgd_gp_XY>& xy1, double a2, const Ref<ocgd_gp_XY>& xy2) {
        xy.SetLinearForm(a1, xy1->get_xy(), a2, xy2->get_xy());
    }
    void SetLinearForm(double a1, const Ref<ocgd_gp_XY>& xy1, double a2, const Ref<ocgd_gp_XY>& xy2, const Ref<ocgd_gp_XY>& xy3) {
        xy.SetLinearForm(a1, xy1->get_xy(), a2, xy2->get_xy(), xy3->get_xy());
    }
    void SetLinearForm(double a1, const Ref<ocgd_gp_XY>& xy1, const Ref<ocgd_gp_XY>& xy2) {
        xy.SetLinearForm(a1, xy1->get_xy(), xy2->get_xy());
    }
    void SetLinearForm(const Ref<ocgd_gp_XY>& xy1, const Ref<ocgd_gp_XY>& xy2) {
        xy.SetLinearForm(xy1->get_xy(), xy2->get_xy());
    }
    
    // Internal access for other wrappers
    const gp_XY& get_xy() const { return xy; }
    gp_XY& get_xy() { return xy; }
};

#endif // _ocgd_gp_XY_h