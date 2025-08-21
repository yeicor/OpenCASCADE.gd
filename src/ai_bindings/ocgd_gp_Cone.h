#ifndef OCGD_GP_CONE_H
#define OCGD_GP_CONE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Cone.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Cone (3D cone)
 * 
 * Describes an infinite conical surface.
 * A cone is defined by its half-angle at the apex and positioned in space
 * with a coordinate system (a gp_Ax3 object), the "main Direction" of which
 * is the axis of revolution of the cone. The apex of the cone is on the negative
 * side of this axis.
 */
class ocgd_gp_Cone : public RefCounted {
    GDCLASS(ocgd_gp_Cone, RefCounted)

private:
    gp_Cone cone;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Cone::init_default);
        ClassDB::bind_method(D_METHOD("init_with_position_angle_radius", "position", "angle", "radius"), &ocgd_gp_Cone::init_with_position_angle_radius);

        // Setters
        ClassDB::bind_method(D_METHOD("set_axis", "axis"), &ocgd_gp_Cone::set_axis);
        ClassDB::bind_method(D_METHOD("set_location", "location"), &ocgd_gp_Cone::set_location);
        ClassDB::bind_method(D_METHOD("set_position", "position"), &ocgd_gp_Cone::set_position);
        ClassDB::bind_method(D_METHOD("set_radius", "radius"), &ocgd_gp_Cone::set_radius);
        ClassDB::bind_method(D_METHOD("set_semi_angle", "angle"), &ocgd_gp_Cone::set_semi_angle);

        // Getters
        ClassDB::bind_method(D_METHOD("get_apex"), &ocgd_gp_Cone::get_apex);
        ClassDB::bind_method(D_METHOD("get_axis"), &ocgd_gp_Cone::get_axis);
        ClassDB::bind_method(D_METHOD("get_coefficients"), &ocgd_gp_Cone::get_coefficients);
        ClassDB::bind_method(D_METHOD("is_direct"), &ocgd_gp_Cone::is_direct);
        ClassDB::bind_method(D_METHOD("get_location"), &ocgd_gp_Cone::get_location);
        ClassDB::bind_method(D_METHOD("get_position"), &ocgd_gp_Cone::get_position);
        ClassDB::bind_method(D_METHOD("get_ref_radius"), &ocgd_gp_Cone::get_ref_radius);
        ClassDB::bind_method(D_METHOD("get_semi_angle"), &ocgd_gp_Cone::get_semi_angle);
        ClassDB::bind_method(D_METHOD("get_x_axis"), &ocgd_gp_Cone::get_x_axis);
        ClassDB::bind_method(D_METHOD("get_y_axis"), &ocgd_gp_Cone::get_y_axis);

        // Transformations
        ClassDB::bind_method(D_METHOD("u_reverse"), &ocgd_gp_Cone::u_reverse);
        ClassDB::bind_method(D_METHOD("v_reverse"), &ocgd_gp_Cone::v_reverse);
        ClassDB::bind_method(D_METHOD("mirror_point", "point"), &ocgd_gp_Cone::mirror_point);
        ClassDB::bind_method(D_METHOD("mirrored_point", "point"), &ocgd_gp_Cone::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "axis"), &ocgd_gp_Cone::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "axis"), &ocgd_gp_Cone::mirrored_axis);
        ClassDB::bind_method(D_METHOD("mirror_plane", "plane"), &ocgd_gp_Cone::mirror_plane);
        ClassDB::bind_method(D_METHOD("mirrored_plane", "plane"), &ocgd_gp_Cone::mirrored_plane);
        ClassDB::bind_method(D_METHOD("rotate", "axis", "angle"), &ocgd_gp_Cone::rotate);
        ClassDB::bind_method(D_METHOD("rotated", "axis", "angle"), &ocgd_gp_Cone::rotated);
        ClassDB::bind_method(D_METHOD("scale", "center", "scale_factor"), &ocgd_gp_Cone::scale);
        ClassDB::bind_method(D_METHOD("scaled", "center", "scale_factor"), &ocgd_gp_Cone::scaled);
        ClassDB::bind_method(D_METHOD("transform", "transformation"), &ocgd_gp_Cone::transform);
        ClassDB::bind_method(D_METHOD("transformed", "transformation"), &ocgd_gp_Cone::transformed);
        ClassDB::bind_method(D_METHOD("translate", "vector"), &ocgd_gp_Cone::translate);
        ClassDB::bind_method(D_METHOD("translated", "vector"), &ocgd_gp_Cone::translated);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Cone::copy);
    }

public:
    ocgd_gp_Cone() : cone() {}
    ~ocgd_gp_Cone() = default;

    // Initialization methods
    void init_default() {
        cone = gp_Cone();
    }

    void init_with_position_angle_radius(Ref<RefCounted> position, double angle, double radius) {
        // Note: This would need ocgd_gp_Ax3 binding
        // For now, we'll create a default cone with angle and radius
        gp_Ax3 default_pos;
        cone = gp_Cone(default_pos, angle, radius);
    }

    // Setters
    void set_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    void set_location(Vector3 location) {
        gp_Pnt p(location.x, location.y, location.z);
        cone.SetLocation(p);
    }

    void set_position(Ref<RefCounted> position) {
        // Note: This would need ocgd_gp_Ax3 binding
    }

    void set_radius(double radius) {
        cone.SetRadius(radius);
    }

    void set_semi_angle(double angle) {
        cone.SetSemiAngle(angle);
    }

    // Getters
    Vector3 get_apex() const {
        const gp_Pnt& p = cone.Apex();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Ref<RefCounted> get_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Array get_coefficients() const {
        Standard_Real a1, a2, a3, b1, b2, b3, c1, c2, c3, d;
        cone.Coefficients(a1, a2, a3, b1, b2, b3, c1, c2, c3, d);
        Array result;
        result.push_back(a1);
        result.push_back(a2);
        result.push_back(a3);
        result.push_back(b1);
        result.push_back(b2);
        result.push_back(b3);
        result.push_back(c1);
        result.push_back(c2);
        result.push_back(c3);
        result.push_back(d);
        return result;
    }

    bool is_direct() const {
        return cone.Direct();
    }

    Vector3 get_location() const {
        const gp_Pnt& p = cone.Location();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Ref<RefCounted> get_position() const {
        // Note: This would return ocgd_gp_Ax3
        return Ref<RefCounted>();
    }

    double get_ref_radius() const {
        return cone.RefRadius();
    }

    double get_semi_angle() const {
        return cone.SemiAngle();
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
    void u_reverse() {
        cone.UReverse();
    }

    void v_reverse() {
        cone.VReverse();
    }

    void mirror_point(Vector3 point) {
        gp_Pnt p(point.x, point.y, point.z);
        cone.Mirror(p);
    }

    Ref<ocgd_gp_Cone> mirrored_point(Vector3 point) const {
        Ref<ocgd_gp_Cone> result;
        result.instantiate();
        gp_Pnt p(point.x, point.y, point.z);
        result->set_gp_cone(cone.Mirrored(p));
        return result;
    }

    void mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Cone> mirrored_axis(Ref<RefCounted> axis) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Cone> result;
        result.instantiate();
        return result;
    }

    void mirror_plane(Ref<RefCounted> plane) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    Ref<ocgd_gp_Cone> mirrored_plane(Ref<RefCounted> plane) const {
        // Note: This would need ocgd_gp_Ax2 binding
        Ref<ocgd_gp_Cone> result;
        result.instantiate();
        return result;
    }

    void rotate(Ref<RefCounted> axis, double angle) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Cone> rotated(Ref<RefCounted> axis, double angle) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Cone> result;
        result.instantiate();
        return result;
    }

    void scale(Vector3 center, double scale_factor) {
        gp_Pnt p(center.x, center.y, center.z);
        cone.Scale(p, scale_factor);
    }

    Ref<ocgd_gp_Cone> scaled(Vector3 center, double scale_factor) const {
        Ref<ocgd_gp_Cone> result;
        result.instantiate();
        gp_Pnt p(center.x, center.y, center.z);
        result->set_gp_cone(cone.Scaled(p, scale_factor));
        return result;
    }

    void transform(Ref<RefCounted> transformation) {
        // Note: This would need ocgd_gp_Trsf binding
    }

    Ref<ocgd_gp_Cone> transformed(Ref<RefCounted> transformation) const {
        // Note: This would need ocgd_gp_Trsf binding
        Ref<ocgd_gp_Cone> result;
        result.instantiate();
        return result;
    }

    void translate(Vector3 vector) {
        gp_Vec v(vector.x, vector.y, vector.z);
        cone.Translate(v);
    }

    Ref<ocgd_gp_Cone> translated(Vector3 vector) const {
        Ref<ocgd_gp_Cone> result;
        result.instantiate();
        gp_Vec v(vector.x, vector.y, vector.z);
        result->set_gp_cone(cone.Translated(v));
        return result;
    }

    // Utility
    Ref<ocgd_gp_Cone> copy() const {
        Ref<ocgd_gp_Cone> result;
        result.instantiate();
        result->set_gp_cone(cone);
        return result;
    }

    // Internal access
    const gp_Cone& get_gp_cone() const { return cone; }
    void set_gp_cone(const gp_Cone& c) { cone = c; }
};

#endif // OCGD_GP_CONE_H