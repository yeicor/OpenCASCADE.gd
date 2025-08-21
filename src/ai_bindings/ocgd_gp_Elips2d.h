#ifndef OCGD_GP_ELIPS2D_H
#define OCGD_GP_ELIPS2D_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Elips2d.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Ax22d.hxx>
#include <gp_Trsf2d.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Elips2d (2D ellipse)
 * 
 * Describes an ellipse in the plane (2D space).
 * An ellipse is defined by its major and minor radii and positioned in the
 * plane with a coordinate system (a gp_Ax22d object) where:
 * - the origin of the coordinate system is the center of the ellipse
 * - its "X Direction" defines the major axis of the ellipse
 * - its "Y Direction" defines the minor axis of the ellipse
 */
class ocgd_gp_Elips2d : public RefCounted {
    GDCLASS(ocgd_gp_Elips2d, RefCounted)

private:
    gp_Elips2d elips;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Elips2d::init_default);
        ClassDB::bind_method(D_METHOD("init_with_axis_radii", "major_axis", "major_radius", "minor_radius", "is_sense"), &ocgd_gp_Elips2d::init_with_axis_radii);
        ClassDB::bind_method(D_METHOD("init_with_position_radii", "position", "major_radius", "minor_radius"), &ocgd_gp_Elips2d::init_with_position_radii);

        // Setters
        ClassDB::bind_method(D_METHOD("set_location", "point"), &ocgd_gp_Elips2d::set_location);
        ClassDB::bind_method(D_METHOD("set_major_radius", "radius"), &ocgd_gp_Elips2d::set_major_radius);
        ClassDB::bind_method(D_METHOD("set_minor_radius", "radius"), &ocgd_gp_Elips2d::set_minor_radius);
        ClassDB::bind_method(D_METHOD("set_axis", "axis"), &ocgd_gp_Elips2d::set_axis);
        ClassDB::bind_method(D_METHOD("set_x_axis", "axis"), &ocgd_gp_Elips2d::set_x_axis);
        ClassDB::bind_method(D_METHOD("set_y_axis", "axis"), &ocgd_gp_Elips2d::set_y_axis);

        // Getters
        ClassDB::bind_method(D_METHOD("get_area"), &ocgd_gp_Elips2d::get_area);
        ClassDB::bind_method(D_METHOD("get_coefficients"), &ocgd_gp_Elips2d::get_coefficients);
        ClassDB::bind_method(D_METHOD("get_directrix1"), &ocgd_gp_Elips2d::get_directrix1);
        ClassDB::bind_method(D_METHOD("get_directrix2"), &ocgd_gp_Elips2d::get_directrix2);
        ClassDB::bind_method(D_METHOD("get_eccentricity"), &ocgd_gp_Elips2d::get_eccentricity);
        ClassDB::bind_method(D_METHOD("get_focal"), &ocgd_gp_Elips2d::get_focal);
        ClassDB::bind_method(D_METHOD("get_focus1"), &ocgd_gp_Elips2d::get_focus1);
        ClassDB::bind_method(D_METHOD("get_focus2"), &ocgd_gp_Elips2d::get_focus2);
        ClassDB::bind_method(D_METHOD("get_location"), &ocgd_gp_Elips2d::get_location);
        ClassDB::bind_method(D_METHOD("get_major_radius"), &ocgd_gp_Elips2d::get_major_radius);
        ClassDB::bind_method(D_METHOD("get_minor_radius"), &ocgd_gp_Elips2d::get_minor_radius);
        ClassDB::bind_method(D_METHOD("get_parameter"), &ocgd_gp_Elips2d::get_parameter);
        ClassDB::bind_method(D_METHOD("get_position"), &ocgd_gp_Elips2d::get_position);
        ClassDB::bind_method(D_METHOD("get_x_axis"), &ocgd_gp_Elips2d::get_x_axis);
        ClassDB::bind_method(D_METHOD("get_y_axis"), &ocgd_gp_Elips2d::get_y_axis);

        // Queries
        ClassDB::bind_method(D_METHOD("is_direct"), &ocgd_gp_Elips2d::is_direct);

        // Transformations
        ClassDB::bind_method(D_METHOD("mirror_point", "point"), &ocgd_gp_Elips2d::mirror_point);
        ClassDB::bind_method(D_METHOD("mirrored_point", "point"), &ocgd_gp_Elips2d::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "axis"), &ocgd_gp_Elips2d::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "axis"), &ocgd_gp_Elips2d::mirrored_axis);
        ClassDB::bind_method(D_METHOD("rotate", "point", "angle"), &ocgd_gp_Elips2d::rotate);
        ClassDB::bind_method(D_METHOD("rotated", "point", "angle"), &ocgd_gp_Elips2d::rotated);
        ClassDB::bind_method(D_METHOD("scale", "point", "scale_factor"), &ocgd_gp_Elips2d::scale);
        ClassDB::bind_method(D_METHOD("scaled", "point", "scale_factor"), &ocgd_gp_Elips2d::scaled);
        ClassDB::bind_method(D_METHOD("transform", "transformation"), &ocgd_gp_Elips2d::transform);
        ClassDB::bind_method(D_METHOD("transformed", "transformation"), &ocgd_gp_Elips2d::transformed);
        ClassDB::bind_method(D_METHOD("translate", "vector"), &ocgd_gp_Elips2d::translate);
        ClassDB::bind_method(D_METHOD("translated", "vector"), &ocgd_gp_Elips2d::translated);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Elips2d::copy);
    }

public:
    ocgd_gp_Elips2d() : elips() {}
    ~ocgd_gp_Elips2d() = default;

    // Initialization methods
    void init_default() {
        elips = gp_Elips2d();
    }

    void init_with_axis_radii(Ref<RefCounted> major_axis, double major_radius, double minor_radius, bool is_sense = true) {
        // Note: This would need ocgd_gp_Ax2d binding
        // For now, we'll create a default ellipse with radii
        gp_Ax2d default_axis;
        elips = gp_Elips2d(default_axis, major_radius, minor_radius, is_sense);
    }

    void init_with_position_radii(Ref<RefCounted> position, double major_radius, double minor_radius) {
        // Note: This would need ocgd_gp_Ax22d binding
        // For now, we'll create a default ellipse with radii
        gp_Ax22d default_pos;
        elips = gp_Elips2d(default_pos, major_radius, minor_radius);
    }

    // Setters
    void set_location(Vector2 point) {
        gp_Pnt2d p(point.x, point.y);
        elips.SetLocation(p);
    }

    void set_major_radius(double radius) {
        elips.SetMajorRadius(radius);
    }

    void set_minor_radius(double radius) {
        elips.SetMinorRadius(radius);
    }

    void set_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax22d binding
    }

    void set_x_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax2d binding
    }

    void set_y_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax2d binding
    }

    // Getters
    double get_area() const {
        return elips.Area();
    }

    Array get_coefficients() const {
        Standard_Real a, b, c, d, e, f;
        elips.Coefficients(a, b, c, d, e, f);
        Array result;
        result.push_back(a);
        result.push_back(b);
        result.push_back(c);
        result.push_back(d);
        result.push_back(e);
        result.push_back(f);
        return result;
    }

    Ref<RefCounted> get_directrix1() const {
        // Note: This would return ocgd_gp_Ax2d
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_directrix2() const {
        // Note: This would return ocgd_gp_Ax2d
        return Ref<RefCounted>();
    }

    double get_eccentricity() const {
        return elips.Eccentricity();
    }

    double get_focal() const {
        return elips.Focal();
    }

    Vector2 get_focus1() const {
        const gp_Pnt2d& p = elips.Focus1();
        return Vector2(p.X(), p.Y());
    }

    Vector2 get_focus2() const {
        const gp_Pnt2d& p = elips.Focus2();
        return Vector2(p.X(), p.Y());
    }

    Vector2 get_location() const {
        const gp_Pnt2d& p = elips.Location();
        return Vector2(p.X(), p.Y());
    }

    double get_major_radius() const {
        return elips.MajorRadius();
    }

    double get_minor_radius() const {
        return elips.MinorRadius();
    }

    double get_parameter() const {
        return elips.Parameter();
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

    // Queries
    bool is_direct() const {
        return elips.IsDirect();
    }

    // Transformations
    void mirror_point(Vector2 point) {
        gp_Pnt2d p(point.x, point.y);
        elips.Mirror(p);
    }

    Ref<ocgd_gp_Elips2d> mirrored_point(Vector2 point) const {
        Ref<ocgd_gp_Elips2d> result;
        result.instantiate();
        gp_Pnt2d p(point.x, point.y);
        result->set_gp_elips2d(elips.Mirrored(p));
        return result;
    }

    void mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax2d binding
    }

    Ref<ocgd_gp_Elips2d> mirrored_axis(Ref<RefCounted> axis) const {
        // Note: This would need ocgd_gp_Ax2d binding
        Ref<ocgd_gp_Elips2d> result;
        result.instantiate();
        return result;
    }

    void rotate(Vector2 point, double angle) {
        gp_Pnt2d p(point.x, point.y);
        elips.Rotate(p, angle);
    }

    Ref<ocgd_gp_Elips2d> rotated(Vector2 point, double angle) const {
        Ref<ocgd_gp_Elips2d> result;
        result.instantiate();
        gp_Pnt2d p(point.x, point.y);
        result->set_gp_elips2d(elips.Rotated(p, angle));
        return result;
    }

    void scale(Vector2 point, double scale_factor) {
        gp_Pnt2d p(point.x, point.y);
        elips.Scale(p, scale_factor);
    }

    Ref<ocgd_gp_Elips2d> scaled(Vector2 point, double scale_factor) const {
        Ref<ocgd_gp_Elips2d> result;
        result.instantiate();
        gp_Pnt2d p(point.x, point.y);
        result->set_gp_elips2d(elips.Scaled(p, scale_factor));
        return result;
    }

    void transform(Ref<RefCounted> transformation) {
        // Note: This would need ocgd_gp_Trsf2d binding
    }

    Ref<ocgd_gp_Elips2d> transformed(Ref<RefCounted> transformation) const {
        // Note: This would need ocgd_gp_Trsf2d binding
        Ref<ocgd_gp_Elips2d> result;
        result.instantiate();
        return result;
    }

    void translate(Vector2 vector) {
        gp_Vec2d v(vector.x, vector.y);
        elips.Translate(v);
    }

    Ref<ocgd_gp_Elips2d> translated(Vector2 vector) const {
        Ref<ocgd_gp_Elips2d> result;
        result.instantiate();
        gp_Vec2d v(vector.x, vector.y);
        result->set_gp_elips2d(elips.Translated(v));
        return result;
    }

    // Utility
    Ref<ocgd_gp_Elips2d> copy() const {
        Ref<ocgd_gp_Elips2d> result;
        result.instantiate();
        result->set_gp_elips2d(elips);
        return result;
    }

    // Internal access
    const gp_Elips2d& get_gp_elips2d() const { return elips; }
    void set_gp_elips2d(const gp_Elips2d& e) { elips = e; }
};

#endif // OCGD_GP_ELIPS2D_H