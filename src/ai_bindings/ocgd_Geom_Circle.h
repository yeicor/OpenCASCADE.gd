#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Geom_Circle.hxx>
#include <gp_Circ.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>

#include "ocgd_Geom_Conic.h"
#include "ocgd_gp_Circ.h"
#include "ocgd_gp_Ax2.h"
#include "ocgd_gp_Trsf.h"

using namespace godot;

class OCGDGeom_Circle : public OCGDGeom_Conic {
    GDCLASS(OCGDGeom_Circle, OCGDGeom_Conic)

protected:
    Handle(Geom_Circle) geom_circle;

public:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("set_circ", "c"), &OCGDGeom_Circle::set_circ);
        ClassDB::bind_method(D_METHOD("set_radius", "r"), &OCGDGeom_Circle::set_radius);
        ClassDB::bind_method(D_METHOD("circ"), &OCGDGeom_Circle::circ);
        ClassDB::bind_method(D_METHOD("radius"), &OCGDGeom_Circle::radius);
        ClassDB::bind_method(D_METHOD("transform_circle", "t"), &OCGDGeom_Circle::transform_circle);
        ClassDB::bind_method(D_METHOD("copy_circle"), &OCGDGeom_Circle::copy_circle);
        ClassDB::bind_method(D_METHOD("evaluate_point", "u"), &OCGDGeom_Circle::evaluate_point);
        ClassDB::bind_method(D_METHOD("evaluate_with_derivative1", "u"), &OCGDGeom_Circle::evaluate_with_derivative1);
        ClassDB::bind_method(D_METHOD("evaluate_with_derivative2", "u"), &OCGDGeom_Circle::evaluate_with_derivative2);
        ClassDB::bind_method(D_METHOD("evaluate_with_derivative3", "u"), &OCGDGeom_Circle::evaluate_with_derivative3);
        ClassDB::bind_method(D_METHOD("evaluate_nth_derivative", "u", "n"), &OCGDGeom_Circle::evaluate_nth_derivative);
        ClassDB::bind_method(D_METHOD("to_string"), &OCGDGeom_Circle::to_string);
        
        // Static constructors
        ClassDB::bind_static_method("OCGDGeom_Circle", D_METHOD("create_from_circ", "c"), &OCGDGeom_Circle::create_from_circ);
        ClassDB::bind_static_method("OCGDGeom_Circle", D_METHOD("create_from_axis_radius", "a2", "radius"), &OCGDGeom_Circle::create_from_axis_radius);
    }

    OCGDGeom_Circle() {
        // Default constructor creates circle with radius 1.0 in XY plane at origin
        geom_circle = new Geom_Circle(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), 1.0);
        set_geom_conic(geom_circle);
    }

    OCGDGeom_Circle(const Handle(Geom_Circle)& geom) : geom_circle(geom) {
        set_geom_conic(geom);
    }

    // Static constructor methods
    static Ref<OCGDGeom_Circle> create_from_circ(Ref<ocgd_gp_Circ> c) {
        if (c.is_null()) return Ref<OCGDGeom_Circle>();
        Ref<OCGDGeom_Circle> result;
        result.instantiate();
        result->geom_circle = new Geom_Circle(c->get_gp_circ());
        result->set_geom_conic(result->geom_circle);
        return result;
    }

    static Ref<OCGDGeom_Circle> create_from_axis_radius(Ref<ocgd_gp_Ax2> a2, real_t radius) {
        if (a2.is_null() || radius < 0.0) return Ref<OCGDGeom_Circle>();
        Ref<OCGDGeom_Circle> result;
        result.instantiate();
        result->geom_circle = new Geom_Circle(a2->get_ax(), radius);
        result->set_geom_conic(result->geom_circle);
        return result;
    }

    Handle(Geom_Circle) get_geom_circle() const { return geom_circle; }
    void set_geom_circle(const Handle(Geom_Circle)& geom) { 
        geom_circle = geom; 
        set_geom_conic(geom);
    }

    // Circle-specific methods
    void set_circ(Ref<ocgd_gp_Circ> c) {
        if (geom_circle.IsNull() || c.is_null()) return;
        geom_circle->SetCirc(c->get_gp_circ());
    }

    void set_radius(real_t r) {
        if (geom_circle.IsNull() || r < 0.0) return;
        geom_circle->SetRadius(r);
    }

    Ref<ocgd_gp_Circ> circ() const {
        if (geom_circle.IsNull()) return Ref<ocgd_gp_Circ>();
        gp_Circ result = geom_circle->Circ();
        Ref<ocgd_gp_Circ> wrapped;
        wrapped.instantiate();
        wrapped->set_gp_circ(result);
        return wrapped;
    }

    real_t radius() const {
        if (geom_circle.IsNull()) return 0.0;
        return geom_circle->Radius();
    }

    // Override curve methods for circle-specific behavior
    real_t reversed_parameter(real_t u) const override {
        if (geom_circle.IsNull()) return 0.0;
        return geom_circle->ReversedParameter(u);
    }

    real_t first_parameter() const override {
        return 0.0; // Circles start at parameter 0
    }

    real_t last_parameter() const override {
        return 2.0 * M_PI; // Circles end at 2*PI
    }

    bool is_closed() const override {
        return true; // Circles are always closed
    }

    bool is_periodic() const override {
        return true; // Circles are always periodic
    }

    real_t period() const override {
        return 2.0 * M_PI; // Period is 2*PI
    }

    // Transform method for concrete type
    void transform_circle(Ref<ocgd_gp_Trsf> t) {
        if (geom_circle.IsNull() || t.is_null()) return;
        geom_circle->Transform(t->get_gp_trsf());
    }

    // Point and derivative evaluation for circle
    Vector3 evaluate_point(real_t u) const {
        if (geom_circle.IsNull()) return Vector3();
        gp_Pnt p;
        geom_circle->D0(u, p);
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Dictionary evaluate_with_derivative1(real_t u) const {
        Dictionary result;
        if (geom_circle.IsNull()) {
            result["point"] = Vector3();
            result["derivative1"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec v1;
        geom_circle->D1(u, p, v1);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["derivative1"] = Vector3(v1.X(), v1.Y(), v1.Z());
        return result;
    }

    Dictionary evaluate_with_derivative2(real_t u) const {
        Dictionary result;
        if (geom_circle.IsNull()) {
            result["point"] = Vector3();
            result["derivative1"] = Vector3();
            result["derivative2"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec v1, v2;
        geom_circle->D2(u, p, v1, v2);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["derivative1"] = Vector3(v1.X(), v1.Y(), v1.Z());
        result["derivative2"] = Vector3(v2.X(), v2.Y(), v2.Z());
        return result;
    }

    Dictionary evaluate_with_derivative3(real_t u) const {
        Dictionary result;
        if (geom_circle.IsNull()) {
            result["point"] = Vector3();
            result["derivative1"] = Vector3();
            result["derivative2"] = Vector3();
            result["derivative3"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec v1, v2, v3;
        geom_circle->D3(u, p, v1, v2, v3);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["derivative1"] = Vector3(v1.X(), v1.Y(), v1.Z());
        result["derivative2"] = Vector3(v2.X(), v2.Y(), v2.Z());
        result["derivative3"] = Vector3(v3.X(), v3.Y(), v3.Z());
        return result;
    }

    Vector3 evaluate_nth_derivative(real_t u, int n) const {
        if (geom_circle.IsNull() || n < 1) return Vector3();
        gp_Vec result = geom_circle->DN(u, n);
        return Vector3(result.X(), result.Y(), result.Z());
    }

    // Copy method to return the correct type
    Ref<OCGDGeom_Circle> copy_circle() {
        if (geom_circle.IsNull()) return Ref<OCGDGeom_Circle>();
        Handle(Geom_Geometry) result = geom_circle->Copy();
        Handle(Geom_Circle) circle_result = Handle(Geom_Circle)::DownCast(result);
        if (circle_result.IsNull()) return Ref<OCGDGeom_Circle>();
        
        Ref<OCGDGeom_Circle> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_circle(circle_result);
        return wrapped;
    }

    String to_string() const {
        if (geom_circle.IsNull()) return "Null Geom_Circle";
        return String("Geom_Circle(radius=") + String::num(radius()) + ")";
    }
};