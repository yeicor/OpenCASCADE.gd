#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Geom_CartesianPoint.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>

#include "ocgd_Geom_Point.h"
#include "ocgd_gp_Pnt.h"
#include "ocgd_gp_Trsf.h"

using namespace godot;

class OCGDGeom_CartesianPoint : public OCGDGeom_Point {
    GDCLASS(OCGDGeom_CartesianPoint, OCGDGeom_Point)

protected:
    Handle(Geom_CartesianPoint) geom_cartesian_point;

public:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("set_coord", "x", "y", "z"), &OCGDGeom_CartesianPoint::set_coord);
        ClassDB::bind_method(D_METHOD("set_pnt", "p"), &OCGDGeom_CartesianPoint::set_pnt);
        ClassDB::bind_method(D_METHOD("set_x", "x"), &OCGDGeom_CartesianPoint::set_x);
        ClassDB::bind_method(D_METHOD("set_y", "y"), &OCGDGeom_CartesianPoint::set_y);
        ClassDB::bind_method(D_METHOD("set_z", "z"), &OCGDGeom_CartesianPoint::set_z);
        ClassDB::bind_method(D_METHOD("transform_cartesian", "t"), &OCGDGeom_CartesianPoint::transform_cartesian);
        ClassDB::bind_method(D_METHOD("copy_cartesian"), &OCGDGeom_CartesianPoint::copy_cartesian);
        ClassDB::bind_method(D_METHOD("to_string"), &OCGDGeom_CartesianPoint::to_string);
        
        // Static constructors
        ClassDB::bind_static_method("OCGDGeom_CartesianPoint", D_METHOD("create_from_pnt", "p"), &OCGDGeom_CartesianPoint::create_from_pnt);
        ClassDB::bind_static_method("OCGDGeom_CartesianPoint", D_METHOD("create_from_coords", "x", "y", "z"), &OCGDGeom_CartesianPoint::create_from_coords);
        ClassDB::bind_static_method("OCGDGeom_CartesianPoint", D_METHOD("create_from_vector3", "v"), &OCGDGeom_CartesianPoint::create_from_vector3);
    }

    OCGDGeom_CartesianPoint() {
        // Default constructor creates point at origin
        geom_cartesian_point = new Geom_CartesianPoint(0.0, 0.0, 0.0);
        set_geom_point(geom_cartesian_point);
    }

    OCGDGeom_CartesianPoint(const Handle(Geom_CartesianPoint)& geom) : geom_cartesian_point(geom) {
        set_geom_point(geom);
    }

    // Static constructor methods
    static Ref<OCGDGeom_CartesianPoint> create_from_pnt(Ref<ocgd_gp_Pnt> p) {
        if (p.is_null()) return Ref<OCGDGeom_CartesianPoint>();
        Ref<OCGDGeom_CartesianPoint> result;
        result.instantiate();
        result->geom_cartesian_point = new Geom_CartesianPoint(p->get_pnt());
        result->set_geom_point(result->geom_cartesian_point);
        return result;
    }

    static Ref<OCGDGeom_CartesianPoint> create_from_coords(real_t x, real_t y, real_t z) {
        Ref<OCGDGeom_CartesianPoint> result;
        result.instantiate();
        result->geom_cartesian_point = new Geom_CartesianPoint(x, y, z);
        result->set_geom_point(result->geom_cartesian_point);
        return result;
    }

    static Ref<OCGDGeom_CartesianPoint> create_from_vector3(Vector3 v) {
        Ref<OCGDGeom_CartesianPoint> result;
        result.instantiate();
        result->geom_cartesian_point = new Geom_CartesianPoint(v.x, v.y, v.z);
        result->set_geom_point(result->geom_cartesian_point);
        return result;
    }

    Handle(Geom_CartesianPoint) get_geom_cartesian_point() const { return geom_cartesian_point; }
    void set_geom_cartesian_point(const Handle(Geom_CartesianPoint)& geom) { 
        geom_cartesian_point = geom; 
        set_geom_point(geom);
    }

    // Coordinate setting methods
    void set_coord(real_t x, real_t y, real_t z) {
        if (geom_cartesian_point.IsNull()) return;
        geom_cartesian_point->SetCoord(x, y, z);
    }

    void set_pnt(Ref<ocgd_gp_Pnt> p) {
        if (geom_cartesian_point.IsNull() || p.is_null()) return;
        geom_cartesian_point->SetPnt(p->get_pnt());
    }

    void set_x(real_t x) {
        if (geom_cartesian_point.IsNull()) return;
        geom_cartesian_point->SetX(x);
    }

    void set_y(real_t y) {
        if (geom_cartesian_point.IsNull()) return;
        geom_cartesian_point->SetY(y);
    }

    void set_z(real_t z) {
        if (geom_cartesian_point.IsNull()) return;
        geom_cartesian_point->SetZ(z);
    }

    // Transform method for concrete type
    void transform_cartesian(Ref<ocgd_gp_Trsf> t) {
        if (geom_cartesian_point.IsNull() || t.is_null()) return;
        geom_cartesian_point->Transform(t->get_gp_trsf());
    }

    // Copy method to return the correct type
    Ref<OCGDGeom_CartesianPoint> copy_cartesian() {
        if (geom_cartesian_point.IsNull()) return Ref<OCGDGeom_CartesianPoint>();
        Handle(Geom_Geometry) result = geom_cartesian_point->Copy();
        Handle(Geom_CartesianPoint) cart_result = Handle(Geom_CartesianPoint)::DownCast(result);
        if (cart_result.IsNull()) return Ref<OCGDGeom_CartesianPoint>();
        
        Ref<OCGDGeom_CartesianPoint> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_cartesian_point(cart_result);
        return wrapped;
    }

    String to_string() const {
        if (geom_cartesian_point.IsNull()) return "Null Geom_CartesianPoint";
        Vector3 coords = coord();
        return String("Geom_CartesianPoint(") + String::num(coords.x) + ", " + 
               String::num(coords.y) + ", " + String::num(coords.z) + ")";
    }
};