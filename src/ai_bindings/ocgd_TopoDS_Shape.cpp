/**
 * ocgd_TopoDS_Shape.cpp
 *
 * Godot GDExtension wrapper implementation for OpenCASCADE TopoDS_Shape class.
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "ocgd_TopoDS_Shape.hxx"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <opencascade/TopoDS_Shape.hxx>
#include <opencascade/TopAbs_ShapeEnum.hxx>
#include <opencascade/TopAbs_Orientation.hxx>
#include <opencascade/TopLoc_Location.hxx>
#include <opencascade/gp_Pnt.hxx>
#include <opencascade/gp_Trsf.hxx>

using namespace godot;

ocgd_TopoDS_Shape::ocgd_TopoDS_Shape() {
    _shape = TopoDS_Shape(); // Creates a NULL shape
}

ocgd_TopoDS_Shape::ocgd_TopoDS_Shape(const TopoDS_Shape& shape) {
    _shape = shape;
}

ocgd_TopoDS_Shape::~ocgd_TopoDS_Shape() {
    // TopoDS_Shape handles its own memory management
}

bool ocgd_TopoDS_Shape::is_null() const {
    return _shape.IsNull() == Standard_True;
}

void ocgd_TopoDS_Shape::nullify() {
    _shape.Nullify();
}

int ocgd_TopoDS_Shape::shape_type() const {
    if (_shape.IsNull()) {
        return static_cast<int>(TopAbs_SHAPE);
    }
    return static_cast<int>(_shape.ShapeType());
}

int ocgd_TopoDS_Shape::orientation() const {
    return static_cast<int>(_shape.Orientation());
}

void ocgd_TopoDS_Shape::set_orientation(int orient) {
    _shape.Orientation(static_cast<TopAbs_Orientation>(orient));
}

Vector3 ocgd_TopoDS_Shape::location() const {
    if (_shape.IsNull()) {
        return Vector3(0, 0, 0);
    }
    
    const TopLoc_Location& loc = _shape.Location();
    const gp_Trsf& trsf = loc.Transformation();
    const gp_XYZ& translation = trsf.TranslationPart();
    
    return Vector3(
        static_cast<float>(translation.X()),
        static_cast<float>(translation.Y()),
        static_cast<float>(translation.Z())
    );
}

bool ocgd_TopoDS_Shape::is_same(const Ref<ocgd_TopoDS_Shape>& other) const {
    if (other.is_null()) {
        return false;
    }
    return _shape.IsSame(other->get_occt_shape()) == Standard_True;
}

bool ocgd_TopoDS_Shape::is_equal(const Ref<ocgd_TopoDS_Shape>& other) const {
    if (other.is_null()) {
        return false;
    }
    return _shape.IsEqual(other->get_occt_shape()) == Standard_True;
}

bool ocgd_TopoDS_Shape::is_valid() const {
    if (_shape.IsNull()) {
        return false;
    }
    // Basic validity check - more complex checks could be added
    return true;
}

Ref<ocgd_TopoDS_Shape> ocgd_TopoDS_Shape::oriented(int orient) const {
    Ref<ocgd_TopoDS_Shape> result = memnew(ocgd_TopoDS_Shape);
    result->set_occt_shape(_shape.Oriented(static_cast<TopAbs_Orientation>(orient)));
    return result;
}

Ref<ocgd_TopoDS_Shape> ocgd_TopoDS_Shape::located(const Vector3& loc) const {
    gp_Trsf trsf;
    trsf.SetTranslation(gp_Vec(loc.x, loc.y, loc.z));
    TopLoc_Location location(trsf);
    
    Ref<ocgd_TopoDS_Shape> result = memnew(ocgd_TopoDS_Shape);
    result->set_occt_shape(_shape.Located(location));
    return result;
}

Ref<ocgd_TopoDS_Shape> ocgd_TopoDS_Shape::moved(const Vector3& loc) const {
    gp_Trsf trsf;
    trsf.SetTranslation(gp_Vec(loc.x, loc.y, loc.z));
    TopLoc_Location location(trsf);
    
    Ref<ocgd_TopoDS_Shape> result = memnew(ocgd_TopoDS_Shape);
    result->set_occt_shape(_shape.Moved(location));
    return result;
}

Ref<ocgd_TopoDS_Shape> ocgd_TopoDS_Shape::reversed() const {
    Ref<ocgd_TopoDS_Shape> result = memnew(ocgd_TopoDS_Shape);
    result->set_occt_shape(_shape.Reversed());
    return result;
}

Ref<ocgd_TopoDS_Shape> ocgd_TopoDS_Shape::complemented() const {
    Ref<ocgd_TopoDS_Shape> result = memnew(ocgd_TopoDS_Shape);
    result->set_occt_shape(_shape.Complemented());
    return result;
}

Ref<ocgd_TopoDS_Shape> ocgd_TopoDS_Shape::empty_copy() const {
    Ref<ocgd_TopoDS_Shape> result = memnew(ocgd_TopoDS_Shape);
    TopoDS_Shape shape_copy = _shape;
    shape_copy.EmptyCopy();
    result->set_occt_shape(shape_copy);
    return result;
}

int ocgd_TopoDS_Shape::hash_code() const {
    // Use pointer address of TShape as a simple hash
    if (_shape.IsNull()) {
        return 0;
    }
    return static_cast<int>(reinterpret_cast<uintptr_t>(_shape.TShape().get()) % INT_MAX);
}

void ocgd_TopoDS_Shape::_bind_methods() {
    // Basic shape methods
    ClassDB::bind_method(D_METHOD("is_null"), &ocgd_TopoDS_Shape::is_null);
    ClassDB::bind_method(D_METHOD("nullify"), &ocgd_TopoDS_Shape::nullify);
    ClassDB::bind_method(D_METHOD("shape_type"), &ocgd_TopoDS_Shape::shape_type);
    ClassDB::bind_method(D_METHOD("orientation"), &ocgd_TopoDS_Shape::orientation);
    ClassDB::bind_method(D_METHOD("set_orientation", "orient"), &ocgd_TopoDS_Shape::set_orientation);
    ClassDB::bind_method(D_METHOD("location"), &ocgd_TopoDS_Shape::location);
    
    // Comparison methods
    ClassDB::bind_method(D_METHOD("is_same", "other"), &ocgd_TopoDS_Shape::is_same);
    ClassDB::bind_method(D_METHOD("is_equal", "other"), &ocgd_TopoDS_Shape::is_equal);
    ClassDB::bind_method(D_METHOD("is_valid"), &ocgd_TopoDS_Shape::is_valid);
    
    // Transformation methods
    ClassDB::bind_method(D_METHOD("oriented", "orient"), &ocgd_TopoDS_Shape::oriented);
    ClassDB::bind_method(D_METHOD("located", "loc"), &ocgd_TopoDS_Shape::located);
    ClassDB::bind_method(D_METHOD("moved", "loc"), &ocgd_TopoDS_Shape::moved);
    ClassDB::bind_method(D_METHOD("reversed"), &ocgd_TopoDS_Shape::reversed);
    ClassDB::bind_method(D_METHOD("complemented"), &ocgd_TopoDS_Shape::complemented);
    ClassDB::bind_method(D_METHOD("empty_copy"), &ocgd_TopoDS_Shape::empty_copy);
    ClassDB::bind_method(D_METHOD("hash_code"), &ocgd_TopoDS_Shape::hash_code);
    
    // Bind shape type enum constants
    BIND_ENUM_CONSTANT(COMPOUND);
    BIND_ENUM_CONSTANT(COMPSOLID);
    BIND_ENUM_CONSTANT(SOLID);
    BIND_ENUM_CONSTANT(SHELL);
    BIND_ENUM_CONSTANT(FACE);
    BIND_ENUM_CONSTANT(WIRE);
    BIND_ENUM_CONSTANT(EDGE);
    BIND_ENUM_CONSTANT(VERTEX);
    BIND_ENUM_CONSTANT(SHAPE);
    
    // Bind orientation enum constants
    BIND_ENUM_CONSTANT(FORWARD);
    BIND_ENUM_CONSTANT(REVERSED);
    BIND_ENUM_CONSTANT(INTERNAL);
    BIND_ENUM_CONSTANT(EXTERNAL);
}