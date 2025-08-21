#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Geom_Point.hxx>
#include <gp_Pnt.hxx>

#include "ocgd_Geom_Geometry.h"
#include "ocgd_gp_Pnt.h"

using namespace godot;

class OCGDGeom_Point : public OCGDGeom_Geometry {
    GDCLASS(OCGDGeom_Point, OCGDGeom_Geometry)

protected:
    Handle(Geom_Point) geom_point;

public:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("coord"), &OCGDGeom_Point::coord);
        ClassDB::bind_method(D_METHOD("pnt"), &OCGDGeom_Point::pnt);
        ClassDB::bind_method(D_METHOD("x"), &OCGDGeom_Point::x);
        ClassDB::bind_method(D_METHOD("y"), &OCGDGeom_Point::y);
        ClassDB::bind_method(D_METHOD("z"), &OCGDGeom_Point::z);
        ClassDB::bind_method(D_METHOD("distance", "other"), &OCGDGeom_Point::distance);
        ClassDB::bind_method(D_METHOD("square_distance", "other"), &OCGDGeom_Point::square_distance);
    }

    OCGDGeom_Point() {
        // This is an abstract class, so no direct instantiation
    }

    OCGDGeom_Point(const Handle(Geom_Point)& geom) : geom_point(geom) {
        set_geom(geom);
    }

    Handle(Geom_Point) get_geom_point() const { return geom_point; }
    void set_geom_point(const Handle(Geom_Point)& geom) { 
        geom_point = geom; 
        set_geom(geom);
    }

    // Get coordinates as Vector3
    Vector3 coord() const {
        if (geom_point.IsNull()) return Vector3();
        Standard_Real x, y, z;
        geom_point->Coord(x, y, z);
        return Vector3(x, y, z);
    }

    // Get as gp_Pnt wrapped in ocgd_gp_Pnt
    Ref<ocgd_gp_Pnt> pnt() const {
        if (geom_point.IsNull()) return Ref<ocgd_gp_Pnt>();
        gp_Pnt result = geom_point->Pnt();
        Ref<ocgd_gp_Pnt> wrapped;
        wrapped.instantiate();
        wrapped->get_pnt() = result;
        return wrapped;
    }

    real_t x() const {
        if (geom_point.IsNull()) return 0.0;
        return geom_point->X();
    }

    real_t y() const {
        if (geom_point.IsNull()) return 0.0;
        return geom_point->Y();
    }

    real_t z() const {
        if (geom_point.IsNull()) return 0.0;
        return geom_point->Z();
    }

    real_t distance(Ref<OCGDGeom_Point> other) const {
        if (geom_point.IsNull() || other.is_null() || other->get_geom_point().IsNull()) {
            return 0.0;
        }
        return geom_point->Distance(other->get_geom_point());
    }

    real_t square_distance(Ref<OCGDGeom_Point> other) const {
        if (geom_point.IsNull() || other.is_null() || other->get_geom_point().IsNull()) {
            return 0.0;
        }
        return geom_point->SquareDistance(other->get_geom_point());
    }

    String to_string() const {
        if (geom_point.IsNull()) return "Null Geom_Point";
        Vector3 coords = coord();
        return String("Geom_Point(") + String::num(coords.x) + ", " + 
               String::num(coords.y) + ", " + String::num(coords.z) + ")";
    }
};