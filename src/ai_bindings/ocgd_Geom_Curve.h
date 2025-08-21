#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Geom_Curve.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
#include <GeomAbs_Shape.hxx>

#include "ocgd_Geom_Geometry.h"
#include "ocgd_gp_Trsf.h"

using namespace godot;

class OCGDGeom_Curve : public OCGDGeom_Geometry {
    GDCLASS(OCGDGeom_Curve, OCGDGeom_Geometry)

protected:
    Handle(Geom_Curve) geom_curve;

public:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("reverse"), &OCGDGeom_Curve::reverse);
        ClassDB::bind_method(D_METHOD("reversed_parameter", "u"), &OCGDGeom_Curve::reversed_parameter);
        ClassDB::bind_method(D_METHOD("transformed_parameter", "u", "t"), &OCGDGeom_Curve::transformed_parameter);
        ClassDB::bind_method(D_METHOD("parametric_transformation", "t"), &OCGDGeom_Curve::parametric_transformation);
        ClassDB::bind_method(D_METHOD("reversed"), &OCGDGeom_Curve::reversed);
        ClassDB::bind_method(D_METHOD("first_parameter"), &OCGDGeom_Curve::first_parameter);
        ClassDB::bind_method(D_METHOD("last_parameter"), &OCGDGeom_Curve::last_parameter);
        ClassDB::bind_method(D_METHOD("is_closed"), &OCGDGeom_Curve::is_closed);
        ClassDB::bind_method(D_METHOD("is_periodic"), &OCGDGeom_Curve::is_periodic);
        ClassDB::bind_method(D_METHOD("period"), &OCGDGeom_Curve::period);
        ClassDB::bind_method(D_METHOD("continuity"), &OCGDGeom_Curve::continuity);
        ClassDB::bind_method(D_METHOD("is_cn", "n"), &OCGDGeom_Curve::is_cn);
        ClassDB::bind_method(D_METHOD("d0", "u"), &OCGDGeom_Curve::d0);
        ClassDB::bind_method(D_METHOD("d1", "u"), &OCGDGeom_Curve::d1);
        ClassDB::bind_method(D_METHOD("d2", "u"), &OCGDGeom_Curve::d2);
        ClassDB::bind_method(D_METHOD("d3", "u"), &OCGDGeom_Curve::d3);
        ClassDB::bind_method(D_METHOD("dn", "u", "n"), &OCGDGeom_Curve::dn);
        ClassDB::bind_method(D_METHOD("value", "u"), &OCGDGeom_Curve::value);
    }

    OCGDGeom_Curve() {
        // This is an abstract class, so no direct instantiation
    }

    OCGDGeom_Curve(const Handle(Geom_Curve)& geom) : geom_curve(geom) {
        set_geom(geom);
    }

    Handle(Geom_Curve) get_geom_curve() const { return geom_curve; }
    void set_geom_curve(const Handle(Geom_Curve)& geom) { 
        geom_curve = geom; 
        set_geom(geom);
    }

    // Virtual methods - these will be overridden in concrete classes
    virtual void reverse() {
        if (geom_curve.IsNull()) return;
        geom_curve->Reverse();
    }

    virtual real_t reversed_parameter(real_t u) const {
        if (geom_curve.IsNull()) return 0.0;
        return geom_curve->ReversedParameter(u);
    }

    virtual real_t transformed_parameter(real_t u, Ref<ocgd_gp_Trsf> t) const {
        if (geom_curve.IsNull() || t.is_null()) return u;
        return geom_curve->TransformedParameter(u, t->get_gp_trsf());
    }

    virtual real_t parametric_transformation(Ref<ocgd_gp_Trsf> t) const {
        if (geom_curve.IsNull() || t.is_null()) return 1.0;
        return geom_curve->ParametricTransformation(t->get_gp_trsf());
    }

    Ref<OCGDGeom_Curve> reversed() const {
        if (geom_curve.IsNull()) return Ref<OCGDGeom_Curve>();
        Handle(Geom_Curve) result = geom_curve->Reversed();
        Ref<OCGDGeom_Curve> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_curve(result);
        return wrapped;
    }

    virtual real_t first_parameter() const {
        if (geom_curve.IsNull()) return 0.0;
        return geom_curve->FirstParameter();
    }

    virtual real_t last_parameter() const {
        if (geom_curve.IsNull()) return 0.0;
        return geom_curve->LastParameter();
    }

    virtual bool is_closed() const {
        if (geom_curve.IsNull()) return false;
        return geom_curve->IsClosed();
    }

    virtual bool is_periodic() const {
        if (geom_curve.IsNull()) return false;
        return geom_curve->IsPeriodic();
    }

    virtual real_t period() const {
        if (geom_curve.IsNull()) return 0.0;
        return geom_curve->Period();
    }

    virtual int continuity() const {
        if (geom_curve.IsNull()) return 0;
        return static_cast<int>(geom_curve->Continuity());
    }

    virtual bool is_cn(int n) const {
        if (geom_curve.IsNull()) return false;
        return geom_curve->IsCN(n);
    }

    // Point evaluation at parameter u
    Vector3 d0(real_t u) const {
        if (geom_curve.IsNull()) return Vector3();
        gp_Pnt p;
        geom_curve->D0(u, p);
        return Vector3(p.X(), p.Y(), p.Z());
    }

    // Point and first derivative at parameter u
    Dictionary d1(real_t u) const {
        Dictionary result;
        if (geom_curve.IsNull()) {
            result["point"] = Vector3();
            result["derivative1"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec v1;
        geom_curve->D1(u, p, v1);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["derivative1"] = Vector3(v1.X(), v1.Y(), v1.Z());
        return result;
    }

    // Point, first and second derivatives at parameter u
    Dictionary d2(real_t u) const {
        Dictionary result;
        if (geom_curve.IsNull()) {
            result["point"] = Vector3();
            result["derivative1"] = Vector3();
            result["derivative2"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec v1, v2;
        geom_curve->D2(u, p, v1, v2);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["derivative1"] = Vector3(v1.X(), v1.Y(), v1.Z());
        result["derivative2"] = Vector3(v2.X(), v2.Y(), v2.Z());
        return result;
    }

    // Point, first, second and third derivatives at parameter u
    Dictionary d3(real_t u) const {
        Dictionary result;
        if (geom_curve.IsNull()) {
            result["point"] = Vector3();
            result["derivative1"] = Vector3();
            result["derivative2"] = Vector3();
            result["derivative3"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec v1, v2, v3;
        geom_curve->D3(u, p, v1, v2, v3);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["derivative1"] = Vector3(v1.X(), v1.Y(), v1.Z());
        result["derivative2"] = Vector3(v2.X(), v2.Y(), v2.Z());
        result["derivative3"] = Vector3(v3.X(), v3.Y(), v3.Z());
        return result;
    }

    // Nth derivative at parameter u
    Vector3 dn(real_t u, int n) const {
        if (geom_curve.IsNull() || n < 1) return Vector3();
        gp_Vec result = geom_curve->DN(u, n);
        return Vector3(result.X(), result.Y(), result.Z());
    }

    // Point at parameter u (convenience method)
    Vector3 value(real_t u) const {
        if (geom_curve.IsNull()) return Vector3();
        gp_Pnt p = geom_curve->Value(u);
        return Vector3(p.X(), p.Y(), p.Z());
    }

    String to_string() const {
        if (geom_curve.IsNull()) return "Null Geom_Curve";
        return String("Geom_Curve [") + String::num(first_parameter()) + ", " + 
               String::num(last_parameter()) + "]";
    }
};