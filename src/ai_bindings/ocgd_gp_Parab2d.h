#ifndef OCGD_GP_PARAB2D_H
#define OCGD_GP_PARAB2D_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Parab2d.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Ax22d.hxx>
#include <gp_Trsf2d.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Parab2d (2D parabola)
 * 
 * Describes a parabola in the plane (2D space).
 * A parabola is defined by its focal length and positioned in the
 * plane with a coordinate system (a gp_Ax22d object) where:
 * - the origin is the apex of the parabola
 * - the "X Axis" of the coordinate system is the axis of symmetry
 * - the parabola is on the positive side of the "Y Axis"
 */
class ocgd_gp_Parab2d : public RefCounted {
    GDCLASS(ocgd_gp_Parab2d, RefCounted)

private:
    gp_Parab2d parab;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Parab2d::init_default);
        ClassDB::bind_method(D_METHOD("init_with_mirror_axis_focal", "mirror_axis", "focal_length", "is_sense"), &ocgd_gp_Parab2d::init_with_mirror_axis_focal);
        ClassDB::bind_method(D_METHOD("init_with_axis_focal", "axis", "focal_length"), &ocgd_gp_Parab2d::init_with_axis_focal);

        // Setters
        ClassDB::bind_method(D_METHOD("set_focal", "focal_length"), &ocgd_gp_Parab2d::set_focal);
        ClassDB::bind_method(D_METHOD("set_location", "point"), &ocgd_gp_Parab2d::set_location);
        ClassDB::bind_method(D_METHOD("set_mirror_axis", "axis"), &ocgd_gp_Parab2d::set_mirror_axis);
        ClassDB::bind_method(D_METHOD("set_axis", "axis"), &ocgd_gp_Parab2d::set_axis);

        // Getters
        ClassDB::bind_method(D_METHOD("get_coefficients"), &ocgd_gp_Parab2d::get_coefficients);
        ClassDB::bind_method(D_METHOD("get_directrix"), &ocgd_gp_Parab2d::get_directrix);
        ClassDB::bind_method(D_METHOD("get_focal"), &ocgd_gp_Parab2d::get_focal);
        ClassDB::bind_method(D_METHOD("get_focus"), &ocgd_gp_Parab2d::get_focus);
        ClassDB::bind_method(D_METHOD("get_location"), &ocgd_gp_Parab2d::get_location);
        ClassDB::bind_method(D_METHOD("get_mirror_axis"), &ocgd_gp_Parab2d::get_mirror_axis);
        ClassDB::bind_method(D_METHOD("get_parameter"), &ocgd_gp_Parab2d::get_parameter);
        ClassDB::bind_method(D_METHOD("get_axis"), &ocgd_gp_Parab2d::get_axis);

        // Queries
        ClassDB::bind_method(D_METHOD("is_direct"), &ocgd_gp_Parab2d::is_direct);

        // Transformations
        ClassDB::bind_method(D_METHOD("mirror_point", "point"), &ocgd_gp_Parab2d::mirror_point);
        ClassDB::bind_method(D_METHOD("mirrored_point", "point"), &ocgd_gp_Parab2d::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "axis"), &ocgd_gp_Parab2d::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "axis"), &ocgd_gp_Parab2d::mirrored_axis);
        ClassDB::bind_method(D_METHOD("rotate", "point", "angle"), &ocgd_gp_Parab2d::rotate);
        ClassDB::bind_method(D_METHOD("rotated", "point", "angle"), &ocgd_gp_Parab2d::rotated);
        ClassDB::bind_method(D_METHOD("scale", "point", "scale_factor"), &ocgd_gp_Parab2d::scale);
        ClassDB::bind_method(D_METHOD("scaled", "point", "scale_factor"), &ocgd_gp_Parab2d::scaled);
        ClassDB::bind_method(D_METHOD("transform", "transformation"), &ocgd_gp_Parab2d::transform);
        ClassDB::bind_method(D_METHOD("transformed", "transformation"), &ocgd_gp_Parab2d::transformed);
        ClassDB::bind_method(D_METHOD("translate", "vector"), &ocgd_gp_Parab2d::translate);
        ClassDB::bind_method(D_METHOD("translated", "vector"), &ocgd_gp_Parab2d::translated);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Parab2d::copy);
    }

public:
    ocgd_gp_Parab2d() : parab() {}
    ~ocgd_gp_Parab2d() = default;

    // Initialization methods
    void init_default() {
        parab = gp_Parab2d();
    }

    void init_with_mirror_axis_focal(Ref<RefCounted> mirror_axis, double focal_length, bool is_sense = true) {
        // Note: This would need ocgd_gp_Ax2d binding
        // For now, we'll create a default parabola with focal length
        gp_Ax2d default_axis;
        parab = gp_Parab2d(default_axis, focal_length, is_sense);
    }

    void init_with_axis_focal(Ref<RefCounted> axis, double focal_length) {
        // Note: This would need ocgd_gp_Ax22d binding
        // For now, we'll create a default parabola with focal length
        gp_Ax22d default_axis;
        parab = gp_Parab2d(default_axis, focal_length);
    }

    // Setters
    void set_focal(double focal_length) {
        parab.SetFocal(focal_length);
    }

    void set_location(Vector2 point) {
        gp_Pnt2d p(point.x, point.y);
        parab.SetLocation(p);
    }

    void set_mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax2d binding
    }

    void set_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax22d binding
    }

    // Getters
    Array get_coefficients() const {
        Standard_Real a, b, c, d, e, f;
        parab.Coefficients(a, b, c, d, e, f);
        Array result;
        result.push_back(a);
        result.push_back(b);
        result.push_back(c);
        result.push_back(d);
        result.push_back(e);
        result.push_back(f);
        return result;
    }

    Ref<RefCounted> get_directrix() const {
        // Note: This would return ocgd_gp_Ax2d
        return Ref<RefCounted>();
    }

    double get_focal() const {
        return parab.Focal();
    }

    Vector2 get_focus() const {
        const gp_Pnt2d& p = parab.Focus();
        return Vector2(p.X(), p.Y());
    }

    Vector2 get_location() const {
        const gp_Pnt2d& p = parab.Location();
        return Vector2(p.X(), p.Y());
    }

    Ref<RefCounted> get_mirror_axis() const {
        // Note: This would return ocgd_gp_Ax2d
        return Ref<RefCounted>();
    }

    double get_parameter() const {
        return parab.Parameter();
    }

    Ref<RefCounted> get_axis() const {
        // Note: This would return ocgd_gp_Ax22d
        return Ref<RefCounted>();
    }

    // Queries
    bool is_direct() const {
        return parab.IsDirect();
    }

    // Transformations
    void mirror_point(Vector2 point) {
        gp_Pnt2d p(point.x, point.y);
        parab.Mirror(p);
    }

    Ref<ocgd_gp_Parab2d> mirrored_point(Vector2 point) const {
        Ref<ocgd_gp_Parab2d> result;
        result.instantiate();
        gp_Pnt2d p(point.x, point.y);
        result->set_gp_parab2d(parab.Mirrored(p));
        return result;
    }

    void mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax2d binding
    }

    Ref<ocgd_gp_Parab2d> mirrored_axis(Ref<RefCounted> axis) const {
        // Note: This would need ocgd_gp_Ax2d binding
        Ref<ocgd_gp_Parab2d> result;
        result.instantiate();
        return result;
    }

    void rotate(Vector2 point, double angle) {
        gp_Pnt2d p(point.x, point.y);
        parab.Rotate(p, angle);
    }

    Ref<ocgd_gp_Parab2d> rotated(Vector2 point, double angle) const {
        Ref<ocgd_gp_Parab2d> result;
        result.instantiate();
        gp_Pnt2d p(point.x, point.y);
        result->set_gp_parab2d(parab.Rotated(p, angle));
        return result;
    }

    void scale(Vector2 point, double scale_factor) {
        gp_Pnt2d p(point.x, point.y);
        parab.Scale(p, scale_factor);
    }

    Ref<ocgd_gp_Parab2d> scaled(Vector2 point, double scale_factor) const {
        Ref<ocgd_gp_Parab2d> result;
        result.instantiate();
        gp_Pnt2d p(point.x, point.y);
        result->set_gp_parab2d(parab.Scaled(p, scale_factor));
        return result;
    }

    void transform(Ref<RefCounted> transformation) {
        // Note: This would need ocgd_gp_Trsf2d binding
    }

    Ref<ocgd_gp_Parab2d> transformed(Ref<RefCounted> transformation) const {
        // Note: This would need ocgd_gp_Trsf2d binding
        Ref<ocgd_gp_Parab2d> result;
        result.instantiate();
        return result;
    }

    void translate(Vector2 vector) {
        gp_Vec2d v(vector.x, vector.y);
        parab.Translate(v);
    }

    Ref<ocgd_gp_Parab2d> translated(Vector2 vector) const {
        Ref<ocgd_gp_Parab2d> result;
        result.instantiate();
        gp_Vec2d v(vector.x, vector.y);
        result->set_gp_parab2d(parab.Translated(v));
        return result;
    }

    // Utility
    Ref<ocgd_gp_Parab2d> copy() const {
        Ref<ocgd_gp_Parab2d> result;
        result.instantiate();
        result->set_gp_parab2d(parab);
        return result;
    }

    // Internal access
    const gp_Parab2d& get_gp_parab2d() const { return parab; }
    void set_gp_parab2d(const gp_Parab2d& p) { parab = p; }
};

#endif // OCGD_GP_PARAB2D_H