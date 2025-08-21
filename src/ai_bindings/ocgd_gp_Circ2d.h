#ifndef OCGD_GP_CIRC2D_H
#define OCGD_GP_CIRC2D_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Circ2d.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Ax22d.hxx>
#include <gp_Trsf2d.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Circ2d (2D circle)
 * 
 * Describes a circle in the plane (2D space).
 * A circle is defined by its radius and positioned in the
 * plane with a coordinate system (a gp_Ax22d object) as follows:
 * - the origin of the coordinate system is the center of the circle, and
 * - the orientation (direct or indirect) of the coordinate
 *   system gives an implicit orientation to the circle (and
 *   defines its trigonometric sense).
 */
class ocgd_gp_Circ2d : public RefCounted {
    GDCLASS(ocgd_gp_Circ2d, RefCounted)

private:
    gp_Circ2d circ;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Circ2d::init_default);
        ClassDB::bind_method(D_METHOD("init_with_axis_radius", "x_axis", "radius", "is_sense"), &ocgd_gp_Circ2d::init_with_axis_radius);
        ClassDB::bind_method(D_METHOD("init_with_position_radius", "position", "radius"), &ocgd_gp_Circ2d::init_with_position_radius);

        // Setters
        ClassDB::bind_method(D_METHOD("set_location", "point"), &ocgd_gp_Circ2d::set_location);
        ClassDB::bind_method(D_METHOD("set_x_axis", "axis"), &ocgd_gp_Circ2d::set_x_axis);
        ClassDB::bind_method(D_METHOD("set_axis", "axis"), &ocgd_gp_Circ2d::set_axis);
        ClassDB::bind_method(D_METHOD("set_y_axis", "axis"), &ocgd_gp_Circ2d::set_y_axis);
        ClassDB::bind_method(D_METHOD("set_radius", "radius"), &ocgd_gp_Circ2d::set_radius);

        // Getters
        ClassDB::bind_method(D_METHOD("get_area"), &ocgd_gp_Circ2d::get_area);
        ClassDB::bind_method(D_METHOD("get_coefficients"), &ocgd_gp_Circ2d::get_coefficients);
        ClassDB::bind_method(D_METHOD("get_length"), &ocgd_gp_Circ2d::get_length);
        ClassDB::bind_method(D_METHOD("get_location"), &ocgd_gp_Circ2d::get_location);
        ClassDB::bind_method(D_METHOD("get_radius"), &ocgd_gp_Circ2d::get_radius);
        ClassDB::bind_method(D_METHOD("get_position"), &ocgd_gp_Circ2d::get_position);
        ClassDB::bind_method(D_METHOD("get_x_axis"), &ocgd_gp_Circ2d::get_x_axis);
        ClassDB::bind_method(D_METHOD("get_y_axis"), &ocgd_gp_Circ2d::get_y_axis);

        // Distance and containment
        ClassDB::bind_method(D_METHOD("contains", "point", "tolerance"), &ocgd_gp_Circ2d::contains);
        ClassDB::bind_method(D_METHOD("distance", "point"), &ocgd_gp_Circ2d::distance);
        ClassDB::bind_method(D_METHOD("square_distance", "point"), &ocgd_gp_Circ2d::square_distance);

        // Transformations
        ClassDB::bind_method(D_METHOD("mirror_point", "point"), &ocgd_gp_Circ2d::mirror_point);
        ClassDB::bind_method(D_METHOD("mirrored_point", "point"), &ocgd_gp_Circ2d::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "axis"), &ocgd_gp_Circ2d::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "axis"), &ocgd_gp_Circ2d::mirrored_axis);
        ClassDB::bind_method(D_METHOD("rotate", "point", "angle"), &ocgd_gp_Circ2d::rotate);
        ClassDB::bind_method(D_METHOD("rotated", "point", "angle"), &ocgd_gp_Circ2d::rotated);
        ClassDB::bind_method(D_METHOD("scale", "point", "scale_factor"), &ocgd_gp_Circ2d::scale);
        ClassDB::bind_method(D_METHOD("scaled", "point", "scale_factor"), &ocgd_gp_Circ2d::scaled);
        ClassDB::bind_method(D_METHOD("transform", "transformation"), &ocgd_gp_Circ2d::transform);
        ClassDB::bind_method(D_METHOD("transformed", "transformation"), &ocgd_gp_Circ2d::transformed);
        ClassDB::bind_method(D_METHOD("translate", "vector"), &ocgd_gp_Circ2d::translate);
        ClassDB::bind_method(D_METHOD("translated", "vector"), &ocgd_gp_Circ2d::translated);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Circ2d::copy);
    }

public:
    ocgd_gp_Circ2d() : circ() {}
    ~ocgd_gp_Circ2d() = default;

    // Initialization methods
    void init_default() {
        circ = gp_Circ2d();
    }

    void init_with_axis_radius(Ref<RefCounted> x_axis, double radius, bool is_sense = true) {
        // Note: This would need ocgd_gp_Ax2d binding
        // For now, we'll create a default circle with radius
        gp_Ax2d default_axis;
        circ = gp_Circ2d(default_axis, radius, is_sense);
    }

    void init_with_position_radius(Ref<RefCounted> position, double radius) {
        // Note: This would need ocgd_gp_Ax22d binding
        // For now, we'll create a default circle with radius
        gp_Ax22d default_pos;
        circ = gp_Circ2d(default_pos, radius);
    }

    // Setters
    void set_location(Vector2 point) {
        gp_Pnt2d p(point.x, point.y);
        circ.SetLocation(p);
    }

    void set_x_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax2d binding
    }

    void set_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax22d binding
    }

    void set_y_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax2d binding
    }

    void set_radius(double radius) {
        circ.SetRadius(radius);
    }

    // Getters
    double get_area() const {
        return circ.Area();
    }

    Array get_coefficients() const {
        Standard_Real a, b, c, d, e, f;
        circ.Coefficients(a, b, c, d, e, f);
        Array result;
        result.push_back(a);
        result.push_back(b);
        result.push_back(c);
        result.push_back(d);
        result.push_back(e);
        result.push_back(f);
        return result;
    }

    double get_length() const {
        return circ.Length();
    }

    Vector2 get_location() const {
        const gp_Pnt2d& p = circ.Location();
        return Vector2(p.X(), p.Y());
    }

    double get_radius() const {
        return circ.Radius();
    }

    Ref<RefCounted> get_position() const {
        // Note: This would return ocgd_gp_Ax22d
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_x_axis() const {
        // Note: This would return ocgd_gp_Ax2d
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_y_axis() const {
        // Note: This would return ocgd_gp_Ax2d
        return Ref<RefCounted>();
    }

    // Distance and containment
    bool contains(Vector2 point, double tolerance) const {
        gp_Pnt2d p(point.x, point.y);
        return circ.Contains(p, tolerance);
    }

    double distance(Vector2 point) const {
        gp_Pnt2d p(point.x, point.y);
        return circ.Distance(p);
    }

    double square_distance(Vector2 point) const {
        gp_Pnt2d p(point.x, point.y);
        return circ.SquareDistance(p);
    }

    // Transformations
    void mirror_point(Vector2 point) {
        gp_Pnt2d p(point.x, point.y);
        circ.Mirror(p);
    }

    Ref<ocgd_gp_Circ2d> mirrored_point(Vector2 point) const {
        Ref<ocgd_gp_Circ2d> result;
        result.instantiate();
        gp_Pnt2d p(point.x, point.y);
        result->set_gp_circ2d(circ.Mirrored(p));
        return result;
    }

    void mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax2d binding
    }

    Ref<ocgd_gp_Circ2d> mirrored_axis(Ref<RefCounted> axis) const {
        // Note: This would need ocgd_gp_Ax2d binding
        Ref<ocgd_gp_Circ2d> result;
        result.instantiate();
        return result;
    }

    void rotate(Vector2 point, double angle) {
        gp_Pnt2d p(point.x, point.y);
        circ.Rotate(p, angle);
    }

    Ref<ocgd_gp_Circ2d> rotated(Vector2 point, double angle) const {
        Ref<ocgd_gp_Circ2d> result;
        result.instantiate();
        gp_Pnt2d p(point.x, point.y);
        result->set_gp_circ2d(circ.Rotated(p, angle));
        return result;
    }

    void scale(Vector2 point, double scale_factor) {
        gp_Pnt2d p(point.x, point.y);
        circ.Scale(p, scale_factor);
    }

    Ref<ocgd_gp_Circ2d> scaled(Vector2 point, double scale_factor) const {
        Ref<ocgd_gp_Circ2d> result;
        result.instantiate();
        gp_Pnt2d p(point.x, point.y);
        result->set_gp_circ2d(circ.Scaled(p, scale_factor));
        return result;
    }

    void transform(Ref<RefCounted> transformation) {
        // Note: This would need ocgd_gp_Trsf2d binding
    }

    Ref<ocgd_gp_Circ2d> transformed(Ref<RefCounted> transformation) const {
        // Note: This would need ocgd_gp_Trsf2d binding
        Ref<ocgd_gp_Circ2d> result;
        result.instantiate();
        return result;
    }

    void translate(Vector2 vector) {
        gp_Vec2d v(vector.x, vector.y);
        circ.Translate(v);
    }

    Ref<ocgd_gp_Circ2d> translated(Vector2 vector) const {
        Ref<ocgd_gp_Circ2d> result;
        result.instantiate();
        gp_Vec2d v(vector.x, vector.y);
        result->set_gp_circ2d(circ.Translated(v));
        return result;
    }

    // Utility
    Ref<ocgd_gp_Circ2d> copy() const {
        Ref<ocgd_gp_Circ2d> result;
        result.instantiate();
        result->set_gp_circ2d(circ);
        return result;
    }

    // Internal access
    const gp_Circ2d& get_gp_circ2d() const { return circ; }
    void set_gp_circ2d(const gp_Circ2d& c) { circ = c; }
};

#endif // OCGD_GP_CIRC2D_H