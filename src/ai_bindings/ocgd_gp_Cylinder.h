#ifndef OCGD_GP_CYLINDER_H
#define OCGD_GP_CYLINDER_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Cylinder.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Cylinder (3D cylinder)
 * 
 * Describes an infinite cylindrical surface.
 * A cylinder is defined by its radius and positioned in space
 * with a coordinate system (a gp_Ax3 object), the "main
 * Axis" of which is the axis of the cylinder.
 */
class ocgd_gp_Cylinder : public RefCounted {
    GDCLASS(ocgd_gp_Cylinder, RefCounted)

private:
    gp_Cylinder cylinder;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Cylinder::init_default);
        ClassDB::bind_method(D_METHOD("init_with_position_radius", "position", "radius"), &ocgd_gp_Cylinder::init_with_position_radius);

        // Setters
        ClassDB::bind_method(D_METHOD("set_axis", "axis"), &ocgd_gp_Cylinder::set_axis);
        ClassDB::bind_method(D_METHOD("set_location", "location"), &ocgd_gp_Cylinder::set_location);
        ClassDB::bind_method(D_METHOD("set_position", "position"), &ocgd_gp_Cylinder::set_position);
        ClassDB::bind_method(D_METHOD("set_radius", "radius"), &ocgd_gp_Cylinder::set_radius);

        // Getters
        ClassDB::bind_method(D_METHOD("get_axis"), &ocgd_gp_Cylinder::get_axis);
        ClassDB::bind_method(D_METHOD("get_coefficients"), &ocgd_gp_Cylinder::get_coefficients);
        ClassDB::bind_method(D_METHOD("is_direct"), &ocgd_gp_Cylinder::is_direct);
        ClassDB::bind_method(D_METHOD("get_location"), &ocgd_gp_Cylinder::get_location);
        ClassDB::bind_method(D_METHOD("get_position"), &ocgd_gp_Cylinder::get_position);
        ClassDB::bind_method(D_METHOD("get_radius"), &ocgd_gp_Cylinder::get_radius);
        ClassDB::bind_method(D_METHOD("get_x_axis"), &ocgd_gp_Cylinder::get_x_axis);
        ClassDB::bind_method(D_METHOD("get_y_axis"), &ocgd_gp_Cylinder::get_y_axis);

        // Transformations
        ClassDB::bind_method(D_METHOD("u_reverse"), &ocgd_gp_Cylinder::u_reverse);
        ClassDB::bind_method(D_METHOD("v_reverse"), &ocgd_gp_Cylinder::v_reverse);
        ClassDB::bind_method(D_METHOD("mirror_point", "point"), &ocgd_gp_Cylinder::mirror_point);
        ClassDB::bind_method(D_METHOD("mirrored_point", "point"), &ocgd_gp_Cylinder::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "axis"), &ocgd_gp_Cylinder::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "axis"), &ocgd_gp_Cylinder::mirrored_axis);
        ClassDB::bind_method(D_METHOD("mirror_plane", "plane"), &ocgd_gp_Cylinder::mirror_plane);
        ClassDB::bind_method(D_METHOD("mirrored_plane", "plane"), &ocgd_gp_Cylinder::mirrored_plane);
        ClassDB::bind_method(D_METHOD("rotate", "axis", "angle"), &ocgd_gp_Cylinder::rotate);
        ClassDB::bind_method(D_METHOD("rotated", "axis", "angle"), &ocgd_gp_Cylinder::rotated);
        ClassDB::bind_method(D_METHOD("scale", "center", "scale_factor"), &ocgd_gp_Cylinder::scale);
        ClassDB::bind_method(D_METHOD("scaled", "center", "scale_factor"), &ocgd_gp_Cylinder::scaled);
        ClassDB::bind_method(D_METHOD("transform", "transformation"), &ocgd_gp_Cylinder::transform);
        ClassDB::bind_method(D_METHOD("transformed", "transformation"), &ocgd_gp_Cylinder::transformed);
        ClassDB::bind_method(D_METHOD("translate", "vector"), &ocgd_gp_Cylinder::translate);
        ClassDB::bind_method(D_METHOD("translated", "vector"), &ocgd_gp_Cylinder::translated);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Cylinder::copy);
    }

public:
    ocgd_gp_Cylinder() : cylinder() {}
    ~ocgd_gp_Cylinder() = default;

    // Initialization methods
    void init_default() {
        cylinder = gp_Cylinder();
    }

    void init_with_position_radius(Ref<RefCounted> position, double radius) {
        // Note: This would need ocgd_gp_Ax3 binding
        // For now, we'll create a default cylinder with radius
        gp_Ax3 default_pos;
        cylinder = gp_Cylinder(default_pos, radius);
    }

    // Setters
    void set_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    void set_location(Vector3 location) {
        gp_Pnt p(location.x, location.y, location.z);
        cylinder.SetLocation(p);
    }

    void set_position(Ref<RefCounted> position) {
        // Note: This would need ocgd_gp_Ax3 binding
    }

    void set_radius(double radius) {
        cylinder.SetRadius(radius);
    }

    // Getters
    Ref<RefCounted> get_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Array get_coefficients() const {
        Standard_Real a1, a2, a3, b1, b2, b3, c1, c2, c3, d;
        cylinder.Coefficients(a1, a2, a3, b1, b2, b3, c1, c2, c3, d);
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
        return cylinder.Direct();
    }

    Vector3 get_location() const {
        const gp_Pnt& p = cylinder.Location();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Ref<RefCounted> get_position() const {
        // Note: This would return ocgd_gp_Ax3
        return Ref<RefCounted>();
    }

    double get_radius() const {
        return cylinder.Radius();
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
        cylinder.UReverse();
    }

    void v_reverse() {
        cylinder.VReverse();
    }

    void mirror_point(Vector3 point) {
        gp_Pnt p(point.x, point.y, point.z);
        cylinder.Mirror(p);
    }

    Ref<ocgd_gp_Cylinder> mirrored_point(Vector3 point) const {
        Ref<ocgd_gp_Cylinder> result;
        result.instantiate();
        gp_Pnt p(point.x, point.y, point.z);
        result->set_gp_cylinder(cylinder.Mirrored(p));
        return result;
    }

    void mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Cylinder> mirrored_axis(Ref<RefCounted> axis) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Cylinder> result;
        result.instantiate();
        return result;
    }

    void mirror_plane(Ref<RefCounted> plane) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    Ref<ocgd_gp_Cylinder> mirrored_plane(Ref<RefCounted> plane) const {
        // Note: This would need ocgd_gp_Ax2 binding
        Ref<ocgd_gp_Cylinder> result;
        result.instantiate();
        return result;
    }

    void rotate(Ref<RefCounted> axis, double angle) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Cylinder> rotated(Ref<RefCounted> axis, double angle) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Cylinder> result;
        result.instantiate();
        return result;
    }

    void scale(Vector3 center, double scale_factor) {
        gp_Pnt p(center.x, center.y, center.z);
        cylinder.Scale(p, scale_factor);
    }

    Ref<ocgd_gp_Cylinder> scaled(Vector3 center, double scale_factor) const {
        Ref<ocgd_gp_Cylinder> result;
        result.instantiate();
        gp_Pnt p(center.x, center.y, center.z);
        result->set_gp_cylinder(cylinder.Scaled(p, scale_factor));
        return result;
    }

    void transform(Ref<RefCounted> transformation) {
        // Note: This would need ocgd_gp_Trsf binding
    }

    Ref<ocgd_gp_Cylinder> transformed(Ref<RefCounted> transformation) const {
        // Note: This would need ocgd_gp_Trsf binding
        Ref<ocgd_gp_Cylinder> result;
        result.instantiate();
        return result;
    }

    void translate(Vector3 vector) {
        gp_Vec v(vector.x, vector.y, vector.z);
        cylinder.Translate(v);
    }

    Ref<ocgd_gp_Cylinder> translated(Vector3 vector) const {
        Ref<ocgd_gp_Cylinder> result;
        result.instantiate();
        gp_Vec v(vector.x, vector.y, vector.z);
        result->set_gp_cylinder(cylinder.Translated(v));
        return result;
    }

    // Utility
    Ref<ocgd_gp_Cylinder> copy() const {
        Ref<ocgd_gp_Cylinder> result;
        result.instantiate();
        result->set_gp_cylinder(cylinder);
        return result;
    }

    // Internal access
    const gp_Cylinder& get_gp_cylinder() const { return cylinder; }
    void set_gp_cylinder(const gp_Cylinder& c) { cylinder = c; }
};

#endif // OCGD_GP_CYLINDER_H