/**
 * ocgd_BRepMesh_IncrementalMesh.cpp
 *
 * Godot GDExtension wrapper implementation for OpenCASCADE BRepMesh_IncrementalMesh class.
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "ocgd_BRepMesh_IncrementalMesh.hxx"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <opencascade/BRepMesh_IncrementalMesh.hxx>
#include <opencascade/TopoDS_Shape.hxx>
#include <opencascade/IMeshTools_Parameters.hxx>
#include <opencascade/Message_ProgressRange.hxx>

using namespace godot;

ocgd_BRepMesh_IncrementalMesh::ocgd_BRepMesh_IncrementalMesh() {
    _mesh = new BRepMesh_IncrementalMesh();
    _owns_mesh = true;
}

ocgd_BRepMesh_IncrementalMesh::~ocgd_BRepMesh_IncrementalMesh() {
    if (_owns_mesh && _mesh != nullptr) {
        delete _mesh;
        _mesh = nullptr;
    }
}

void ocgd_BRepMesh_IncrementalMesh::init_with_shape(const Ref<ocgd_TopoDS_Shape>& shape,
                                                   double linear_deflection,
                                                   bool is_relative,
                                                   double angular_deflection,
                                                   bool is_in_parallel) {
    if (shape.is_null()) {
        UtilityFunctions::printerr("Cannot initialize BRepMesh_IncrementalMesh with null shape");
        return;
    }

    if (_owns_mesh && _mesh != nullptr) {
        delete _mesh;
    }

    _mesh = new BRepMesh_IncrementalMesh(
        shape->get_occt_shape(),
        linear_deflection,
        is_relative ? Standard_True : Standard_False,
        angular_deflection,
        is_in_parallel ? Standard_True : Standard_False
    );
    _owns_mesh = true;
}

void ocgd_BRepMesh_IncrementalMesh::perform() {
    if (_mesh == nullptr) {
        UtilityFunctions::printerr("BRepMesh_IncrementalMesh is not initialized");
        return;
    }

    Message_ProgressRange progress;
    _mesh->Perform(progress);
}

double ocgd_BRepMesh_IncrementalMesh::get_linear_deflection() const {
    if (_mesh == nullptr) {
        return 0.0;
    }
    return _mesh->Parameters().Deflection;
}

void ocgd_BRepMesh_IncrementalMesh::set_linear_deflection(double deflection) {
    if (_mesh == nullptr) {
        UtilityFunctions::printerr("BRepMesh_IncrementalMesh is not initialized");
        return;
    }
    _mesh->ChangeParameters().Deflection = deflection;
}

double ocgd_BRepMesh_IncrementalMesh::get_angular_deflection() const {
    if (_mesh == nullptr) {
        return 0.0;
    }
    return _mesh->Parameters().Angle;
}

void ocgd_BRepMesh_IncrementalMesh::set_angular_deflection(double deflection) {
    if (_mesh == nullptr) {
        UtilityFunctions::printerr("BRepMesh_IncrementalMesh is not initialized");
        return;
    }
    _mesh->ChangeParameters().Angle = deflection;
}

bool ocgd_BRepMesh_IncrementalMesh::get_relative_deflection() const {
    if (_mesh == nullptr) {
        return false;
    }
    return _mesh->Parameters().Relative == Standard_True;
}

void ocgd_BRepMesh_IncrementalMesh::set_relative_deflection(bool is_relative) {
    if (_mesh == nullptr) {
        UtilityFunctions::printerr("BRepMesh_IncrementalMesh is not initialized");
        return;
    }
    _mesh->ChangeParameters().Relative = is_relative ? Standard_True : Standard_False;
}

bool ocgd_BRepMesh_IncrementalMesh::get_parallel_processing() const {
    if (_mesh == nullptr) {
        return false;
    }
    return _mesh->Parameters().InParallel == Standard_True;
}

void ocgd_BRepMesh_IncrementalMesh::set_parallel_processing(bool is_parallel) {
    if (_mesh == nullptr) {
        UtilityFunctions::printerr("BRepMesh_IncrementalMesh is not initialized");
        return;
    }
    _mesh->ChangeParameters().InParallel = is_parallel ? Standard_True : Standard_False;
}

bool ocgd_BRepMesh_IncrementalMesh::is_modified() const {
    if (_mesh == nullptr) {
        return false;
    }
    return _mesh->IsModified() == Standard_True;
}

int ocgd_BRepMesh_IncrementalMesh::get_status_flags() const {
    if (_mesh == nullptr) {
        return 0;
    }
    return _mesh->GetStatusFlags();
}

double ocgd_BRepMesh_IncrementalMesh::get_min_size() const {
    if (_mesh == nullptr) {
        return 0.0;
    }
    return _mesh->Parameters().MinSize;
}

void ocgd_BRepMesh_IncrementalMesh::set_min_size(double min_size) {
    if (_mesh == nullptr) {
        UtilityFunctions::printerr("BRepMesh_IncrementalMesh is not initialized");
        return;
    }
    _mesh->ChangeParameters().MinSize = min_size;
}

double ocgd_BRepMesh_IncrementalMesh::get_interior_deflection() const {
    if (_mesh == nullptr) {
        return 0.0;
    }
    return _mesh->Parameters().DeflectionInterior;
}

void ocgd_BRepMesh_IncrementalMesh::set_interior_deflection(double deflection) {
    if (_mesh == nullptr) {
        UtilityFunctions::printerr("BRepMesh_IncrementalMesh is not initialized");
        return;
    }
    _mesh->ChangeParameters().DeflectionInterior = deflection;
}

double ocgd_BRepMesh_IncrementalMesh::get_interior_angular_deflection() const {
    if (_mesh == nullptr) {
        return 0.0;
    }
    return _mesh->Parameters().AngleInterior;
}

void ocgd_BRepMesh_IncrementalMesh::set_interior_angular_deflection(double deflection) {
    if (_mesh == nullptr) {
        UtilityFunctions::printerr("BRepMesh_IncrementalMesh is not initialized");
        return;
    }
    _mesh->ChangeParameters().AngleInterior = deflection;
}



void ocgd_BRepMesh_IncrementalMesh::_bind_methods() {
    // Initialization methods
    ClassDB::bind_method(D_METHOD("init_with_shape", "shape", "linear_deflection", "is_relative", "angular_deflection", "is_in_parallel"), 
                        &ocgd_BRepMesh_IncrementalMesh::init_with_shape, 
                        DEFVAL(false), DEFVAL(0.5), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("perform"), &ocgd_BRepMesh_IncrementalMesh::perform);
    
    // Linear deflection property
    ClassDB::bind_method(D_METHOD("get_linear_deflection"), &ocgd_BRepMesh_IncrementalMesh::get_linear_deflection);
    ClassDB::bind_method(D_METHOD("set_linear_deflection", "deflection"), &ocgd_BRepMesh_IncrementalMesh::set_linear_deflection);
    ClassDB::add_property("ocgd_BRepMesh_IncrementalMesh", PropertyInfo(Variant::FLOAT, "linear_deflection"), "set_linear_deflection", "get_linear_deflection");
    
    // Angular deflection property
    ClassDB::bind_method(D_METHOD("get_angular_deflection"), &ocgd_BRepMesh_IncrementalMesh::get_angular_deflection);
    ClassDB::bind_method(D_METHOD("set_angular_deflection", "deflection"), &ocgd_BRepMesh_IncrementalMesh::set_angular_deflection);
    ClassDB::add_property("ocgd_BRepMesh_IncrementalMesh", PropertyInfo(Variant::FLOAT, "angular_deflection"), "set_angular_deflection", "get_angular_deflection");
    
    // Relative deflection property
    ClassDB::bind_method(D_METHOD("get_relative_deflection"), &ocgd_BRepMesh_IncrementalMesh::get_relative_deflection);
    ClassDB::bind_method(D_METHOD("set_relative_deflection", "is_relative"), &ocgd_BRepMesh_IncrementalMesh::set_relative_deflection);
    ClassDB::add_property("ocgd_BRepMesh_IncrementalMesh", PropertyInfo(Variant::BOOL, "relative_deflection"), "set_relative_deflection", "get_relative_deflection");
    
    // Parallel processing property
    ClassDB::bind_method(D_METHOD("get_parallel_processing"), &ocgd_BRepMesh_IncrementalMesh::get_parallel_processing);
    ClassDB::bind_method(D_METHOD("set_parallel_processing", "is_parallel"), &ocgd_BRepMesh_IncrementalMesh::set_parallel_processing);
    ClassDB::add_property("ocgd_BRepMesh_IncrementalMesh", PropertyInfo(Variant::BOOL, "parallel_processing"), "set_parallel_processing", "get_parallel_processing");
    
    // Min size property
    ClassDB::bind_method(D_METHOD("get_min_size"), &ocgd_BRepMesh_IncrementalMesh::get_min_size);
    ClassDB::bind_method(D_METHOD("set_min_size", "min_size"), &ocgd_BRepMesh_IncrementalMesh::set_min_size);
    ClassDB::add_property("ocgd_BRepMesh_IncrementalMesh", PropertyInfo(Variant::FLOAT, "min_size"), "set_min_size", "get_min_size");
    
    // Interior deflection property
    ClassDB::bind_method(D_METHOD("get_interior_deflection"), &ocgd_BRepMesh_IncrementalMesh::get_interior_deflection);
    ClassDB::bind_method(D_METHOD("set_interior_deflection", "deflection"), &ocgd_BRepMesh_IncrementalMesh::set_interior_deflection);
    ClassDB::add_property("ocgd_BRepMesh_IncrementalMesh", PropertyInfo(Variant::FLOAT, "interior_deflection"), "set_interior_deflection", "get_interior_deflection");
    
    // Interior angular deflection property
    ClassDB::bind_method(D_METHOD("get_interior_angular_deflection"), &ocgd_BRepMesh_IncrementalMesh::get_interior_angular_deflection);
    ClassDB::bind_method(D_METHOD("set_interior_angular_deflection", "deflection"), &ocgd_BRepMesh_IncrementalMesh::set_interior_angular_deflection);
    ClassDB::add_property("ocgd_BRepMesh_IncrementalMesh", PropertyInfo(Variant::FLOAT, "interior_angular_deflection"), "set_interior_angular_deflection", "get_interior_angular_deflection");
    
    // Status methods
    ClassDB::bind_method(D_METHOD("is_modified"), &ocgd_BRepMesh_IncrementalMesh::is_modified);
    ClassDB::bind_method(D_METHOD("get_status_flags"), &ocgd_BRepMesh_IncrementalMesh::get_status_flags);
    
    // Note: Static methods set_parallel_default and is_parallel_default 
    // cannot be bound through ClassDB, they would need to be exposed differently
}