#ifndef _ocgd_gp_Dir2d_h
#define _ocgd_gp_Dir2d_h

/**
 * ocgd_gp_Dir2d.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Dir2d as a Godot class.
 * 
 * A gp_Dir2d represents a unit vector in 2D space. The vector is always normalized.
 * Used to represent directions in the plane.
 * 
 * Original OCCT header: <opencascade/gp_Dir2d.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Dir2d.hxx>

#include "ocgd_gp_XY.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Dir2d
 *
 * Wrapper class to expose the OpenCASCADE gp_Dir2d class to Godot.
 * 
 * Defines a normalized vector in 2D space.
 * 
 * The class maps closely to Godot's Vector2 but maintains the OpenCASCADE API
 * for consistency with the rest of the OpenCASCADE bindings.
 */
class ocgd_gp_Dir2d : public RefCounted {
    GDCLASS(ocgd_gp_Dir2d, RefCounted)

private:
    gp_Dir2d dir; // The actual OpenCASCADE direction

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Dir2d", D_METHOD("new"), &ocgd_gp_Dir2d::_new);

        // Conversion methods
        ClassDB::bind_method(D_METHOD("to_vector2"), &ocgd_gp_Dir2d::to_vector2);
        
        // Coordinate access
        ClassDB::bind_method(D_METHOD("SetCoord", "Index", "Xi"), static_cast<void (ocgd_gp_Dir2d::*)(const Standard_Integer, const Standard_Real)>(&ocgd_gp_Dir2d::SetCoord));
        ClassDB::bind_method(D_METHOD("SetCoordXY", "Xv", "Yv"), static_cast<void (ocgd_gp_Dir2d::*)(const Standard_Real, const Standard_Real)>(&ocgd_gp_Dir2d::SetCoord));
        ClassDB::bind_method(D_METHOD("SetX", "X"), &ocgd_gp_Dir2d::SetX);
        ClassDB::bind_method(D_METHOD("SetY", "Y"), &ocgd_gp_Dir2d::SetY);
        ClassDB::bind_method(D_METHOD("SetXY", "Coord"), &ocgd_gp_Dir2d::SetXY);
        ClassDB::bind_method(D_METHOD("Coord", "Index"), static_cast<Standard_Real (ocgd_gp_Dir2d::*)(const Standard_Integer) const>(&ocgd_gp_Dir2d::Coord));
        ClassDB::bind_method(D_METHOD("X"), &ocgd_gp_Dir2d::X);
        ClassDB::bind_method(D_METHOD("Y"), &ocgd_gp_Dir2d::Y);
        ClassDB::bind_method(D_METHOD("XY"), &ocgd_gp_Dir2d::XY);
        
        // Direction comparison operations
        ClassDB::bind_method(D_METHOD("IsEqual", "Other", "AngularTolerance"), &ocgd_gp_Dir2d::IsEqual);
        ClassDB::bind_method(D_METHOD("IsNormal", "Other", "AngularTolerance"), &ocgd_gp_Dir2d::IsNormal);
        ClassDB::bind_method(D_METHOD("IsOpposite", "Other", "AngularTolerance"), &ocgd_gp_Dir2d::IsOpposite);
        ClassDB::bind_method(D_METHOD("IsParallel", "Other", "AngularTolerance"), &ocgd_gp_Dir2d::IsParallel);
        ClassDB::bind_method(D_METHOD("Angle", "Other"), &ocgd_gp_Dir2d::Angle);
        
        // Vector operations
        ClassDB::bind_method(D_METHOD("Crossed", "Other"), &ocgd_gp_Dir2d::Crossed);
        ClassDB::bind_method(D_METHOD("Dot", "Other"), &ocgd_gp_Dir2d::Dot);
        
        ClassDB::bind_method(D_METHOD("Reverse"), &ocgd_gp_Dir2d::Reverse);
        ClassDB::bind_method(D_METHOD("Reversed"), &ocgd_gp_Dir2d::Reversed);
        
        ClassDB::bind_method(D_METHOD("Rotate", "Ang"), &ocgd_gp_Dir2d::Rotate);
        ClassDB::bind_method(D_METHOD("Rotated", "Ang"), &ocgd_gp_Dir2d::Rotated);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "X"), "SetX", "X");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Y"), "SetY", "Y");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Dir2d> _new() { return memnew(ocgd_gp_Dir2d); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Dir2d() : dir() {} // Creates a direction with zero coordinates
    ocgd_gp_Dir2d(const Standard_Real Xv, const Standard_Real Yv) : dir(Xv, Yv) {} // Creates direction from coordinates
    
    // Convert from/to Godot Vector2 (additional convenience methods)
    ocgd_gp_Dir2d(const Vector2 &v) : dir(v.x, v.y) {}
    Vector2 to_vector2() const { return Vector2(dir.X(), dir.Y()); }
    
    // Access methods - matching original API names
    void SetCoord(const Standard_Integer Index, const Standard_Real Xi) { dir.SetCoord(Index, Xi); }
    void SetCoord(const Standard_Real Xv, const Standard_Real Yv) { dir.SetCoord(Xv, Yv); }
    void SetX(const Standard_Real X) { dir.SetX(X); }
    void SetY(const Standard_Real Y) { dir.SetY(Y); }
    void SetXY(const Ref<ocgd_gp_XY>& Coord) { dir.SetXY(Coord->get_xy()); }
    Standard_Real Coord(const Standard_Integer Index) const { return dir.Coord(Index); }
    void Coord(Standard_Real& Xv, Standard_Real& Yv) const { dir.Coord(Xv, Yv); }
    Standard_Real X() const { return dir.X(); }
    Standard_Real Y() const { return dir.Y(); }
    Ref<ocgd_gp_XY> XY() const { 
        Ref<ocgd_gp_XY> xy;
        xy.instantiate();
        xy->get_xy() = dir.XY();
        return xy;
    }
    
    // Direction operations - matching original API names
    Standard_Boolean IsEqual(const Ref<ocgd_gp_Dir2d>& Other, const Standard_Real AngularTolerance) const {
        return dir.IsEqual(Other->get_dir(), AngularTolerance);
    }
    Standard_Boolean IsNormal(const Ref<ocgd_gp_Dir2d>& Other, const Standard_Real AngularTolerance) const {
        return dir.IsNormal(Other->get_dir(), AngularTolerance);
    }
    Standard_Boolean IsOpposite(const Ref<ocgd_gp_Dir2d>& Other, const Standard_Real AngularTolerance) const {
        return dir.IsOpposite(Other->get_dir(), AngularTolerance);
    }
    Standard_Boolean IsParallel(const Ref<ocgd_gp_Dir2d>& Other, const Standard_Real AngularTolerance) const {
        return dir.IsParallel(Other->get_dir(), AngularTolerance);
    }
    Standard_Real Angle(const Ref<ocgd_gp_Dir2d>& Other) const {
        return dir.Angle(Other->get_dir());
    }
    
    Standard_Real Crossed(const Ref<ocgd_gp_Dir2d>& Right) const {
        return dir.Crossed(Right->get_dir());
    }
    
    Standard_Real Dot(const Ref<ocgd_gp_Dir2d>& Other) const {
        return dir.Dot(Other->get_dir());
    }
    
    void Reverse() { dir.Reverse(); }
    Ref<ocgd_gp_Dir2d> Reversed() const {
        Ref<ocgd_gp_Dir2d> result;
        result.instantiate();
        result->get_dir() = dir.Reversed();
        return result;
    }
    
    void Rotate(const Standard_Real Ang) { dir.Rotate(Ang); }
    Ref<ocgd_gp_Dir2d> Rotated(const Standard_Real Ang) const {
        Ref<ocgd_gp_Dir2d> result;
        result.instantiate();
        result->get_dir() = dir.Rotated(Ang);
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Dir2d& get_dir() const { return dir; }
    gp_Dir2d& get_dir() { return dir; }
};

#endif // _ocgd_gp_Dir2d_h