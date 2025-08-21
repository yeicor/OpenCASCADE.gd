#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Geom_Surface.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
#include <gp_GTrsf2d.hxx>
#include <GeomAbs_Shape.hxx>

#include "ocgd_Geom_Geometry.h"
#include "ocgd_gp_Trsf.h"
#include "ocgd_Geom_Curve.h"

using namespace godot;

class OCGDGeom_Surface : public OCGDGeom_Geometry {
    GDCLASS(OCGDGeom_Surface, OCGDGeom_Geometry)

protected:
    Handle(Geom_Surface) geom_surface;

public:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("u_reverse"), &OCGDGeom_Surface::u_reverse);
        ClassDB::bind_method(D_METHOD("u_reversed"), &OCGDGeom_Surface::u_reversed);
        ClassDB::bind_method(D_METHOD("u_reversed_parameter", "u"), &OCGDGeom_Surface::u_reversed_parameter);
        ClassDB::bind_method(D_METHOD("v_reverse"), &OCGDGeom_Surface::v_reverse);
        ClassDB::bind_method(D_METHOD("v_reversed"), &OCGDGeom_Surface::v_reversed);
        ClassDB::bind_method(D_METHOD("v_reversed_parameter", "v"), &OCGDGeom_Surface::v_reversed_parameter);
        ClassDB::bind_method(D_METHOD("transform_parameters", "u", "v", "t"), &OCGDGeom_Surface::transform_parameters);
        ClassDB::bind_method(D_METHOD("parametric_transformation", "t"), &OCGDGeom_Surface::parametric_transformation);
        ClassDB::bind_method(D_METHOD("bounds"), &OCGDGeom_Surface::bounds);
        ClassDB::bind_method(D_METHOD("is_u_closed"), &OCGDGeom_Surface::is_u_closed);
        ClassDB::bind_method(D_METHOD("is_v_closed"), &OCGDGeom_Surface::is_v_closed);
        ClassDB::bind_method(D_METHOD("is_u_periodic"), &OCGDGeom_Surface::is_u_periodic);
        ClassDB::bind_method(D_METHOD("u_period"), &OCGDGeom_Surface::u_period);
        ClassDB::bind_method(D_METHOD("is_v_periodic"), &OCGDGeom_Surface::is_v_periodic);
        ClassDB::bind_method(D_METHOD("v_period"), &OCGDGeom_Surface::v_period);
        ClassDB::bind_method(D_METHOD("u_iso", "u"), &OCGDGeom_Surface::u_iso);
        ClassDB::bind_method(D_METHOD("v_iso", "v"), &OCGDGeom_Surface::v_iso);
        ClassDB::bind_method(D_METHOD("continuity"), &OCGDGeom_Surface::continuity);
        ClassDB::bind_method(D_METHOD("is_cn_u", "n"), &OCGDGeom_Surface::is_cn_u);
        ClassDB::bind_method(D_METHOD("is_cn_v", "n"), &OCGDGeom_Surface::is_cn_v);
        ClassDB::bind_method(D_METHOD("d0", "u", "v"), &OCGDGeom_Surface::d0);
        ClassDB::bind_method(D_METHOD("d1", "u", "v"), &OCGDGeom_Surface::d1);
        ClassDB::bind_method(D_METHOD("d2", "u", "v"), &OCGDGeom_Surface::d2);
        ClassDB::bind_method(D_METHOD("d3", "u", "v"), &OCGDGeom_Surface::d3);
        ClassDB::bind_method(D_METHOD("dn", "u", "v", "nu", "nv"), &OCGDGeom_Surface::dn);
        ClassDB::bind_method(D_METHOD("value", "u", "v"), &OCGDGeom_Surface::value);
    }

    OCGDGeom_Surface() {
        // This is an abstract class, so no direct instantiation
    }

    OCGDGeom_Surface(const Handle(Geom_Surface)& geom) : geom_surface(geom) {
        set_geom(geom);
    }

    Handle(Geom_Surface) get_geom_surface() const { return geom_surface; }
    void set_geom_surface(const Handle(Geom_Surface)& geom) { 
        geom_surface = geom; 
        set_geom(geom);
    }

    // Virtual methods - these will be overridden in concrete classes
    virtual void u_reverse() {
        if (geom_surface.IsNull()) return;
        geom_surface->UReverse();
    }

    Ref<OCGDGeom_Surface> u_reversed() const {
        if (geom_surface.IsNull()) return Ref<OCGDGeom_Surface>();
        Handle(Geom_Surface) result = geom_surface->UReversed();
        Ref<OCGDGeom_Surface> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_surface(result);
        return wrapped;
    }

    virtual real_t u_reversed_parameter(real_t u) const {
        if (geom_surface.IsNull()) return 0.0;
        return geom_surface->UReversedParameter(u);
    }

    virtual void v_reverse() {
        if (geom_surface.IsNull()) return;
        geom_surface->VReverse();
    }

    Ref<OCGDGeom_Surface> v_reversed() const {
        if (geom_surface.IsNull()) return Ref<OCGDGeom_Surface>();
        Handle(Geom_Surface) result = geom_surface->VReversed();
        Ref<OCGDGeom_Surface> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_surface(result);
        return wrapped;
    }

    virtual real_t v_reversed_parameter(real_t v) const {
        if (geom_surface.IsNull()) return 0.0;
        return geom_surface->VReversedParameter(v);
    }

    Vector2 transform_parameters(real_t u, real_t v, Ref<ocgd_gp_Trsf> t) const {
        if (geom_surface.IsNull() || t.is_null()) return Vector2(u, v);
        Standard_Real new_u = u, new_v = v;
        geom_surface->TransformParameters(new_u, new_v, t->get_gp_trsf());
        return Vector2(new_u, new_v);
    }

    Dictionary parametric_transformation(Ref<ocgd_gp_Trsf> t) const {
        Dictionary result;
        if (geom_surface.IsNull() || t.is_null()) {
            result["identity"] = true;
            return result;
        }
        // Note: gp_GTrsf2d is a 2D generalized transformation
        // For simplicity, we'll just return an identity marker
        result["identity"] = true;
        return result;
    }

    Dictionary bounds() const {
        Dictionary result;
        if (geom_surface.IsNull()) {
            result["u1"] = 0.0;
            result["u2"] = 0.0;
            result["v1"] = 0.0;
            result["v2"] = 0.0;
            return result;
        }
        Standard_Real u1, u2, v1, v2;
        geom_surface->Bounds(u1, u2, v1, v2);
        result["u1"] = u1;
        result["u2"] = u2;
        result["v1"] = v1;
        result["v2"] = v2;
        return result;
    }

    virtual bool is_u_closed() const {
        if (geom_surface.IsNull()) return false;
        return geom_surface->IsUClosed();
    }

    virtual bool is_v_closed() const {
        if (geom_surface.IsNull()) return false;
        return geom_surface->IsVClosed();
    }

    virtual bool is_u_periodic() const {
        if (geom_surface.IsNull()) return false;
        return geom_surface->IsUPeriodic();
    }

    virtual real_t u_period() const {
        if (geom_surface.IsNull()) return 0.0;
        return geom_surface->UPeriod();
    }

    virtual bool is_v_periodic() const {
        if (geom_surface.IsNull()) return false;
        return geom_surface->IsVPeriodic();
    }

    virtual real_t v_period() const {
        if (geom_surface.IsNull()) return 0.0;
        return geom_surface->VPeriod();
    }

    Ref<OCGDGeom_Curve> u_iso(real_t u) const {
        if (geom_surface.IsNull()) return Ref<OCGDGeom_Curve>();
        Handle(Geom_Curve) result = geom_surface->UIso(u);
        Ref<OCGDGeom_Curve> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_curve(result);
        return wrapped;
    }

    Ref<OCGDGeom_Curve> v_iso(real_t v) const {
        if (geom_surface.IsNull()) return Ref<OCGDGeom_Curve>();
        Handle(Geom_Curve) result = geom_surface->VIso(v);
        Ref<OCGDGeom_Curve> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_curve(result);
        return wrapped;
    }

    virtual int continuity() const {
        if (geom_surface.IsNull()) return 0;
        return static_cast<int>(geom_surface->Continuity());
    }

    virtual bool is_cn_u(int n) const {
        if (geom_surface.IsNull()) return false;
        return geom_surface->IsCNu(n);
    }

    virtual bool is_cn_v(int n) const {
        if (geom_surface.IsNull()) return false;
        return geom_surface->IsCNv(n);
    }

    // Point evaluation at parameters u, v
    Vector3 d0(real_t u, real_t v) const {
        if (geom_surface.IsNull()) return Vector3();
        gp_Pnt p;
        geom_surface->D0(u, v, p);
        return Vector3(p.X(), p.Y(), p.Z());
    }

    // Point and first derivatives at parameters u, v
    Dictionary d1(real_t u, real_t v) const {
        Dictionary result;
        if (geom_surface.IsNull()) {
            result["point"] = Vector3();
            result["d1u"] = Vector3();
            result["d1v"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec d1u, d1v;
        geom_surface->D1(u, v, p, d1u, d1v);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["d1u"] = Vector3(d1u.X(), d1u.Y(), d1u.Z());
        result["d1v"] = Vector3(d1v.X(), d1v.Y(), d1v.Z());
        return result;
    }

    // Point, first and second derivatives at parameters u, v
    Dictionary d2(real_t u, real_t v) const {
        Dictionary result;
        if (geom_surface.IsNull()) {
            result["point"] = Vector3();
            result["d1u"] = Vector3();
            result["d1v"] = Vector3();
            result["d2u"] = Vector3();
            result["d2v"] = Vector3();
            result["d2uv"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec d1u, d1v, d2u, d2v, d2uv;
        geom_surface->D2(u, v, p, d1u, d1v, d2u, d2v, d2uv);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["d1u"] = Vector3(d1u.X(), d1u.Y(), d1u.Z());
        result["d1v"] = Vector3(d1v.X(), d1v.Y(), d1v.Z());
        result["d2u"] = Vector3(d2u.X(), d2u.Y(), d2u.Z());
        result["d2v"] = Vector3(d2v.X(), d2v.Y(), d2v.Z());
        result["d2uv"] = Vector3(d2uv.X(), d2uv.Y(), d2uv.Z());
        return result;
    }

    // Point, first, second and third derivatives at parameters u, v
    Dictionary d3(real_t u, real_t v) const {
        Dictionary result;
        if (geom_surface.IsNull()) {
            result["point"] = Vector3();
            result["d1u"] = Vector3();
            result["d1v"] = Vector3();
            result["d2u"] = Vector3();
            result["d2v"] = Vector3();
            result["d2uv"] = Vector3();
            result["d3u"] = Vector3();
            result["d3v"] = Vector3();
            result["d3uuv"] = Vector3();
            result["d3uvv"] = Vector3();
            return result;
        }
        gp_Pnt p;
        gp_Vec d1u, d1v, d2u, d2v, d2uv, d3u, d3v, d3uuv, d3uvv;
        geom_surface->D3(u, v, p, d1u, d1v, d2u, d2v, d2uv, d3u, d3v, d3uuv, d3uvv);
        result["point"] = Vector3(p.X(), p.Y(), p.Z());
        result["d1u"] = Vector3(d1u.X(), d1u.Y(), d1u.Z());
        result["d1v"] = Vector3(d1v.X(), d1v.Y(), d1v.Z());
        result["d2u"] = Vector3(d2u.X(), d2u.Y(), d2u.Z());
        result["d2v"] = Vector3(d2v.X(), d2v.Y(), d2v.Z());
        result["d2uv"] = Vector3(d2uv.X(), d2uv.Y(), d2uv.Z());
        result["d3u"] = Vector3(d3u.X(), d3u.Y(), d3u.Z());
        result["d3v"] = Vector3(d3v.X(), d3v.Y(), d3v.Z());
        result["d3uuv"] = Vector3(d3uuv.X(), d3uuv.Y(), d3uuv.Z());
        result["d3uvv"] = Vector3(d3uvv.X(), d3uvv.Y(), d3uvv.Z());
        return result;
    }

    // Nth derivative at parameters u, v
    Vector3 dn(real_t u, real_t v, int nu, int nv) const {
        if (geom_surface.IsNull() || nu < 0 || nv < 0 || (nu + nv) < 1) return Vector3();
        gp_Vec result = geom_surface->DN(u, v, nu, nv);
        return Vector3(result.X(), result.Y(), result.Z());
    }

    // Point at parameters u, v (convenience method)
    Vector3 value(real_t u, real_t v) const {
        if (geom_surface.IsNull()) return Vector3();
        gp_Pnt p = geom_surface->Value(u, v);
        return Vector3(p.X(), p.Y(), p.Z());
    }

    String to_string() const {
        if (geom_surface.IsNull()) return "Null Geom_Surface";
        Dictionary b = bounds();
        return String("Geom_Surface [U: ") + String::num(b["u1"]) + " to " + String::num(b["u2"]) +
               ", V: " + String::num(b["v1"]) + " to " + String::num(b["v2"]) + "]";
    }
};