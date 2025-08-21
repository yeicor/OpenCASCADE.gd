#ifndef OCGD_GP_PARAB_H
#define OCGD_GP_PARAB_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Parab.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Parab (3D parabola)
 * 
 * Describes a parabola in 3D space.
 * A parabola is defined by its focal length and positioned in space
 * with a coordinate system (a gp_Ax2 object) where:
 * - the origin is the apex of the parabola
 * - the "X Axis" of the coordinate system is the axis of symmetry
 * - the parabola is on the positive side of the "Y Axis"
 */
class ocgd_gp_Parab : public RefCounted {
    GDCLASS(ocgd_gp_Parab, RefCounted)

private:
    gp_Parab parab;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Parab::init_default);
        ClassDB::bind_method(D_METHOD("init_with_position_focal", "position", "focal_length"), &ocgd_gp_Parab::init_with_position_focal);

        // Setters
        ClassDB::bind_method(D_METHOD("set_axis", "axis"), &ocgd_gp_Parab::set_axis);
        ClassDB::bind_method(D_METHOD("set_focal", "focal_length"), &ocgd_gp_Parab::set_focal);
        ClassDB::bind_method(D_METHOD("set_location", "location"), &ocgd_gp_Parab::set_location);
        ClassDB::bind_method(D_METHOD("set_position", "position"), &ocgd_gp_Parab::set_position);

        // Getters
        ClassDB::bind_method(D_METHOD("get_axis"), &ocgd_gp_Parab::get_axis);
        ClassDB::bind_method(D_METHOD("get_directrix"), &ocgd_gp_Parab::get_directrix);
        ClassDB::bind_method(D_METHOD("get_focal"), &ocgd_gp_Parab::get_focal);
        ClassDB::bind_method(D_METHOD("get_focus"), &ocgd_gp_Parab::get_focus);
        ClassDB::bind_method(D_METHOD("get_location"), &ocgd_gp_Parab::get_location);
        ClassDB::bind_method(D_METHOD("get_parameter"), &ocgd_gp_Parab::get_parameter);
        ClassDB::bind_method(D_METHOD("get_position"), &ocgd_gp_Parab::get_position);
        ClassDB::bind_method(D_METHOD("get_x_axis"), &ocgd_gp_Parab::get_x_axis);
        ClassDB::bind_method(D_METHOD("get_y_axis"), &ocgd_gp_Parab::get_y_axis);

        // Transformations
        ClassDB::bind_method(D_METHOD("mirror_point", "point"), &ocgd_gp_Parab::mirror_point);
        ClassDB::bind_method(D_METHOD("mirrored_point", "point"), &ocgd_gp_Parab::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "axis"), &ocgd_gp_Parab::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "axis"), &ocgd_gp_Parab::mirrored_axis);
        ClassDB::bind_method(D_METHOD("mirror_plane", "plane"), &ocgd_gp_Parab::mirror_plane);
        ClassDB::bind_method(D_METHOD("mirrored_plane", "plane"), &ocgd_gp_Parab::mirrored_plane);
        ClassDB::bind_method(D_METHOD("rotate", "axis", "angle"), &ocgd_gp_Parab::rotate);
        ClassDB::bind_method(D_METHOD("rotated", "axis", "angle"), &ocgd_gp_Parab::rotated);
        ClassDB::bind_method(D_METHOD("scale", "center", "scale_factor"), &ocgd_gp_Parab::scale);
        ClassDB::bind_method(D_METHOD("scaled", "center", "scale_factor"), &ocgd_gp_Parab::scaled);
        ClassDB::bind_method(D_METHOD("transform", "transformation"), &ocgd_gp_Parab::transform);
        ClassDB::bind_method(D_METHOD("transformed", "transformation"), &ocgd_gp_Parab::transformed);
        ClassDB::bind_method(D_METHOD("translate", "vector"), &ocgd_gp_Parab::translate);
        ClassDB::bind_method(D_METHOD("translated", "vector"), &ocgd_gp_Parab::translated);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Parab::copy);
    }

public:
    ocgd_gp_Parab() : parab() {}
    ~ocgd_gp_Parab() = default;

    // Initialization methods
    void init_default() {
        parab = gp_Parab();
    }

    void init_with_position_focal(Ref<RefCounted> position, double focal_length) {
        // Note: This would need ocgd_gp_Ax2 binding
        // For now, we'll create a default parabola with focal length
        gp_Ax2 default_pos;
        parab = gp_Parab(default_pos, focal_length);
    }

    // Setters
    void set_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    void set_focal(double focal_length) {
        parab.SetFocal(focal_length);
    }

    void set_location(Vector3 location) {
        gp_Pnt p(location.x, location.y, location.z);
        parab.SetLocation(p);
    }

    void set_position(Ref<RefCounted> position) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    // Getters
    Ref<RefCounted> get_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_directrix() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    double get_focal() const {
        return parab.Focal();
    }

    Vector3 get_focus() const {
        const gp_Pnt& p = parab.Focus();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Vector3 get_location() const {
        const gp_Pnt& p = parab.Location();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    double get_parameter() const {
        return parab.Parameter();
    }

    Ref<RefCounted> get_position() const {
        // Note: This would return ocgd_gp_Ax2
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_x_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_y_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    // Transformations
    void mirror_point(Vector3 point) {
        gp_Pnt p(point.x, point.y, point.z);
        parab.Mirror(p);
    }

    Ref<ocgd_gp_Parab> mirrored_point(Vector3 point) const {
        Ref<ocgd_gp_Parab> result;
        result.instantiate();
        gp_Pnt p(point.x, point.y, point.z);
        result->set_gp_parab(parab.Mirrored(p));
        return result;
    }

    void mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Parab> mirrored_axis(Ref<RefCounted> axis) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Parab> result;
        result.instantiate();
        return result;
    }

    void mirror_plane(Ref<RefCounted> plane) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    Ref<ocgd_gp_Parab> mirrored_plane(Ref<RefCounted> plane) const {
        // Note: This would need ocgd_gp_Ax2 binding
        Ref<ocgd_gp_Parab> result;
        result.instantiate();
        return result;
    }

    void rotate(Ref<RefCounted> axis, double angle) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Parab> rotated(Ref<RefCounted> axis, double angle) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Parab> result;
        result.instantiate();
        return result;
    }

    void scale(Vector3 center, double scale_factor) {
        gp_Pnt p(center.x, center.y, center.z);
        parab.Scale(p, scale_factor);
    }

    Ref<ocgd_gp_Parab> scaled(Vector3 center, double scale_factor) const {
        Ref<ocgd_gp_Parab> result;
        result.instantiate();
        gp_Pnt p(center.x, center.y, center.z);
        result->set_gp_parab(parab.Scaled(p, scale_factor));
        return result;
    }

    void transform(Ref<RefCounted> transformation) {
        // Note: This would need ocgd_gp_Trsf binding
    }

    Ref<ocgd_gp_Parab> transformed(Ref<RefCounted> transformation) const {
        // Note: This would need ocgd_gp_Trsf binding
        Ref<ocgd_gp_Parab> result;
        result.instantiate();
        return result;
    }

    void translate(Vector3 vector) {
        gp_Vec v(vector.x, vector.y, vector.z);
        parab.Translate(v);
    }

    Ref<ocgd_gp_Parab> translated(Vector3 vector) const {
        Ref<ocgd_gp_Parab> result;
        result.instantiate();
        gp_Vec v(vector.x, vector.y, vector.z);
        result->set_gp_parab(parab.Translated(v));
        return result;
    }

    // Utility
    Ref<ocgd_gp_Parab> copy() const {
        Ref<ocgd_gp_Parab> result;
        result.instantiate();
        result->set_gp_parab(parab);
        return result;
    }

    // Internal access
    const gp_Parab& get_gp_parab() const { return parab; }
    void set_gp_parab(const gp_Parab& p) { parab = p; }
};

#endif // OCGD_GP_PARAB_H