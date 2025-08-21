#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Geom_Geometry.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>

#include "ocgd_gp_Pnt.h"
#include "ocgd_gp_Ax1.h"
#include "ocgd_gp_Ax2.h"
#include "ocgd_gp_Vec.h"
#include "ocgd_gp_Trsf.h"

using namespace godot;

class OCGDGeom_Geometry : public RefCounted {
    GDCLASS(OCGDGeom_Geometry, RefCounted)

protected:
    Handle(Geom_Geometry) geom_geometry;

public:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("mirror_point", "p"), &OCGDGeom_Geometry::mirror_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "a1"), &OCGDGeom_Geometry::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirror_plane", "a2"), &OCGDGeom_Geometry::mirror_plane);
        ClassDB::bind_method(D_METHOD("rotate", "a1", "angle"), &OCGDGeom_Geometry::rotate);
        ClassDB::bind_method(D_METHOD("scale", "p", "s"), &OCGDGeom_Geometry::scale);
        ClassDB::bind_method(D_METHOD("translate_vector", "v"), &OCGDGeom_Geometry::translate_vector);
        ClassDB::bind_method(D_METHOD("translate_points", "p1", "p2"), &OCGDGeom_Geometry::translate_points);
        ClassDB::bind_method(D_METHOD("transform", "t"), &OCGDGeom_Geometry::transform);
        
        ClassDB::bind_method(D_METHOD("mirrored_point", "p"), &OCGDGeom_Geometry::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "a1"), &OCGDGeom_Geometry::mirrored_axis);
        ClassDB::bind_method(D_METHOD("mirrored_plane", "a2"), &OCGDGeom_Geometry::mirrored_plane);
        ClassDB::bind_method(D_METHOD("rotated", "a1", "angle"), &OCGDGeom_Geometry::rotated);
        ClassDB::bind_method(D_METHOD("scaled", "p", "s"), &OCGDGeom_Geometry::scaled);
        ClassDB::bind_method(D_METHOD("transformed", "t"), &OCGDGeom_Geometry::transformed);
        ClassDB::bind_method(D_METHOD("translated_vector", "v"), &OCGDGeom_Geometry::translated_vector);
        ClassDB::bind_method(D_METHOD("translated_points", "p1", "p2"), &OCGDGeom_Geometry::translated_points);
        
        ClassDB::bind_method(D_METHOD("copy"), &OCGDGeom_Geometry::copy);
    }

    OCGDGeom_Geometry() {
        // This is an abstract class, so no direct instantiation
    }

    OCGDGeom_Geometry(const Handle(Geom_Geometry)& geom) : geom_geometry(geom) {}

    Handle(Geom_Geometry) get_geom() const { return geom_geometry; }
    void set_geom(const Handle(Geom_Geometry)& geom) { geom_geometry = geom; }

    // Mirror transformations (in-place)
    void mirror_point(Ref<ocgd_gp_Pnt> p) {
        if (geom_geometry.IsNull() || p.is_null()) return;
        geom_geometry->Mirror(p->get_pnt());
    }

    void mirror_axis(Ref<ocgd_gp_Ax1> a1) {
        if (geom_geometry.IsNull() || a1.is_null()) return;
        geom_geometry->Mirror(a1->get_ax());
    }

    void mirror_plane(Ref<ocgd_gp_Ax2> a2) {
        if (geom_geometry.IsNull() || a2.is_null()) return;
        geom_geometry->Mirror(a2->get_ax());
    }

    void rotate(Ref<ocgd_gp_Ax1> a1, real_t angle) {
        if (geom_geometry.IsNull() || a1.is_null()) return;
        geom_geometry->Rotate(a1->get_ax(), angle);
    }

    void scale(Ref<ocgd_gp_Pnt> p, real_t s) {
        if (geom_geometry.IsNull() || p.is_null()) return;
        geom_geometry->Scale(p->get_pnt(), s);
    }

    void translate_vector(Ref<ocgd_gp_Vec> v) {
        if (geom_geometry.IsNull() || v.is_null()) return;
        geom_geometry->Translate(v->get_vec());
    }

    void translate_points(Ref<ocgd_gp_Pnt> p1, Ref<ocgd_gp_Pnt> p2) {
        if (geom_geometry.IsNull() || p1.is_null() || p2.is_null()) return;
        geom_geometry->Translate(p1->get_pnt(), p2->get_pnt());
    }

    void transform(Ref<ocgd_gp_Trsf> t) {
        if (geom_geometry.IsNull() || t.is_null()) return;
        geom_geometry->Transform(t->get_gp_trsf());
    }

    // Mirror transformations (return new object)
    Ref<OCGDGeom_Geometry> mirrored_point(Ref<ocgd_gp_Pnt> p) {
        if (geom_geometry.IsNull() || p.is_null()) return Ref<OCGDGeom_Geometry>();
        Handle(Geom_Geometry) result = geom_geometry->Mirrored(p->get_pnt());
        Ref<OCGDGeom_Geometry> wrapped;
        wrapped.instantiate();
        wrapped->set_geom(result);
        return wrapped;
    }

    Ref<OCGDGeom_Geometry> mirrored_axis(Ref<ocgd_gp_Ax1> a1) {
        if (geom_geometry.IsNull() || a1.is_null()) return Ref<OCGDGeom_Geometry>();
        Handle(Geom_Geometry) result = geom_geometry->Mirrored(a1->get_ax());
        Ref<OCGDGeom_Geometry> wrapped;
        wrapped.instantiate();
        wrapped->set_geom(result);
        return wrapped;
    }

    Ref<OCGDGeom_Geometry> mirrored_plane(Ref<ocgd_gp_Ax2> a2) {
        if (geom_geometry.IsNull() || a2.is_null()) return Ref<OCGDGeom_Geometry>();
        Handle(Geom_Geometry) result = geom_geometry->Mirrored(a2->get_ax());
        Ref<OCGDGeom_Geometry> wrapped;
        wrapped.instantiate();
        wrapped->set_geom(result);
        return wrapped;
    }

    Ref<OCGDGeom_Geometry> rotated(Ref<ocgd_gp_Ax1> a1, real_t angle) {
        if (geom_geometry.IsNull() || a1.is_null()) return Ref<OCGDGeom_Geometry>();
        Handle(Geom_Geometry) result = geom_geometry->Rotated(a1->get_ax(), angle);
        Ref<OCGDGeom_Geometry> wrapped;
        wrapped.instantiate();
        wrapped->set_geom(result);
        return wrapped;
    }

    Ref<OCGDGeom_Geometry> scaled(Ref<ocgd_gp_Pnt> p, real_t s) {
        if (geom_geometry.IsNull() || p.is_null()) return Ref<OCGDGeom_Geometry>();
        Handle(Geom_Geometry) result = geom_geometry->Scaled(p->get_pnt(), s);
        Ref<OCGDGeom_Geometry> wrapped;
        wrapped.instantiate();
        wrapped->set_geom(result);
        return wrapped;
    }

    Ref<OCGDGeom_Geometry> transformed(Ref<ocgd_gp_Trsf> t) {
        if (geom_geometry.IsNull() || t.is_null()) return Ref<OCGDGeom_Geometry>();
        Handle(Geom_Geometry) result = geom_geometry->Transformed(t->get_gp_trsf());
        Ref<OCGDGeom_Geometry> wrapped;
        wrapped.instantiate();
        wrapped->set_geom(result);
        return wrapped;
    }

    Ref<OCGDGeom_Geometry> translated_vector(Ref<ocgd_gp_Vec> v) {
        if (geom_geometry.IsNull() || v.is_null()) return Ref<OCGDGeom_Geometry>();
        Handle(Geom_Geometry) result = geom_geometry->Translated(v->get_vec());
        Ref<OCGDGeom_Geometry> wrapped;
        wrapped.instantiate();
        wrapped->set_geom(result);
        return wrapped;
    }

    Ref<OCGDGeom_Geometry> translated_points(Ref<ocgd_gp_Pnt> p1, Ref<ocgd_gp_Pnt> p2) {
        if (geom_geometry.IsNull() || p1.is_null() || p2.is_null()) return Ref<OCGDGeom_Geometry>();
        Handle(Geom_Geometry) result = geom_geometry->Translated(p1->get_pnt(), p2->get_pnt());
        Ref<OCGDGeom_Geometry> wrapped;
        wrapped.instantiate();
        wrapped->set_geom(result);
        return wrapped;
    }

    Ref<OCGDGeom_Geometry> copy() {
        if (geom_geometry.IsNull()) return Ref<OCGDGeom_Geometry>();
        Handle(Geom_Geometry) result = geom_geometry->Copy();
        Ref<OCGDGeom_Geometry> wrapped;
        wrapped.instantiate();
        wrapped->set_geom(result);
        return wrapped;
    }

    String to_string() const {
        if (geom_geometry.IsNull()) return "Null Geom_Geometry";
        return "Geom_Geometry";
    }
};