#ifndef _ocgd_gp_Dir_h
#define _ocgd_gp_Dir_h

/**
 * ocgd_gp_Dir.h
 *
 * Godot GDExtension wrapper exposing OpenCASCADE gp_Dir as a Godot class.
 * 
 * A gp_Dir represents a unit vector in 3D space. The vector is always normalized.
 * Used to represent directions in space.
 * 
 * Original OCCT header: <opencascade/gp_Dir.hxx>
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "godot_cpp/core/binder_common.hpp"
#include <opencascade/gp_Dir.hxx>

#include "ocgd_gp_XYZ.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

/**
 * ocgd_gp_Dir
 *
 * Wrapper class to expose the OpenCASCADE gp_Dir class to Godot.
 * 
 * Defines a normalized vector in 3D space.
 * 
 * The class maps closely to Godot's Vector3 but maintains the OpenCASCADE API
 * for consistency with the rest of the OpenCASCADE bindings.
 */
class ocgd_gp_Dir : public RefCounted {
    GDCLASS(ocgd_gp_Dir, RefCounted)

private:
    gp_Dir dir; // The actual OpenCASCADE direction

protected:
    static void _bind_methods() {
        // Constructor
        ClassDB::bind_static_method("ocgd_gp_Dir", D_METHOD("new"), &ocgd_gp_Dir::_new);

        // Conversion methods
        ClassDB::bind_method(D_METHOD("to_vector3"), &ocgd_gp_Dir::to_vector3);
        
        // Coordinate access
        ClassDB::bind_method(D_METHOD("SetCoord", "Index", "Xi"), static_cast<void (ocgd_gp_Dir::*)(const Standard_Integer, const Standard_Real)>(&ocgd_gp_Dir::SetCoord));
        ClassDB::bind_method(D_METHOD("SetCoordXYZ", "Xv", "Yv", "Zv"), static_cast<void (ocgd_gp_Dir::*)(const Standard_Real, const Standard_Real, const Standard_Real)>(&ocgd_gp_Dir::SetCoord));
        ClassDB::bind_method(D_METHOD("SetX", "X"), &ocgd_gp_Dir::SetX);
        ClassDB::bind_method(D_METHOD("SetY", "Y"), &ocgd_gp_Dir::SetY);
        ClassDB::bind_method(D_METHOD("SetZ", "Z"), &ocgd_gp_Dir::SetZ);
        ClassDB::bind_method(D_METHOD("SetXYZ", "Coord"), &ocgd_gp_Dir::SetXYZ);
        ClassDB::bind_method(D_METHOD("Coord", "Index"), static_cast<Standard_Real (ocgd_gp_Dir::*)(const Standard_Integer) const>(&ocgd_gp_Dir::Coord));
        ClassDB::bind_method(D_METHOD("X"), &ocgd_gp_Dir::X);
        ClassDB::bind_method(D_METHOD("Y"), &ocgd_gp_Dir::Y);
        ClassDB::bind_method(D_METHOD("Z"), &ocgd_gp_Dir::Z);
        ClassDB::bind_method(D_METHOD("XYZ"), &ocgd_gp_Dir::XYZ);
        
        // Direction comparison operations
        ClassDB::bind_method(D_METHOD("IsEqual", "Other", "AngularTolerance"), &ocgd_gp_Dir::IsEqual);
        ClassDB::bind_method(D_METHOD("IsNormal", "Other", "AngularTolerance"), &ocgd_gp_Dir::IsNormal);
        ClassDB::bind_method(D_METHOD("IsOpposite", "Other", "AngularTolerance"), &ocgd_gp_Dir::IsOpposite);
        ClassDB::bind_method(D_METHOD("IsParallel", "Other", "AngularTolerance"), &ocgd_gp_Dir::IsParallel);
        ClassDB::bind_method(D_METHOD("Angle", "Other"), &ocgd_gp_Dir::Angle);
        ClassDB::bind_method(D_METHOD("AngleWithRef", "Other", "VRef"), &ocgd_gp_Dir::AngleWithRef);
        
        // Vector operations
        ClassDB::bind_method(D_METHOD("Cross", "Right"), &ocgd_gp_Dir::Cross);
        ClassDB::bind_method(D_METHOD("Crossed", "Right"), &ocgd_gp_Dir::Crossed);
        ClassDB::bind_method(D_METHOD("CrossCross", "V1", "V2"), &ocgd_gp_Dir::CrossCross);
        ClassDB::bind_method(D_METHOD("CrossCrossed", "V1", "V2"), &ocgd_gp_Dir::CrossCrossed);
        ClassDB::bind_method(D_METHOD("Dot", "Other"), &ocgd_gp_Dir::Dot);
        ClassDB::bind_method(D_METHOD("DotCross", "V1", "V2"), &ocgd_gp_Dir::DotCross);
        
        ClassDB::bind_method(D_METHOD("Reverse"), &ocgd_gp_Dir::Reverse);
        ClassDB::bind_method(D_METHOD("Reversed"), &ocgd_gp_Dir::Reversed);

        // Properties
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "X"), "SetX", "X");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Y"), "SetY", "Y");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Z"), "SetZ", "Z");
    }

public:
    // Constructor helper - matches original no-arg constructor
    static Ref<ocgd_gp_Dir> _new() { return memnew(ocgd_gp_Dir); }
    
    // Constructors - matching original API names and docs
    ocgd_gp_Dir() : dir() {} // Creates a direction with zero coordinates (not valid)
    ocgd_gp_Dir(const Standard_Real Xv, const Standard_Real Yv, const Standard_Real Zv) : dir(Xv, Yv, Zv) {} // Creates direction from coordinates
    
    // Convert from/to Godot Vector3 (additional convenience methods)
    ocgd_gp_Dir(const Vector3 &v) : dir(v.x, v.y, v.z) {}
    Vector3 to_vector3() const { return Vector3(dir.X(), dir.Y(), dir.Z()); }
    
    // Access methods - matching original API names
    void SetCoord(const Standard_Integer Index, const Standard_Real Xi) { dir.SetCoord(Index, Xi); }
    void SetCoord(const Standard_Real Xv, const Standard_Real Yv, const Standard_Real Zv) { dir.SetCoord(Xv, Yv, Zv); }
    void SetX(const Standard_Real X) { dir.SetX(X); }
    void SetY(const Standard_Real Y) { dir.SetY(Y); }
    void SetZ(const Standard_Real Z) { dir.SetZ(Z); }
    void SetXYZ(const Ref<ocgd_gp_XYZ>& Coord) { dir.SetXYZ(Coord->get_xyz()); }
    Standard_Real Coord(const Standard_Integer Index) const { return dir.Coord(Index); }
    void Coord(Standard_Real& Xv, Standard_Real& Yv, Standard_Real& Zv) const { dir.Coord(Xv, Yv, Zv); }
    Standard_Real X() const { return dir.X(); }
    Standard_Real Y() const { return dir.Y(); }
    Standard_Real Z() const { return dir.Z(); }
    Ref<ocgd_gp_XYZ> XYZ() const { 
        Ref<ocgd_gp_XYZ> xyz;
        xyz.instantiate();
        xyz->get_xyz() = dir.XYZ();
        return xyz;
    }
    
    // Direction operations - matching original API names
    Standard_Boolean IsEqual(const Ref<ocgd_gp_Dir>& Other, const Standard_Real AngularTolerance) const {
        return dir.IsEqual(Other->get_dir(), AngularTolerance);
    }
    Standard_Boolean IsNormal(const Ref<ocgd_gp_Dir>& Other, const Standard_Real AngularTolerance) const {
        return dir.IsNormal(Other->get_dir(), AngularTolerance);
    }
    Standard_Boolean IsOpposite(const Ref<ocgd_gp_Dir>& Other, const Standard_Real AngularTolerance) const {
        return dir.IsOpposite(Other->get_dir(), AngularTolerance);
    }
    Standard_Boolean IsParallel(const Ref<ocgd_gp_Dir>& Other, const Standard_Real AngularTolerance) const {
        return dir.IsParallel(Other->get_dir(), AngularTolerance);
    }
    Standard_Real Angle(const Ref<ocgd_gp_Dir>& Other) const {
        return dir.Angle(Other->get_dir());
    }
    Standard_Real AngleWithRef(const Ref<ocgd_gp_Dir>& Other, const Ref<ocgd_gp_Dir>& VRef) const {
        return dir.AngleWithRef(Other->get_dir(), VRef->get_dir());
    }
    
    // Vector operations - matching original API names
    void Cross(const Ref<ocgd_gp_Dir>& Right) { dir.Cross(Right->get_dir()); }
    Ref<ocgd_gp_Dir> Crossed(const Ref<ocgd_gp_Dir>& Right) const {
        Ref<ocgd_gp_Dir> result;
        result.instantiate();
        result->get_dir() = dir.Crossed(Right->get_dir());
        return result;
    }
    void CrossCross(const Ref<ocgd_gp_Dir>& V1, const Ref<ocgd_gp_Dir>& V2) {
        dir.CrossCross(V1->get_dir(), V2->get_dir());
    }
    Ref<ocgd_gp_Dir> CrossCrossed(const Ref<ocgd_gp_Dir>& V1, const Ref<ocgd_gp_Dir>& V2) const {
        Ref<ocgd_gp_Dir> result;
        result.instantiate();
        result->get_dir() = dir.CrossCrossed(V1->get_dir(), V2->get_dir());
        return result;
    }
    Standard_Real Dot(const Ref<ocgd_gp_Dir>& Other) const {
        return dir.Dot(Other->get_dir());
    }
    Standard_Real DotCross(const Ref<ocgd_gp_Dir>& V1, const Ref<ocgd_gp_Dir>& V2) const {
        return dir.DotCross(V1->get_dir(), V2->get_dir());
    }
    
    void Reverse() { dir.Reverse(); }
    Ref<ocgd_gp_Dir> Reversed() const {
        Ref<ocgd_gp_Dir> result;
        result.instantiate();
        result->get_dir() = dir.Reversed();
        return result;
    }
    
    // Internal access for other wrappers
    const gp_Dir& get_dir() const { return dir; }
    gp_Dir& get_dir() { return dir; }
};

#endif // _ocgd_gp_Dir_h