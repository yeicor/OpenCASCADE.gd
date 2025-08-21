#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Geom_Vector.hxx>
#include <gp_Vec.hxx>

#include "ocgd_Geom_Geometry.h"
#include "ocgd_gp_Vec.h"

using namespace godot;

class OCGDGeom_Vector : public OCGDGeom_Geometry {
    GDCLASS(OCGDGeom_Vector, OCGDGeom_Geometry)

protected:
    Handle(Geom_Vector) geom_vector;

public:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("reverse"), &OCGDGeom_Vector::reverse);
        ClassDB::bind_method(D_METHOD("reversed"), &OCGDGeom_Vector::reversed);
        ClassDB::bind_method(D_METHOD("angle", "other"), &OCGDGeom_Vector::angle);
        ClassDB::bind_method(D_METHOD("angle_with_ref", "other", "vref"), &OCGDGeom_Vector::angle_with_ref);
        ClassDB::bind_method(D_METHOD("coord"), &OCGDGeom_Vector::coord);
        ClassDB::bind_method(D_METHOD("magnitude"), &OCGDGeom_Vector::magnitude);
        ClassDB::bind_method(D_METHOD("square_magnitude"), &OCGDGeom_Vector::square_magnitude);
        ClassDB::bind_method(D_METHOD("x"), &OCGDGeom_Vector::x);
        ClassDB::bind_method(D_METHOD("y"), &OCGDGeom_Vector::y);
        ClassDB::bind_method(D_METHOD("z"), &OCGDGeom_Vector::z);
        ClassDB::bind_method(D_METHOD("cross", "other"), &OCGDGeom_Vector::cross);
        ClassDB::bind_method(D_METHOD("crossed", "other"), &OCGDGeom_Vector::crossed);
        ClassDB::bind_method(D_METHOD("cross_cross", "v1", "v2"), &OCGDGeom_Vector::cross_cross);
        ClassDB::bind_method(D_METHOD("cross_crossed", "v1", "v2"), &OCGDGeom_Vector::cross_crossed);
        ClassDB::bind_method(D_METHOD("dot", "other"), &OCGDGeom_Vector::dot);
        ClassDB::bind_method(D_METHOD("dot_cross", "v1", "v2"), &OCGDGeom_Vector::dot_cross);
        ClassDB::bind_method(D_METHOD("vec"), &OCGDGeom_Vector::vec);
    }

    OCGDGeom_Vector() {
        // This is an abstract class, so no direct instantiation
    }

    OCGDGeom_Vector(const Handle(Geom_Vector)& geom) : geom_vector(geom) {
        set_geom(geom);
    }

    Handle(Geom_Vector) get_geom_vector() const { return geom_vector; }
    void set_geom_vector(const Handle(Geom_Vector)& geom) { 
        geom_vector = geom; 
        set_geom(geom);
    }

    // Reverse the vector in place
    void reverse() {
        if (geom_vector.IsNull()) return;
        geom_vector->Reverse();
    }

    // Return reversed copy
    Ref<OCGDGeom_Vector> reversed() const {
        if (geom_vector.IsNull()) return Ref<OCGDGeom_Vector>();
        Handle(Geom_Vector) result = geom_vector->Reversed();
        Ref<OCGDGeom_Vector> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_vector(result);
        return wrapped;
    }

    // Compute angle between vectors
    real_t angle(Ref<OCGDGeom_Vector> other) const {
        if (geom_vector.IsNull() || other.is_null() || other->get_geom_vector().IsNull()) {
            return 0.0;
        }
        return geom_vector->Angle(other->get_geom_vector());
    }

    // Compute angle with reference vector
    real_t angle_with_ref(Ref<OCGDGeom_Vector> other, Ref<OCGDGeom_Vector> vref) const {
        if (geom_vector.IsNull() || other.is_null() || vref.is_null() ||
            other->get_geom_vector().IsNull() || vref->get_geom_vector().IsNull()) {
            return 0.0;
        }
        return geom_vector->AngleWithRef(other->get_geom_vector(), vref->get_geom_vector());
    }

    // Get coordinates as Vector3
    Vector3 coord() const {
        if (geom_vector.IsNull()) return Vector3();
        Standard_Real x, y, z;
        geom_vector->Coord(x, y, z);
        return Vector3(x, y, z);
    }

    // Virtual methods - these will be overridden in concrete classes
    virtual real_t magnitude() const {
        if (geom_vector.IsNull()) return 0.0;
        return geom_vector->Magnitude();
    }

    virtual real_t square_magnitude() const {
        if (geom_vector.IsNull()) return 0.0;
        return geom_vector->SquareMagnitude();
    }

    real_t x() const {
        if (geom_vector.IsNull()) return 0.0;
        return geom_vector->X();
    }

    real_t y() const {
        if (geom_vector.IsNull()) return 0.0;
        return geom_vector->Y();
    }

    real_t z() const {
        if (geom_vector.IsNull()) return 0.0;
        return geom_vector->Z();
    }

    // Virtual cross product methods - these will be overridden in concrete classes
    virtual void cross(Ref<OCGDGeom_Vector> other) {
        if (geom_vector.IsNull() || other.is_null() || other->get_geom_vector().IsNull()) return;
        geom_vector->Cross(other->get_geom_vector());
    }

    virtual Ref<OCGDGeom_Vector> crossed(Ref<OCGDGeom_Vector> other) const {
        if (geom_vector.IsNull() || other.is_null() || other->get_geom_vector().IsNull()) {
            return Ref<OCGDGeom_Vector>();
        }
        Handle(Geom_Vector) result = geom_vector->Crossed(other->get_geom_vector());
        Ref<OCGDGeom_Vector> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_vector(result);
        return wrapped;
    }

    virtual void cross_cross(Ref<OCGDGeom_Vector> v1, Ref<OCGDGeom_Vector> v2) {
        if (geom_vector.IsNull() || v1.is_null() || v2.is_null() ||
            v1->get_geom_vector().IsNull() || v2->get_geom_vector().IsNull()) return;
        geom_vector->CrossCross(v1->get_geom_vector(), v2->get_geom_vector());
    }

    virtual Ref<OCGDGeom_Vector> cross_crossed(Ref<OCGDGeom_Vector> v1, Ref<OCGDGeom_Vector> v2) const {
        if (geom_vector.IsNull() || v1.is_null() || v2.is_null() ||
            v1->get_geom_vector().IsNull() || v2->get_geom_vector().IsNull()) {
            return Ref<OCGDGeom_Vector>();
        }
        Handle(Geom_Vector) result = geom_vector->CrossCrossed(v1->get_geom_vector(), v2->get_geom_vector());
        Ref<OCGDGeom_Vector> wrapped;
        wrapped.instantiate();
        wrapped->set_geom_vector(result);
        return wrapped;
    }

    // Dot product
    real_t dot(Ref<OCGDGeom_Vector> other) const {
        if (geom_vector.IsNull() || other.is_null() || other->get_geom_vector().IsNull()) {
            return 0.0;
        }
        return geom_vector->Dot(other->get_geom_vector());
    }

    // Triple scalar product
    real_t dot_cross(Ref<OCGDGeom_Vector> v1, Ref<OCGDGeom_Vector> v2) const {
        if (geom_vector.IsNull() || v1.is_null() || v2.is_null() ||
            v1->get_geom_vector().IsNull() || v2->get_geom_vector().IsNull()) {
            return 0.0;
        }
        return geom_vector->DotCross(v1->get_geom_vector(), v2->get_geom_vector());
    }

    // Get as gp_Vec wrapped in ocgd_gp_Vec
    Ref<ocgd_gp_Vec> vec() const {
        if (geom_vector.IsNull()) return Ref<ocgd_gp_Vec>();
        const gp_Vec& result = geom_vector->Vec();
        Ref<ocgd_gp_Vec> wrapped;
        wrapped.instantiate();
        wrapped->get_vec() = result;
        return wrapped;
    }

    String to_string() const {
        if (geom_vector.IsNull()) return "Null Geom_Vector";
        Vector3 coords = coord();
        return String("Geom_Vector(") + String::num(coords.x) + ", " + 
               String::num(coords.y) + ", " + String::num(coords.z) + ")";
    }
};