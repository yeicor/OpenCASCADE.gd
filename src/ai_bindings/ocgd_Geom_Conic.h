#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Geom_Conic.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <GeomAbs_Shape.hxx>

#include "ocgd_Geom_Curve.h"
#include "ocgd_gp_Ax1.h"
#include "ocgd_gp_Ax2.h"
#include "ocgd_gp_Pnt.h"

using namespace godot;

class OCGDGeom_Conic : public OCGDGeom_Curve {
    GDCLASS(OCGDGeom_Conic, OCGDGeom_Curve)

protected:
    Handle(Geom_Conic) geom_conic;

public:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("set_axis", "a1"), &OCGDGeom_Conic::set_axis);
        ClassDB::bind_method(D_METHOD("set_location", "p"), &OCGDGeom_Conic::set_location);
        ClassDB::bind_method(D_METHOD("set_position", "a2"), &OCGDGeom_Conic::set_position);
        ClassDB::bind_method(D_METHOD("axis"), &OCGDGeom_Conic::axis);
        ClassDB::bind_method(D_METHOD("location"), &OCGDGeom_Conic::location);
        ClassDB::bind_method(D_METHOD("position"), &OCGDGeom_Conic::position);
        ClassDB::bind_method(D_METHOD("eccentricity"), &OCGDGeom_Conic::eccentricity);
        ClassDB::bind_method(D_METHOD("x_axis"), &OCGDGeom_Conic::x_axis);
        ClassDB::bind_method(D_METHOD("y_axis"), &OCGDGeom_Conic::y_axis);
    }

    OCGDGeom_Conic() {
        // This is an abstract class, so no direct instantiation
    }

    OCGDGeom_Conic(const Handle(Geom_Conic)& geom) : geom_conic(geom) {
        set_geom_curve(geom);
    }

    Handle(Geom_Conic) get_geom_conic() const { return geom_conic; }
    void set_geom_conic(const Handle(Geom_Conic)& geom) { 
        geom_conic = geom; 
        set_geom_curve(geom);
    }

    // Conic-specific methods
    void set_axis(Ref<ocgd_gp_Ax1> a1) {
        if (geom_conic.IsNull() || a1.is_null()) return;
        geom_conic->SetAxis(a1->get_ax());
    }

    void set_location(Ref<ocgd_gp_Pnt> p) {
        if (geom_conic.IsNull() || p.is_null()) return;
        geom_conic->SetLocation(p->get_pnt());
    }

    void set_position(Ref<ocgd_gp_Ax2> a2) {
        if (geom_conic.IsNull() || a2.is_null()) return;
        geom_conic->SetPosition(a2->get_ax());
    }

    Ref<ocgd_gp_Ax1> axis() const {
        if (geom_conic.IsNull()) return Ref<ocgd_gp_Ax1>();
        const gp_Ax1& result = geom_conic->Axis();
        Ref<ocgd_gp_Ax1> wrapped;
        wrapped.instantiate();
        wrapped->get_ax() = result;
        return wrapped;
    }

    Ref<ocgd_gp_Pnt> location() const {
        if (geom_conic.IsNull()) return Ref<ocgd_gp_Pnt>();
        const gp_Pnt& result = geom_conic->Location();
        Ref<ocgd_gp_Pnt> wrapped;
        wrapped.instantiate();
        wrapped->get_pnt() = result;
        return wrapped;
    }

    Ref<ocgd_gp_Ax2> position() const {
        if (geom_conic.IsNull()) return Ref<ocgd_gp_Ax2>();
        const gp_Ax2& result = geom_conic->Position();
        Ref<ocgd_gp_Ax2> wrapped;
        wrapped.instantiate();
        wrapped->get_ax() = result;
        return wrapped;
    }

    real_t eccentricity() const {
        if (geom_conic.IsNull()) return 0.0;
        return geom_conic->Eccentricity();
    }

    Ref<ocgd_gp_Ax1> x_axis() const {
        if (geom_conic.IsNull()) return Ref<ocgd_gp_Ax1>();
        const gp_Ax1& result = geom_conic->XAxis();
        Ref<ocgd_gp_Ax1> wrapped;
        wrapped.instantiate();
        wrapped->get_ax() = result;
        return wrapped;
    }

    Ref<ocgd_gp_Ax1> y_axis() const {
        if (geom_conic.IsNull()) return Ref<ocgd_gp_Ax1>();
        const gp_Ax1& result = geom_conic->YAxis();
        Ref<ocgd_gp_Ax1> wrapped;
        wrapped.instantiate();
        wrapped->get_ax() = result;
        return wrapped;
    }

    // Override curve methods that are common to all conics
    bool is_closed() const override {
        // Most conics are closed except parabola and hyperbola (which have infinite extent)
        if (geom_conic.IsNull()) return false;
        return geom_conic->IsClosed();
    }

    bool is_periodic() const override {
        if (geom_conic.IsNull()) return false;
        return geom_conic->IsPeriodic();
    }

    int continuity() const override {
        if (geom_conic.IsNull()) return 0;
        return static_cast<int>(geom_conic->Continuity());
    }

    bool is_cn(int n) const override {
        if (geom_conic.IsNull()) return false;
        return geom_conic->IsCN(n);
    }

    real_t first_parameter() const override {
        if (geom_conic.IsNull()) return 0.0;
        return geom_conic->FirstParameter();
    }

    real_t last_parameter() const override {
        if (geom_conic.IsNull()) return 0.0;
        return geom_conic->LastParameter();
    }

    String to_string() const {
        if (geom_conic.IsNull()) return "Null Geom_Conic";
        return String("Geom_Conic [") + String::num(first_parameter()) + ", " + 
               String::num(last_parameter()) + "]";
    }
};