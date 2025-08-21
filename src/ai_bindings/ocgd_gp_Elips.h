#ifndef OCGD_GP_ELIPS_H
#define OCGD_GP_ELIPS_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Elips.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Elips (3D ellipse)
 * 
 * Describes an ellipse in 3D space.
 * An ellipse is defined by its major and minor radii and positioned in space
 * with a coordinate system (a gp_Ax2 object) as follows:
 * - the origin of the coordinate system is the center of the ellipse
 * - its "X Direction" defines the major axis of the ellipse
 * - its "Y Direction" defines the minor axis of the ellipse
 * - the origin, "X Direction" and "Y Direction" of the coordinate system
 *   define the plane of the ellipse
 */
class ocgd_gp_Elips : public RefCounted {
    GDCLASS(ocgd_gp_Elips, RefCounted)

private:
    gp_Elips elips;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Elips::init_default);
        ClassDB::bind_method(D_METHOD("init_with_position_radii", "position", "major_radius", "minor_radius"), &ocgd_gp_Elips::init_with_position_radii);

        // Setters
        ClassDB::bind_method(D_METHOD("set_axis", "axis"), &ocgd_gp_Elips::set_axis);
        ClassDB::bind_method(D_METHOD("set_location", "location"), &ocgd_gp_Elips::set_location);
        ClassDB::bind_method(D_METHOD("set_major_radius", "radius"), &ocgd_gp_Elips::set_major_radius);
        ClassDB::bind_method(D_METHOD("set_minor_radius", "radius"), &ocgd_gp_Elips::set_minor_radius);
        ClassDB::bind_method(D_METHOD("set_position", "position"), &ocgd_gp_Elips::set_position);

        // Getters
        ClassDB::bind_method(D_METHOD("get_area"), &ocgd_gp_Elips::get_area);
        ClassDB::bind_method(D_METHOD("get_axis"), &ocgd_gp_Elips::get_axis);
        ClassDB::bind_method(D_METHOD("get_directrix1"), &ocgd_gp_Elips::get_directrix1);
        ClassDB::bind_method(D_METHOD("get_directrix2"), &ocgd_gp_Elips::get_directrix2);
        ClassDB::bind_method(D_METHOD("get_eccentricity"), &ocgd_gp_Elips::get_eccentricity);
        ClassDB::bind_method(D_METHOD("get_focal"), &ocgd_gp_Elips::get_focal);
        ClassDB::bind_method(D_METHOD("get_focus1"), &ocgd_gp_Elips::get_focus1);
        ClassDB::bind_method(D_METHOD("get_focus2"), &ocgd_gp_Elips::get_focus2);
        ClassDB::bind_method(D_METHOD("get_location"), &ocgd_gp_Elips::get_location);
        ClassDB::bind_method(D_METHOD("get_major_radius"), &ocgd_gp_Elips::get_major_radius);
        ClassDB::bind_method(D_METHOD("get_minor_radius"), &ocgd_gp_Elips::get_minor_radius);
        ClassDB::bind_method(D_METHOD("get_parameter"), &ocgd_gp_Elips::get_parameter);
        ClassDB::bind_method(D_METHOD("get_position"), &ocgd_gp_Elips::get_position);
        ClassDB::bind_method(D_METHOD("get_x_axis"), &ocgd_gp_Elips::get_x_axis);
        ClassDB::bind_method(D_METHOD("get_y_axis"), &ocgd_gp_Elips::get_y_axis);



        // Transformations
        ClassDB::bind_method(D_METHOD("mirror_point", "point"), &ocgd_gp_Elips::mirror_point);
        ClassDB::bind_method(D_METHOD("mirrored_point", "point"), &ocgd_gp_Elips::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "axis"), &ocgd_gp_Elips::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "axis"), &ocgd_gp_Elips::mirrored_axis);
        ClassDB::bind_method(D_METHOD("mirror_plane", "plane"), &ocgd_gp_Elips::mirror_plane);
        ClassDB::bind_method(D_METHOD("mirrored_plane", "plane"), &ocgd_gp_Elips::mirrored_plane);
        ClassDB::bind_method(D_METHOD("rotate", "axis", "angle"), &ocgd_gp_Elips::rotate);
        ClassDB::bind_method(D_METHOD("rotated", "axis", "angle"), &ocgd_gp_Elips::rotated);
        ClassDB::bind_method(D_METHOD("scale", "center", "scale_factor"), &ocgd_gp_Elips::scale);
        ClassDB::bind_method(D_METHOD("scaled", "center", "scale_factor"), &ocgd_gp_Elips::scaled);
        ClassDB::bind_method(D_METHOD("transform", "transformation"), &ocgd_gp_Elips::transform);
        ClassDB::bind_method(D_METHOD("transformed", "transformation"), &ocgd_gp_Elips::transformed);
        ClassDB::bind_method(D_METHOD("translate", "vector"), &ocgd_gp_Elips::translate);
        ClassDB::bind_method(D_METHOD("translated", "vector"), &ocgd_gp_Elips::translated);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Elips::copy);
    }

public:
    ocgd_gp_Elips() : elips() {}
    ~ocgd_gp_Elips() = default;

    // Initialization methods
    void init_default() {
        elips = gp_Elips();
    }

    void init_with_position_radii(Ref<RefCounted> position, double major_radius, double minor_radius) {
        // Note: This would need ocgd_gp_Ax2 binding
        // For now, we'll create a default ellipse with radii
        gp_Ax2 default_pos;
        elips = gp_Elips(default_pos, major_radius, minor_radius);
    }

    // Setters
    void set_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    void set_location(Vector3 location) {
        gp_Pnt p(location.x, location.y, location.z);
        elips.SetLocation(p);
    }

    void set_major_radius(double radius) {
        elips.SetMajorRadius(radius);
    }

    void set_minor_radius(double radius) {
        elips.SetMinorRadius(radius);
    }

    void set_position(Ref<RefCounted> position) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    // Getters
    double get_area() const {
        return elips.Area();
    }

    Ref<RefCounted> get_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_directrix1() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_directrix2() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    double get_eccentricity() const {
        return elips.Eccentricity();
    }

    double get_focal() const {
        return elips.Focal();
    }

    Vector3 get_focus1() const {
        const gp_Pnt& p = elips.Focus1();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Vector3 get_focus2() const {
        const gp_Pnt& p = elips.Focus2();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Vector3 get_location() const {
        const gp_Pnt& p = elips.Location();
        return Vector3(p.X(), p.Y(), p.Z());
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
        elips.Mirror(p);
    }

    Ref<ocgd_gp_Elips> mirrored_point(Vector3 point) const {
        Ref<ocgd_gp_Elips> result;
        result.instantiate();
        gp_Pnt p(point.x, point.y, point.z);
        result->set_gp_elips(elips.Mirrored(p));
        return result;
    }

    void mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Elips> mirrored_axis(Ref<RefCounted> axis) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Elips> result;
        result.instantiate();
        return result;
    }

    void mirror_plane(Ref<RefCounted> plane) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    Ref<ocgd_gp_Elips> mirrored_plane(Ref<RefCounted> plane) const {
        // Note: This would need ocgd_gp_Ax2 binding
        Ref<ocgd_gp_Elips> result;
        result.instantiate();
        return result;
    }

    void rotate(Ref<RefCounted> axis, double angle) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Elips> rotated(Ref<RefCounted> axis, double angle) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Elips> result;
        result.instantiate();
        return result;
    }

    void scale(Vector3 center, double scale_factor) {
        gp_Pnt p(center.x, center.y, center.z);
        elips.Scale(p, scale_factor);
    }

    Ref<ocgd_gp_Elips> scaled(Vector3 center, double scale_factor) const {
        Ref<ocgd_gp_Elips> result;
        result.instantiate();
        gp_Pnt p(center.x, center.y, center.z);
        result->set_gp_elips(elips.Scaled(p, scale_factor));
        return result;
    }

    void transform(Ref<RefCounted> transformation) {
        // Note: This would need ocgd_gp_Trsf binding
    }

    Ref<ocgd_gp_Elips> transformed(Ref<RefCounted> transformation) const {
        // Note: This would need ocgd_gp_Trsf binding
        Ref<ocgd_gp_Elips> result;
        result.instantiate();
        return result;
    }

    void translate(Vector3 vector) {
        gp_Vec v(vector.x, vector.y, vector.z);
        elips.Translate(v);
    }

    Ref<ocgd_gp_Elips> translated(Vector3 vector) const {
        Ref<ocgd_gp_Elips> result;
        result.instantiate();
        gp_Vec v(vector.x, vector.y, vector.z);
        result->set_gp_elips(elips.Translated(v));
        return result;
    }

    // Utility
    Ref<ocgd_gp_Elips> copy() const {
        Ref<ocgd_gp_Elips> result;
        result.instantiate();
        result->set_gp_elips(elips);
        return result;
    }

    // Internal access
    const gp_Elips& get_gp_elips() const { return elips; }
    void set_gp_elips(const gp_Elips& e) { elips = e; }
};

#endif // OCGD_GP_ELIPS_H