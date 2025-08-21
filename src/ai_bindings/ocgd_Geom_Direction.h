#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Geom_Direction.hxx>
#include <gp_Dir.hxx>
#include <gp_Trsf.hxx>

#include "ocgd_Geom_Vector.h"
#include "ocgd_gp_Dir.h"
#include "ocgd_gp_Trsf.h"

using namespace godot;

class OCGDGeom_Direction : public OCGDGeom_Vector {
    GDCLASS(OCGDGeom_Direction, OCGDGeom_Vector)

protected:
    Handle(Geom_Direction) geom_direction;

public:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("set_coord", "x", "y", "z"), &OCGDGeom_Direction::set_coord);
        ClassDB::bind_method(D_METHOD("set_dir", "v"), &OCGDGeom_Direction::set_dir);
        ClassDB::bind_method(D_METHOD("set_x", "x"), &OCGDGeom_Direction::set_x);
        ClassDB::bind_method(D_METHOD("set_y", "y"), &OCGDGeom_Direction::set_y);
        ClassDB::bind_method(D_METHOD("set_z", "z"), &OCGDGeom_Direction::set_z);
        ClassDB::bind_method(D_METHOD("dir"), &OCGDGeom_Direction::dir);
        ClassDB::bind_method(D_METHOD("crossed_direction", "other"), &OCGDGeom_Direction::crossed_direction);
        ClassDB::bind_method(D_METHOD("cross_crossed_direction", "v1", "v2"), &OCGDGeom_Direction::cross_crossed_direction);
        ClassDB::bind_method(D_METHOD("transform_direction", "t"), &OCGDGeom_Direction::transform_direction);
        ClassDB::bind_method(D_METHOD("copy_direction"), &OCGDGeom_Direction::copy_direction);
        ClassDB::bind_method(D_METHOD("to_string"), &OCGDGeom_Direction::to_string);
        
        // Static constructors
        ClassDB::bind_static_method("OCGDGeom_Direction", D_METHOD("create_from_coords", "x", "y", "z"), &OCGDGeom_Direction::create_from_coords);
        ClassDB::bind_static_method("OCGDGeom_Direction", D_METHOD("create_from_dir", "v"), &OCGDGeom_Direction::create_from_dir);
        ClassDB::bind_static_method("OCGDGeom_Direction", D_METHOD("create_from_vector3", "v"), &OCGDGeom_Direction::create_from_vector3);
    }

    OCGDGeom_Direction() {
        // Default constructor creates direction along Z-axis
        geom_direction = new Geom_Direction(0.0, 0.0, 1.0);
        set_geom_vector(geom_direction);
    }

    OCGDGeom_Direction(const Handle(Geom_Direction)& geom) : geom_direction(geom) {
        set_geom_vector(geom);
    }

    // Static constructor methods
    static Ref<OCGDGeom_Direction> create_from_coords(real_t x, real_t y, real_t z) {
        Ref<OCGDGeom_Direction> result;
        result.instantiate();
        result->geom_direction = new Geom_Direction(x, y, z);
        result->set_geom_vector(result->geom_direction);
        return result;
    }

    static Ref<OCGDGeom_Direction> create_from_dir(Ref<ocgd_gp_Dir> v) {
        if (v.is_null()) return Ref<OCGDGeom_Direction>();
        Ref<OCGDGeom_Direction> result;
        result.instantiate();
        result->geom_direction = new Geom_Direction(v->get_dir());
        result->set_geom_vector(result->geom_direction);
        return result;
    }

    static Ref<OCGDGeom_Direction> create_from_vector3(Vector3 v) {
        // Normalize the vector to make it a unit direction
        v = v.normalized();
        Ref<OCGDGeom_Direction> result;
        result.instantiate();
        result->geom_direction = new Geom_Direction(v.x, v.y, v.z);
        result->set_geom_vector(result->geom_direction);
        return result;
    }

    Handle(Geom_Direction) get_geom_direction() const { return geom_direction; }
    void set_geom_direction(const Handle(Geom_Direction)& geom) { 
        geom_direction = geom; 
        set_geom_vector(geom);
    }

    // Coordinate setting methods
    void set_coord(real_t x, real_t y, real_t z) {
        if (geom_direction.IsNull()) return;
        geom_direction->SetCoord(x, y, z);
    }

    void set_dir(Ref<ocgd_gp_Dir> v) {
        if (geom_direction.IsNull() || v.is_null()) return;
        geom_direction->SetDir(v->get_dir());
    }

    void set_x(real_t x) {
        if (geom_direction.IsNull()) return;
        geom_direction->SetX(x);
    }

    void set_y(real_t y) {
        if (geom_direction.IsNull()) return;
        geom_direction->SetY(y);
    }

    void set_z(real_t z) {
        if (geom_direction.IsNull()) return;
        geom_direction->SetZ(z);
    }

    // Get as gp_Dir wrapped in ocgd_gp_Dir
    Ref<ocgd_gp_Dir> dir() const {
        if (geom_direction.IsNull()) return Ref<ocgd_gp_Dir>();
        gp_Dir result = geom_direction->Dir();
        Ref<ocgd_gp_Dir> wrapped;
        wrapped.instantiate();
        wrapped->get_dir() = result;
        return wrapped;
    }

    // Override magnitude methods (always returns 1.0 for unit vectors)
    real_t magnitude() const override {
        return 1.0;
    }

    real_t square_magnitude() const override {
        return 1.0;
    }

    // Override cross product methods to maintain direction type
    void cross(Ref<OCGDGeom_Vector> other) override {
        if (geom_direction.IsNull() || other.is_null() || other->get_geom_vector().IsNull()) return;
        geom_direction->Cross(other->get_geom_vector());
    }

    Ref<OCGDGeom_Direction> crossed_direction(Ref<OCGDGeom_Vector> other) const {
        if (geom_direction.IsNull() || other.is_null() || other->get_geom_vector().IsNull()) {
            return Ref<OCGDGeom_Direction>();
        }
        Handle(Geom_Vector) result = geom_direction->Crossed(other->get_geom_vector());
        Handle(Geom_Direction) dir_result = Handle(Geom_Direction)::DownCast(result);
        if (dir_result.IsNull()) return Ref<OCGDGeom_Direction>();
        
        Ref<OCGDGeom_Direction> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_direction(dir_result);
        return wrapped;
    }

    void cross_cross(Ref<OCGDGeom_Vector> v1, Ref<OCGDGeom_Vector> v2) override {
        if (geom_direction.IsNull() || v1.is_null() || v2.is_null() ||
            v1->get_geom_vector().IsNull() || v2->get_geom_vector().IsNull()) return;
        geom_direction->CrossCross(v1->get_geom_vector(), v2->get_geom_vector());
    }

    Ref<OCGDGeom_Direction> cross_crossed_direction(Ref<OCGDGeom_Vector> v1, Ref<OCGDGeom_Vector> v2) const {
        if (geom_direction.IsNull() || v1.is_null() || v2.is_null() ||
            v1->get_geom_vector().IsNull() || v2->get_geom_vector().IsNull()) {
            return Ref<OCGDGeom_Direction>();
        }
        Handle(Geom_Vector) result = geom_direction->CrossCrossed(v1->get_geom_vector(), v2->get_geom_vector());
        Handle(Geom_Direction) dir_result = Handle(Geom_Direction)::DownCast(result);
        if (dir_result.IsNull()) return Ref<OCGDGeom_Direction>();
        
        Ref<OCGDGeom_Direction> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_direction(dir_result);
        return wrapped;
    }

    // Transform method for concrete type
    void transform_direction(Ref<ocgd_gp_Trsf> t) {
        if (geom_direction.IsNull() || t.is_null()) return;
        geom_direction->Transform(t->get_gp_trsf());
    }

    // Copy method to return the correct type
    Ref<OCGDGeom_Direction> copy_direction() {
        if (geom_direction.IsNull()) return Ref<OCGDGeom_Direction>();
        Handle(Geom_Geometry) result = geom_direction->Copy();
        Handle(Geom_Direction) dir_result = Handle(Geom_Direction)::DownCast(result);
        if (dir_result.IsNull()) return Ref<OCGDGeom_Direction>();
        
        Ref<OCGDGeom_Direction> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_direction(dir_result);
        return wrapped;
    }

    String to_string() const {
        if (geom_direction.IsNull()) return "Null Geom_Direction";
        Vector3 coords = coord();
        return String("Geom_Direction(") + String::num(coords.x) + ", " + 
               String::num(coords.y) + ", " + String::num(coords.z) + ")";
    }
};