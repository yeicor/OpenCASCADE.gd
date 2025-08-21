/**
 * ocgd_TopExp_Explorer.cpp
 *
 * Godot GDExtension wrapper implementation for OpenCASCADE TopExp_Explorer class.
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "ocgd_TopExp_Explorer.hxx"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <opencascade/TopExp_Explorer.hxx>
#include <opencascade/TopoDS_Shape.hxx>
#include <opencascade/TopAbs_ShapeEnum.hxx>

using namespace godot;

ocgd_TopExp_Explorer::ocgd_TopExp_Explorer() {
    _explorer = TopExp_Explorer();
}

ocgd_TopExp_Explorer::~ocgd_TopExp_Explorer() {
    // TopExp_Explorer handles its own memory management
}

void ocgd_TopExp_Explorer::init(const Ref<ocgd_TopoDS_Shape>& shape, int to_find, int to_avoid) {
    if (shape.is_null()) {
        UtilityFunctions::printerr("Cannot initialize TopExp_Explorer with null shape");
        return;
    }
    
    _explorer.Init(
        shape->get_occt_shape(),
        static_cast<TopAbs_ShapeEnum>(to_find),
        static_cast<TopAbs_ShapeEnum>(to_avoid)
    );
}

bool ocgd_TopExp_Explorer::more() const {
    return _explorer.More() == Standard_True;
}

void ocgd_TopExp_Explorer::next() {
    if (!more()) {
        UtilityFunctions::printerr("TopExp_Explorer: No more shapes to explore");
        return;
    }
    _explorer.Next();
}

Ref<ocgd_TopoDS_Shape> ocgd_TopExp_Explorer::current() const {
    if (!more()) {
        UtilityFunctions::printerr("TopExp_Explorer: No current shape available");
        return Ref<ocgd_TopoDS_Shape>();
    }
    
    Ref<ocgd_TopoDS_Shape> result = memnew(ocgd_TopoDS_Shape);
    result->set_occt_shape(_explorer.Current());
    return result;
}

Ref<ocgd_TopoDS_Shape> ocgd_TopExp_Explorer::value() const {
    return current(); // value() is an alias for current()
}

void ocgd_TopExp_Explorer::re_init() {
    _explorer.ReInit();
}

Ref<ocgd_TopoDS_Shape> ocgd_TopExp_Explorer::explored_shape() const {
    Ref<ocgd_TopoDS_Shape> result = memnew(ocgd_TopoDS_Shape);
    result->set_occt_shape(_explorer.ExploredShape());
    return result;
}

int ocgd_TopExp_Explorer::depth() const {
    return _explorer.Depth();
}

void ocgd_TopExp_Explorer::clear() {
    _explorer.Clear();
}

void ocgd_TopExp_Explorer::_bind_methods() {
    // Initialization and control methods
    ClassDB::bind_method(D_METHOD("init", "shape", "to_find", "to_avoid"), &ocgd_TopExp_Explorer::init, DEFVAL(TopAbs_SHAPE));
    ClassDB::bind_method(D_METHOD("more"), &ocgd_TopExp_Explorer::more);
    ClassDB::bind_method(D_METHOD("next"), &ocgd_TopExp_Explorer::next);
    ClassDB::bind_method(D_METHOD("current"), &ocgd_TopExp_Explorer::current);
    ClassDB::bind_method(D_METHOD("value"), &ocgd_TopExp_Explorer::value);
    ClassDB::bind_method(D_METHOD("re_init"), &ocgd_TopExp_Explorer::re_init);
    ClassDB::bind_method(D_METHOD("explored_shape"), &ocgd_TopExp_Explorer::explored_shape);
    ClassDB::bind_method(D_METHOD("depth"), &ocgd_TopExp_Explorer::depth);
    ClassDB::bind_method(D_METHOD("clear"), &ocgd_TopExp_Explorer::clear);
    
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
}