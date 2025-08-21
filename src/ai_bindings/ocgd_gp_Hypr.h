#ifndef OCGD_GP_HYPR_H
#define OCGD_GP_HYPR_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Hypr.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Hypr (3D hyperbola)
 * 
 * Describes a hyperbola in 3D space.
 * A hyperbola is defined by its major and minor radii and positioned in space
 * with a coordinate system (a gp_Ax2 object) where:
 * - the origin of the coordinate system is the center of the hyperbola
 * - the "X Axis" of the coordinate system is the major axis of the hyperbola
 * - the "Y Axis" of the coordinate system is the minor axis of the hyperbola
 */
class ocgd_gp_Hypr : public RefCounted {
    GDCLASS(ocgd_gp_Hypr, RefCounted)

private:
    gp_Hypr hypr;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Hypr::init_default);
        ClassDB::bind_method(D_METHOD("init_with_position_radii", "position", "major_radius", "minor_radius"), &ocgd_gp_Hypr::init_with_position_radii);

        // Setters
        ClassDB::bind_method(D_METHOD("set_axis", "axis"), &ocgd_gp_Hypr::set_axis);
        ClassDB::bind_method(D_METHOD("set_location", "location"), &ocgd_gp_Hypr::set_location);
        ClassDB::bind_method(D_METHOD("set_major_radius", "radius"), &ocgd_gp_Hypr::set_major_radius);
        ClassDB::bind_method(D_METHOD("set_minor_radius", "radius"), &ocgd_gp_Hypr::set_minor_radius);
        ClassDB::bind_method(D_METHOD("set_position", "position"), &ocgd_gp_Hypr::set_position);

        // Getters
        ClassDB::bind_method(D_METHOD("get_axis"), &ocgd_gp_Hypr::get_axis);
        ClassDB::bind_method(D_METHOD("get_asymptote1"), &ocgd_gp_Hypr::get_asymptote1);
        ClassDB::bind_method(D_METHOD("get_asymptote2"), &ocgd_gp_Hypr::get_asymptote2);
        ClassDB::bind_method(D_METHOD("get_conjugate_branch1"), &ocgd_gp_Hypr::get_conjugate_branch1);
        ClassDB::bind_method(D_METHOD("get_conjugate_branch2"), &ocgd_gp_Hypr::get_conjugate_branch2);
        ClassDB::bind_method(D_METHOD("get_directrix1"), &ocgd_gp_Hypr::get_directrix1);
        ClassDB::bind_method(D_METHOD("get_directrix2"), &ocgd_gp_Hypr::get_directrix2);
        ClassDB::bind_method(D_METHOD("get_eccentricity"), &ocgd_gp_Hypr::get_eccentricity);
        ClassDB::bind_method(D_METHOD("get_focal"), &ocgd_gp_Hypr::get_focal);
        ClassDB::bind_method(D_METHOD("get_focus1"), &ocgd_gp_Hypr::get_focus1);
        ClassDB::bind_method(D_METHOD("get_focus2"), &ocgd_gp_Hypr::get_focus2);
        ClassDB::bind_method(D_METHOD("get_location"), &ocgd_gp_Hypr::get_location);
        ClassDB::bind_method(D_METHOD("get_major_radius"), &ocgd_gp_Hypr::get_major_radius);
        ClassDB::bind_method(D_METHOD("get_minor_radius"), &ocgd_gp_Hypr::get_minor_radius);
        ClassDB::bind_method(D_METHOD("get_other_branch"), &ocgd_gp_Hypr::get_other_branch);
        ClassDB::bind_method(D_METHOD("get_parameter"), &ocgd_gp_Hypr::get_parameter);
        ClassDB::bind_method(D_METHOD("get_position"), &ocgd_gp_Hypr::get_position);
        ClassDB::bind_method(D_METHOD("get_x_axis"), &ocgd_gp_Hypr::get_x_axis);
        ClassDB::bind_method(D_METHOD("get_y_axis"), &ocgd_gp_Hypr::get_y_axis);

        // Transformations
        ClassDB::bind_method(D_METHOD("mirror_point", "point"), &ocgd_gp_Hypr::mirror_point);
        ClassDB::bind_method(D_METHOD("mirrored_point", "point"), &ocgd_gp_Hypr::mirrored_point);
        ClassDB::bind_method(D_METHOD("mirror_axis", "axis"), &ocgd_gp_Hypr::mirror_axis);
        ClassDB::bind_method(D_METHOD("mirrored_axis", "axis"), &ocgd_gp_Hypr::mirrored_axis);
        ClassDB::bind_method(D_METHOD("mirror_plane", "plane"), &ocgd_gp_Hypr::mirror_plane);
        ClassDB::bind_method(D_METHOD("mirrored_plane", "plane"), &ocgd_gp_Hypr::mirrored_plane);
        ClassDB::bind_method(D_METHOD("rotate", "axis", "angle"), &ocgd_gp_Hypr::rotate);
        ClassDB::bind_method(D_METHOD("rotated", "axis", "angle"), &ocgd_gp_Hypr::rotated);
        ClassDB::bind_method(D_METHOD("scale", "center", "scale_factor"), &ocgd_gp_Hypr::scale);
        ClassDB::bind_method(D_METHOD("scaled", "center", "scale_factor"), &ocgd_gp_Hypr::scaled);
        ClassDB::bind_method(D_METHOD("transform", "transformation"), &ocgd_gp_Hypr::transform);
        ClassDB::bind_method(D_METHOD("transformed", "transformation"), &ocgd_gp_Hypr::transformed);
        ClassDB::bind_method(D_METHOD("translate", "vector"), &ocgd_gp_Hypr::translate);
        ClassDB::bind_method(D_METHOD("translated", "vector"), &ocgd_gp_Hypr::translated);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Hypr::copy);
    }

public:
    ocgd_gp_Hypr() : hypr() {}
    ~ocgd_gp_Hypr() = default;

    // Initialization methods
    void init_default() {
        hypr = gp_Hypr();
    }

    void init_with_position_radii(Ref<RefCounted> position, double major_radius, double minor_radius) {
        // Note: This would need ocgd_gp_Ax2 binding
        // For now, we'll create a default hyperbola with radii
        gp_Ax2 default_pos;
        hypr = gp_Hypr(default_pos, major_radius, minor_radius);
    }

    // Setters
    void set_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    void set_location(Vector3 location) {
        gp_Pnt p(location.x, location.y, location.z);
        hypr.SetLocation(p);
    }

    void set_major_radius(double radius) {
        hypr.SetMajorRadius(radius);
    }

    void set_minor_radius(double radius) {
        hypr.SetMinorRadius(radius);
    }

    void set_position(Ref<RefCounted> position) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    // Getters
    Ref<RefCounted> get_axis() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_asymptote1() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Ref<RefCounted> get_asymptote2() const {
        // Note: This would return ocgd_gp_Ax1
        return Ref<RefCounted>();
    }

    Ref<ocgd_gp_Hypr> get_conjugate_branch1() const {
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        result->set_gp_hypr(hypr.ConjugateBranch1());
        return result;
    }

    Ref<ocgd_gp_Hypr> get_conjugate_branch2() const {
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        result->set_gp_hypr(hypr.ConjugateBranch2());
        return result;
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
        return hypr.Eccentricity();
    }

    double get_focal() const {
        return hypr.Focal();
    }

    Vector3 get_focus1() const {
        const gp_Pnt& p = hypr.Focus1();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Vector3 get_focus2() const {
        const gp_Pnt& p = hypr.Focus2();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    Vector3 get_location() const {
        const gp_Pnt& p = hypr.Location();
        return Vector3(p.X(), p.Y(), p.Z());
    }

    double get_major_radius() const {
        return hypr.MajorRadius();
    }

    double get_minor_radius() const {
        return hypr.MinorRadius();
    }

    Ref<ocgd_gp_Hypr> get_other_branch() const {
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        result->set_gp_hypr(hypr.OtherBranch());
        return result;
    }

    double get_parameter() const {
        return hypr.Parameter();
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
        hypr.Mirror(p);
    }

    Ref<ocgd_gp_Hypr> mirrored_point(Vector3 point) const {
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        gp_Pnt p(point.x, point.y, point.z);
        result->set_gp_hypr(hypr.Mirrored(p));
        return result;
    }

    void mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Hypr> mirrored_axis(Ref<RefCounted> axis) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        return result;
    }

    void mirror_plane(Ref<RefCounted> plane) {
        // Note: This would need ocgd_gp_Ax2 binding
    }

    Ref<ocgd_gp_Hypr> mirrored_plane(Ref<RefCounted> plane) const {
        // Note: This would need ocgd_gp_Ax2 binding
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        return result;
    }

    void rotate(Ref<RefCounted> axis, double angle) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    Ref<ocgd_gp_Hypr> rotated(Ref<RefCounted> axis, double angle) const {
        // Note: This would need ocgd_gp_Ax1 binding
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        return result;
    }

    void scale(Vector3 center, double scale_factor) {
        gp_Pnt p(center.x, center.y, center.z);
        hypr.Scale(p, scale_factor);
    }

    Ref<ocgd_gp_Hypr> scaled(Vector3 center, double scale_factor) const {
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        gp_Pnt p(center.x, center.y, center.z);
        result->set_gp_hypr(hypr.Scaled(p, scale_factor));
        return result;
    }

    void transform(Ref<RefCounted> transformation) {
        // Note: This would need ocgd_gp_Trsf binding
    }

    Ref<ocgd_gp_Hypr> transformed(Ref<RefCounted> transformation) const {
        // Note: This would need ocgd_gp_Trsf binding
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        return result;
    }

    void translate(Vector3 vector) {
        gp_Vec v(vector.x, vector.y, vector.z);
        hypr.Translate(v);
    }

    Ref<ocgd_gp_Hypr> translated(Vector3 vector) const {
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        gp_Vec v(vector.x, vector.y, vector.z);
        result->set_gp_hypr(hypr.Translated(v));
        return result;
    }

    // Utility
    Ref<ocgd_gp_Hypr> copy() const {
        Ref<ocgd_gp_Hypr> result;
        result.instantiate();
        result->set_gp_hypr(hypr);
        return result;
    }

    // Internal access
    const gp_Hypr& get_gp_hypr() const { return hypr; }
    void set_gp_hypr(const gp_Hypr& h) { hypr = h; }
};

#endif // OCGD_GP_HYPR_H