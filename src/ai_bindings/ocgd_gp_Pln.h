#ifndef OCGD_GP_PLN_H
#define OCGD_GP_PLN_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax3.hxx>
#include <gp_Lin.hxx>
#include <gp_Trsf.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Pln (3D plane)
 * 
 * Describes a plane.
 * A plane is positioned in space with a coordinate system
 * (a gp_Ax3 object), such that the plane is defined by the
 * origin, "X Direction" and "Y Direction" of this coordinate
 * system, which is the "local coordinate system" of the
 * plane. The "main Direction" of the coordinate system is a
 * vector normal to the plane.
 */
class ocgd_gp_Pln : public RefCounted {
    GDCLASS(ocgd_gp_Pln, RefCounted)

private:
    gp_Pln pln;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Pln::init_default);
        ClassDB::bind_method(D_METHOD("init_with_position", "position"), &ocgd_gp_Pln::init_with_position);
        ClassDB::bind_method(D_METHOD("init_with_point_normal", "point", "normal"), &ocgd_gp_Pln::init_with_point_normal);
        ClassDB::bind_method(D_METHOD("init_with_coefficients", "a", "b", "c", "d"), &ocgd_gp_Pln::init_with_coefficients);

        // Setters
        ClassDB::bind_method(D_METHOD("set_axis", "axis"), &ocgd_gp_Pln::set_axis);
        ClassDB::bind_method(D_METHOD("set_location", "location"), &ocgd_gp_Pln::set_location);
        ClassDB::bind_method(D_METHOD("set_position", "position"), &ocgd_gp_Pln::set_position);

        // Getters
        ClassDB::bind_method(D_METHOD("get_coefficients"), &ocgd_gp_Pln::get_coefficients);
        ClassDB::bind_method(D_METHOD("get_axis"), &ocgd_gp_Pln::get_axis);
        ClassDB::bind_method(D_METHOD("get_location"), &ocgd_gp_Pln::get_location);
        ClassDB::bind_method(D_METHOD("get_position"), &ocgd_gp_Pln::get_position);
        ClassDB::bind_method(D_METHOD("get_x_axis"), &ocgd_gp_Pln::get_x_axis);
        ClassDB::bind_method(D_METHOD("get_y_axis"), &ocgd_gp_Pln::get_y_axis);
        ClassDB::bind_method(D_METHOD("is_direct"), &ocgd_gp_Pln::is_direct);

        // Operations
        ClassDB::bind_method(D_METHOD("u_reverse"), &ocgd_gp_Pln::u_reverse);
        ClassDB::bind_method(D_METHOD("v_reverse"), &ocgd_gp_Pln::v_reverse);

        // Distance calculations
        ClassDB::bind_method(D_METHOD("distance_to_point", "point"), &ocgd_gp_Pln::distance_to_point);
        ClassDB::bind_method(D_METHOD("distance_to_line", "line"), &ocgd_gp_Pln::distance_to_line);
        ClassDB::bind_method(D_METHOD("distance_to_plane", "other"), &ocgd_gp_Pln::distance_to_plane);
        ClassDB::bind_method(D_METHOD("square_distance_to_point", "point"), &ocgd_gp_Pln::square_distance_to_point);
        ClassDB::bind_method(D_METHOD("square_distance_to_line", "line"), &ocgd_gp_Pln::square_distance_to_line);
        ClassDB::bind_method(D_METHOD("square_distance_to_plane", "other"), &ocgd_gp_Pln::square_distance_to_plane);

        // Containment tests
        ClassDB::bind_method(D_METHOD("contains_point", "point", "tolerance"), &ocgd_gp_Pln::contains_point);
        ClassDB::bind_method(D_METHOD("contains_line", "line", "linear_tolerance", "angular_tolerance"), &ocgd_gp_Pln::contains_line);

        // Transformations
        ClassDB::bind_method(D_METHOD("mirror_point", "point"), &ocgd_gp_Pln::mirror_point);
        ClassDB::bind_method(D_METHOD("mirrored_point", "point"), &ocgd_gp_Pln::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "axis"), &ocgd_gp_Pln::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "axis"), &ocgd_gp_Pln::mirrored_axis);
        ClassDB::bind_method(D_METHOD("mirror_plane", "plane"), &ocgd_gp_Pln::mirror_plane);
        ClassDB::bind_method(D_METHOD("mirrored_plane", "plane"), &ocgd_gp_Pln::mirrored_plane);
        ClassDB::bind_method(D_METHOD("rotate", "axis", "angle"), &ocgd_gp_Pln::rotate);
        ClassDB::bind_method(D_METHOD("rotated", "axis", "angle"), &ocgd_gp_Pln::rotated);
        ClassDB::bind_method(D_METHOD("scale", "center", "scale_factor"), &ocgd_gp_Pln::scale);
        ClassDB::bind_method(D_METHOD("scaled", "center", "scale_factor"), &ocgd_gp_Pln::scaled);
        ClassDB::bind_method(D_METHOD("transform", "transformation"), &ocgd_gp_Pln::transform);
        ClassDB::bind_method(D_METHOD("transformed", "transformation"), &ocgd_gp_Pln::transformed);
        ClassDB::bind_method(D_METHOD("translate", "vector"), &ocgd_gp_Pln::translate);
        ClassDB::bind_method(D_METHOD("translated", "vector"), &ocgd_gp_Pln::translated);
        ClassDB::bind_method(D_METHOD("translate_points", "p1", "p2"), &ocgd_gp_Pln::translate_points);
        ClassDB::bind_method(D_METHOD("translated_points", "p1", "p2"), &ocgd_gp_Pln::translated_points);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Pln::copy);
    }

public:
    ocgd_gp_Pln() : pln() {}
    ~ocgd_gp_Pln() = default;

    // Initialization methods
    void init_default() {
        pln = gp_Pln();
    }

    void init_with_position(Ref<RefCounted> position) {
        // Note: This would need ocgd_gp_Ax3 binding
        // For now, we'll create a default plane
        gp_Ax3 default_pos;
        pln = gp_Pln(default_pos);
    }

    void init_with_point_normal(Vector3 point, Vector3 normal) {
        gp_Pnt p(point.x, point.y, point.z);
        gp_Dir n(normal.x, normal.y, normal.z);
        pln = gp_Pln(p, n);
    }

    void init_with_coefficients(double a, double b, double c, double d) {
        pln = gp_Pln(a, b, c, d);
    }

    // Setters
    void set_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    void set_location(Vector3 location) {
        gp_Pnt p(location.x, location.y, location.z);
        pln.SetLocation(p);
    }

    void set_position(Ref<RefCounted> position) {
        // Note: This would need ocgd_gp_Ax3 binding
    }

    // Getters
    Array get_coefficients() const {
        Standard_Real a, b, c, d;
        pln.Coefficients(a, b, c, d);
        Array result;
        result.push_back(a);
        result.push_back(b);
        result.push_back(c);
        result.push_back(d);
        return result;
    }

    Ref<RefCounted> get_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Vector3 get_location() const {
        const gp_Pnt& p = pln.Location();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Ref<RefCounted> get_position() const {
        // Note: This would return ocgd_gp_Ax3
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

    bool is_direct() const {
        return pln.Direct();
    }

    // Operations
    void u_reverse() {
        pln.UReverse();
    }

    void v_reverse() {
        pln.VReverse();
    }

    // Distance calculations
    double distance_to_point(Vector3 point) const {
        gp_Pnt p(point.x, point.y, point.z);
        return pln.Distance(p);
    }

    double distance_to_line(Ref<RefCounted> line) const {
        // Note: This would need ocgd_gp_Lin binding
        // For now, return 0
        return 0.0;
    }

    double distance_to_plane(Ref<ocgd_gp_Pln> other) const {
        if (other.is_valid()) {
            return pln.Distance(other->get_gp_pln());
        }
        return 0.0;
    }

    double square_distance_to_point(Vector3 point) const {
        gp_Pnt p(point.x, point.y, point.z);
        return pln.SquareDistance(p);
    }

    double square_distance_to_line(Ref<RefCounted> line) const {
        // Note: This would need ocgd_gp_Lin binding
        // For now, return 0
        return 0.0;
    }

    double square_distance_to_plane(Ref<ocgd_gp_Pln> other) const {
        if (other.is_valid()) {
            return pln.SquareDistance(other->get_gp_pln());
        }
        return 0.0;
    }

    // Containment tests
    bool contains_point(Vector3 point, double tolerance) const {
        gp_Pnt p(point.x, point.y, point.z);
        return pln.Contains(p, tolerance);
    }

    bool contains_line(Ref<RefCounted> line, double linear_tolerance, double angular_tolerance) const {
        // Note: This would need ocgd_gp_Lin binding
        // For now, return false
        return false;
    }

    // Transformations
    void mirror_point(Vector3 point) {
        gp_Pnt p(point.x, point.y, point.z);
        pln.Mirror(p);
    }

    Ref<ocgd_gp_Pln> mirrored_point(Vector3 point) const {
        Ref<ocgd_gp_Pln> result;
        result.instantiate();
        gp_Pnt p(point.x, point.y, point.z);
        result->set_gp_pln(pln.Mirrored(p));
        return result;
    }

    void mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Pln> mirrored_axis(Ref<RefCounted> axis) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Pln> result;
        result.instantiate();
        return result;
    }

    void mirror_plane(Ref<RefCounted> plane) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    Ref<ocgd_gp_Pln> mirrored_plane(Ref<RefCounted> plane) const {
        // Note: This would need ocgd_gp_Ax2 binding
        Ref<ocgd_gp_Pln> result;
        result.instantiate();
        return result;
    }

    void rotate(Ref<RefCounted> axis, double angle) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Pln> rotated(Ref<RefCounted> axis, double angle) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Pln> result;
        result.instantiate();
        return result;
    }

    void scale(Vector3 center, double scale_factor) {
        gp_Pnt p(center.x, center.y, center.z);
        pln.Scale(p, scale_factor);
    }

    Ref<ocgd_gp_Pln> scaled(Vector3 center, double scale_factor) const {
        Ref<ocgd_gp_Pln> result;
        result.instantiate();
        gp_Pnt p(center.x, center.y, center.z);
        result->set_gp_pln(pln.Scaled(p, scale_factor));
        return result;
    }

    void transform(Ref<RefCounted> transformation) {
        // Note: This would need ocgd_gp_Trsf binding
    }

    Ref<ocgd_gp_Pln> transformed(Ref<RefCounted> transformation) const {
        // Note: This would need ocgd_gp_Trsf binding
        Ref<ocgd_gp_Pln> result;
        result.instantiate();
        return result;
    }

    void translate(Vector3 vector) {
        gp_Vec v(vector.x, vector.y, vector.z);
        pln.Translate(v);
    }

    Ref<ocgd_gp_Pln> translated(Vector3 vector) const {
        Ref<ocgd_gp_Pln> result;
        result.instantiate();
        gp_Vec v(vector.x, vector.y, vector.z);
        result->set_gp_pln(pln.Translated(v));
        return result;
    }

    void translate_points(Vector3 p1, Vector3 p2) {
        gp_Pnt point1(p1.x, p1.y, p1.z);
        gp_Pnt point2(p2.x, p2.y, p2.z);
        pln.Translate(point1, point2);
    }

    Ref<ocgd_gp_Pln> translated_points(Vector3 p1, Vector3 p2) const {
        Ref<ocgd_gp_Pln> result;
        result.instantiate();
        gp_Pnt point1(p1.x, p1.y, p1.z);
        gp_Pnt point2(p2.x, p2.y, p2.z);
        result->set_gp_pln(pln.Translated(point1, point2));
        return result;
    }

    // Utility
    Ref<ocgd_gp_Pln> copy() const {
        Ref<ocgd_gp_Pln> result;
        result.instantiate();
        result->set_gp_pln(pln);
        return result;
    }

    // Internal access
    const gp_Pln& get_gp_pln() const { return pln; }
    void set_gp_pln(const gp_Pln& p) { pln = p; }
};

#endif // OCGD_GP_PLN_H