#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Geom_Line.hxx>
#include <gp_Lin.hxx>
#include <gp_Ax1.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
#include <GeomAbs_Shape.hxx>

#include "ocgd_Geom_Curve.h"
#include "ocgd_gp_Lin.h"
#include "ocgd_gp_Ax1.h"
#include "ocgd_gp_Pnt.h"
#include "ocgd_gp_Dir.h"
#include "ocgd_gp_Vec.h"
#include "ocgd_gp_Trsf.h"

using namespace godot;

class OCGDGeom_Line : public OCGDGeom_Curve {
    GDCLASS(OCGDGeom_Line, OCGDGeom_Curve)

protected:
    Handle(Geom_Line) geom_line;

public:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("set_lin", "l"), &OCGDGeom_Line::set_lin);
        ClassDB::bind_method(D_METHOD("set_direction", "v"), &OCGDGeom_Line::set_direction);
        ClassDB::bind_method(D_METHOD("set_location", "p"), &OCGDGeom_Line::set_location);
        ClassDB::bind_method(D_METHOD("set_position", "a1"), &OCGDGeom_Line::set_position);
        ClassDB::bind_method(D_METHOD("lin"), &OCGDGeom_Line::lin);
        ClassDB::bind_method(D_METHOD("position"), &OCGDGeom_Line::position);
        ClassDB::bind_method(D_METHOD("transform_line", "t"), &OCGDGeom_Line::transform_line);
        ClassDB::bind_method(D_METHOD("evaluate_point", "u"), &OCGDGeom_Line::evaluate_point);
        ClassDB::bind_method(D_METHOD("evaluate_with_derivative1", "u"), &OCGDGeom_Line::evaluate_with_derivative1);
        ClassDB::bind_method(D_METHOD("evaluate_with_derivative2", "u"), &OCGDGeom_Line::evaluate_with_derivative2);
        ClassDB::bind_method(D_METHOD("evaluate_with_derivative3", "u"), &OCGDGeom_Line::evaluate_with_derivative3);
        ClassDB::bind_method(D_METHOD("evaluate_nth_derivative", "u", "n"), &OCGDGeom_Line::evaluate_nth_derivative);
        ClassDB::bind_method(D_METHOD("copy_line"), &OCGDGeom_Line::copy_line);
        ClassDB::bind_method(D_METHOD("to_string"), &OCGDGeom_Line::to_string);
        
        // Static constructors
        ClassDB::bind_static_method("OCGDGeom_Line", D_METHOD("create_from_axis", "a1"), &OCGDGeom_Line::create_from_axis);
        ClassDB::bind_static_method("OCGDGeom_Line", D_METHOD("create_from_lin", "l"), &OCGDGeom_Line::create_from_lin);
        ClassDB::bind_static_method("OCGDGeom_Line", D_METHOD("create_from_point_dir", "p", "v"), &OCGDGeom_Line::create_from_point_dir);
    }

    OCGDGeom_Line() {
        // Default constructor creates line along Z-axis through origin
        geom_line = new Geom_Line(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)));
        set_geom_curve(geom_line);
    }

    OCGDGeom_Line(const Handle(Geom_Line)& geom) : geom_line(geom) {
        set_geom_curve(geom);
    }

    // Static constructor methods
    static Ref<OCGDGeom_Line> create_from_axis(Ref<ocgd_gp_Ax1> a1) {
        if (a1.is_null()) return Ref<OCGDGeom_Line>();
        Ref<OCGDGeom_Line> result;
        result.instantiate();
        result->geom_line = new Geom_Line(a1->get_ax());
        result->set_geom_curve(result->geom_line);
        return result;
    }

    static Ref<OCGDGeom_Line> create_from_lin(Ref<ocgd_gp_Lin> l) {
        if (l.is_null()) return Ref<OCGDGeom_Line>();
        Ref<OCGDGeom_Line> result;
        result.instantiate();
        result->geom_line = new Geom_Line(l->get_lin());
        result->set_geom_curve(result->geom_line);
        return result;
    }

    static Ref<OCGDGeom_Line> create_from_point_dir(Ref<ocgd_gp_Pnt> p, Ref<ocgd_gp_Dir> v) {
        if (p.is_null() || v.is_null()) return Ref<OCGDGeom_Line>();
        Ref<OCGDGeom_Line> result;
        result.instantiate();
        result->geom_line = new Geom_Line(p->get_pnt(), v->get_dir());
        result->set_geom_curve(result->geom_line);
        return result;
    }

    Handle(Geom_Line) get_geom_line() const { return geom_line; }
    void set_geom_line(const Handle(Geom_Line)& geom) { 
        geom_line = geom; 
        set_geom_curve(geom);
    }

    // Line-specific methods
    void set_lin(Ref<ocgd_gp_Lin> l) {
        if (geom_line.IsNull() || l.is_null()) return;
        geom_line->SetLin(l->get_lin());
    }

    void set_direction(Ref<ocgd_gp_Dir> v) {
        if (geom_line.IsNull() || v.is_null()) return;
        geom_line->SetDirection(v->get_dir());
    }

    void set_location(Ref<ocgd_gp_Pnt> p) {
        if (geom_line.IsNull() || p.is_null()) return;
        geom_line->SetLocation(p->get_pnt());
    }

    void set_position(Ref<ocgd_gp_Ax1> a1) {
        if (geom_line.IsNull() || a1.is_null()) return;
        geom_line->SetPosition(a1->get_ax());
    }

    Ref<ocgd_gp_Lin> lin() const {
        if (geom_line.IsNull()) return Ref<ocgd_gp_Lin>();
        gp_Lin result = geom_line->Lin();
        Ref<ocgd_gp_Lin> wrapped;
        wrapped.instantiate();
        wrapped->get_lin() = result;
        return wrapped;
    }

    Ref<ocgd_gp_Ax1> position() const {
        if (geom_line.IsNull()) return Ref<ocgd_gp_Ax1>();
        const gp_Ax1& result = geom_line->Position();
        Ref<ocgd_gp_Ax1> wrapped;
        wrapped.instantiate();
        wrapped->get_ax() = result;
        return wrapped;
    }

    // Override curve methods for line-specific behavior
    void reverse() override {
        if (geom_line.IsNull()) return;
        geom_line->Reverse();
    }

    real_t reversed_parameter(real_t u) const override {
        if (geom_line.IsNull()) return 0.0;
        return geom_line->ReversedParameter(u);
    }

    real_t first_parameter() const override {
        if (geom_line.IsNull()) return 0.0;
        return geom_line->FirstParameter();
    }

    real_t last_parameter() const override {
        if (geom_line.IsNull()) return 0.0;
        return geom_line->LastParameter();
    }

    bool is_closed() const override {
        return false; // Lines are never closed
    }

    bool is_periodic() const override {
        return false; // Lines are never periodic
    }

    int continuity() const override {
        return static_cast<int>(GeomAbs_CN); // Lines have infinite continuity
    }

    bool is_cn(int n) const override {
        return true; // Lines are CN for any N
    }

    real_t transformed_parameter(real_t u, Ref<ocgd_gp_Trsf> t) const override {
        if (geom_line.IsNull() || t.is_null()) return u;
        return geom_line->TransformedParameter(u, t->get_gp_trsf());
    }

    real_t parametric_transformation(Ref<ocgd_gp_Trsf> t) const override {
        if (geom_line.IsNull() || t.is_null()) return 1.0;
        return geom_line->ParametricTransformation(t->get_gp_trsf());
    }

    // Transform method for concrete type
    void transform_line(Ref<ocgd_gp_Trsf> t) {
        if (geom_line.IsNull() || t.is_null()) return;
        geom_line->Transform(t->get_gp_trsf());
    }

    // Point and derivative evaluation for line
    Vector3 evaluate_point(real_t u) const {
        if (geom_line.IsNull()) return Vector3();
        gp_Pnt p;
        geom_line->D0(u, p);
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Dictionary evaluate_with_derivative1(real_t u) const {
        Dictionary result;
        if (geom_line.IsNull()) {
            result["point"] = Vector3();
            result["derivative1"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec v1;
        geom_line->D1(u, p, v1);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["derivative1"] = Vector3(v1.X(), v1.Y(), v1.Z());
        return result;
    }

    Dictionary evaluate_with_derivative2(real_t u) const {
        Dictionary result;
        if (geom_line.IsNull()) {
            result["point"] = Vector3();
            result["derivative1"] = Vector3();
            result["derivative2"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec v1, v2;
        geom_line->D2(u, p, v1, v2);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["derivative1"] = Vector3(v1.X(), v1.Y(), v1.Z());
        result["derivative2"] = Vector3(v2.X(), v2.Y(), v2.Z()); // Will be zero vector for lines
        return result;
    }

    Dictionary evaluate_with_derivative3(real_t u) const {
        Dictionary result;
        if (geom_line.IsNull()) {
            result["point"] = Vector3();
            result["derivative1"] = Vector3();
            result["derivative2"] = Vector3();
            result["derivative3"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec v1, v2, v3;
        geom_line->D3(u, p, v1, v2, v3);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["derivative1"] = Vector3(v1.X(), v1.Y(), v1.Z());
        result["derivative2"] = Vector3(v2.X(), v2.Y(), v2.Z()); // Will be zero vector for lines
        result["derivative3"] = Vector3(v3.X(), v3.Y(), v3.Z()); // Will be zero vector for lines
        return result;
    }

    Vector3 evaluate_nth_derivative(real_t u, int n) const {
        if (geom_line.IsNull() || n < 1) return Vector3();
        gp_Vec result = geom_line->DN(u, n);
        return Vector3(result.X(), result.Y(), result.Z());
    }

    // Copy method to return the correct type
    Ref<OCGDGeom_Line> copy_line() {
        if (geom_line.IsNull()) return Ref<OCGDGeom_Line>();
        Handle(Geom_Geometry) result = geom_line->Copy();
        Handle(Geom_Line) line_result = Handle(Geom_Line)::DownCast(result);
        if (line_result.IsNull()) return Ref<OCGDGeom_Line>();
        
        Ref<OCGDGeom_Line> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_line(line_result);
        return wrapped;
    }

    String to_string() const {
        if (geom_line.IsNull()) return "Null Geom_Line";
        return String("Geom_Line [infinite line]");
    }
};