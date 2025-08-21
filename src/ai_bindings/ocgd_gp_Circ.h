#ifndef OCGD_GP_CIRC_H
#define OCGD_GP_CIRC_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Circ.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Trsf.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Circ (3D circle)
 * 
 * Describes a circle in 3D space.
 * A circle is defined by its radius and positioned in space
 * with a coordinate system (a gp_Ax2 object) as follows:
 * - the origin of the coordinate system is the center of the circle, and
 * - the origin, "X Direction" and "Y Direction" of the
 *   coordinate system define the plane of the circle.
 */
class ocgd_gp_Circ : public RefCounted {
    GDCLASS(ocgd_gp_Circ, RefCounted)

private:
    gp_Circ circ;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Circ::init_default);
        ClassDB::bind_method(D_METHOD("init_with_position_radius", "position", "radius"), &ocgd_gp_Circ::init_with_position_radius);

        // Setters
        ClassDB::bind_method(D_METHOD("set_axis", "axis"), &ocgd_gp_Circ::set_axis);
        ClassDB::bind_method(D_METHOD("set_location", "point"), &ocgd_gp_Circ::set_location);
        ClassDB::bind_method(D_METHOD("set_position", "position"), &ocgd_gp_Circ::set_position);
        ClassDB::bind_method(D_METHOD("set_radius", "radius"), &ocgd_gp_Circ::set_radius);

        // Getters
        ClassDB::bind_method(D_METHOD("get_area"), &ocgd_gp_Circ::get_area);
        ClassDB::bind_method(D_METHOD("get_axis"), &ocgd_gp_Circ::get_axis);
        ClassDB::bind_method(D_METHOD("get_length"), &ocgd_gp_Circ::get_length);
        ClassDB::bind_method(D_METHOD("get_location"), &ocgd_gp_Circ::get_location);
        ClassDB::bind_method(D_METHOD("get_position"), &ocgd_gp_Circ::get_position);
        ClassDB::bind_method(D_METHOD("get_radius"), &ocgd_gp_Circ::get_radius);
        ClassDB::bind_method(D_METHOD("get_x_axis"), &ocgd_gp_Circ::get_x_axis);
        ClassDB::bind_method(D_METHOD("get_y_axis"), &ocgd_gp_Circ::get_y_axis);

        // Distance and containment
        ClassDB::bind_method(D_METHOD("distance", "point"), &ocgd_gp_Circ::distance);
        ClassDB::bind_method(D_METHOD("square_distance", "point"), &ocgd_gp_Circ::square_distance);
        ClassDB::bind_method(D_METHOD("contains", "point", "tolerance"), &ocgd_gp_Circ::contains);

        // Transformations
        ClassDB::bind_method(D_METHOD("mirror_point", "point"), &ocgd_gp_Circ::mirror_point);
        ClassDB::bind_method(D_METHOD("mirrored_point", "point"), &ocgd_gp_Circ::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "axis"), &ocgd_gp_Circ::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "axis"), &ocgd_gp_Circ::mirrored_axis);
        ClassDB::bind_method(D_METHOD("mirror_plane", "plane"), &ocgd_gp_Circ::mirror_plane);
        ClassDB::bind_method(D_METHOD("mirrored_plane", "plane"), &ocgd_gp_Circ::mirrored_plane);
        ClassDB::bind_method(D_METHOD("rotate", "axis", "angle"), &ocgd_gp_Circ::rotate);
        ClassDB::bind_method(D_METHOD("rotated", "axis", "angle"), &ocgd_gp_Circ::rotated);
        ClassDB::bind_method(D_METHOD("scale", "center", "scale_factor"), &ocgd_gp_Circ::scale);
        ClassDB::bind_method(D_METHOD("scaled", "center", "scale_factor"), &ocgd_gp_Circ::scaled);
        ClassDB::bind_method(D_METHOD("transform", "transformation"), &ocgd_gp_Circ::transform);
        ClassDB::bind_method(D_METHOD("transformed", "transformation"), &ocgd_gp_Circ::transformed);
        ClassDB::bind_method(D_METHOD("translate", "vector"), &ocgd_gp_Circ::translate);
        ClassDB::bind_method(D_METHOD("translated", "vector"), &ocgd_gp_Circ::translated);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Circ::copy);
    }

public:
    ocgd_gp_Circ() : circ() {}
    ~ocgd_gp_Circ() = default;

    // Initialization methods
    void init_default() {
        circ = gp_Circ();
    }

    void init_with_position_radius(Ref<RefCounted> position, double radius) {
        // Note: This would need ocgd_gp_Ax2 binding
        // For now, we'll create a default circle with radius
        gp_Ax2 default_pos;
        circ = gp_Circ(default_pos, radius);
    }

    // Setters
    void set_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    void set_location(Vector3 point) {
        gp_Pnt p(point.x, point.y, point.z);
        circ.SetLocation(p);
    }

    void set_position(Ref<RefCounted> position) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    void set_radius(double radius) {
        circ.SetRadius(radius);
    }

    // Getters
    double get_area() const {
        return circ.Area();
    }

    Ref<RefCounted> get_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    double get_length() const {
        return circ.Length();
    }

    Vector3 get_location() const {
        const gp_Pnt& p = circ.Location();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Ref<RefCounted> get_position() const {
        // Note: This would return ocgd_gp_Ax2
        return Ref<RefCounted>();
    }

    double get_radius() const {
        return circ.Radius();
    }

    Ref<RefCounted> get_x_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_y_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    // Distance and containment
    double distance(Vector3 point) const {
        gp_Pnt p(point.x, point.y, point.z);
        return circ.Distance(p);
    }

    double square_distance(Vector3 point) const {
        gp_Pnt p(point.x, point.y, point.z);
        return circ.SquareDistance(p);
    }

    bool contains(Vector3 point, double tolerance) const {
        gp_Pnt p(point.x, point.y, point.z);
        return circ.Contains(p, tolerance);
    }

    // Transformations
    void mirror_point(Vector3 point) {
        gp_Pnt p(point.x, point.y, point.z);
        circ.Mirror(p);
    }

    Ref<ocgd_gp_Circ> mirrored_point(Vector3 point) const {
        Ref<ocgd_gp_Circ> result;
        result.instantiate();
        gp_Pnt p(point.x, point.y, point.z);
        result->set_gp_circ(circ.Mirrored(p));
        return result;
    }

    void mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Circ> mirrored_axis(Ref<RefCounted> axis) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Circ> result;
        result.instantiate();
        return result;
    }

    void mirror_plane(Ref<RefCounted> plane) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    Ref<ocgd_gp_Circ> mirrored_plane(Ref<RefCounted> plane) const {
        // Note: This would need ocgd_gp_Ax2 binding
        Ref<ocgd_gp_Circ> result;
        result.instantiate();
        return result;
    }

    void rotate(Ref<RefCounted> axis, double angle) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Circ> rotated(Ref<RefCounted> axis, double angle) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Circ> result;
        result.instantiate();
        return result;
    }

    void scale(Vector3 center, double scale_factor) {
        gp_Pnt p(center.x, center.y, center.z);
        circ.Scale(p, scale_factor);
    }

    Ref<ocgd_gp_Circ> scaled(Vector3 center, double scale_factor) const {
        Ref<ocgd_gp_Circ> result;
        result.instantiate();
        gp_Pnt p(center.x, center.y, center.z);
        result->set_gp_circ(circ.Scaled(p, scale_factor));
        return result;
    }

    void transform(Ref<RefCounted> transformation) {
        // Note: This would need ocgd_gp_Trsf binding
    }

    Ref<ocgd_gp_Circ> transformed(Ref<RefCounted> transformation) const {
        // Note: This would need ocgd_gp_Trsf binding
        Ref<ocgd_gp_Circ> result;
        result.instantiate();
        return result;
    }

    void translate(Vector3 vector) {
        gp_Vec v(vector.x, vector.y, vector.z);
        circ.Translate(v);
    }

    Ref<ocgd_gp_Circ> translated(Vector3 vector) const {
        Ref<ocgd_gp_Circ> result;
        result.instantiate();
        gp_Vec v(vector.x, vector.y, vector.z);
        result->set_gp_circ(circ.Translated(v));
        return result;
    }

    // Utility
    Ref<ocgd_gp_Circ> copy() const {
        Ref<ocgd_gp_Circ> result;
        result.instantiate();
        result->set_gp_circ(circ);
        return result;
    }

    // Internal access
    const gp_Circ& get_gp_circ() const { return circ; }
    void set_gp_circ(const gp_Circ& c) { circ = c; }
};

#endif // OCGD_GP_CIRC_H