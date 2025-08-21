#ifndef OCGD_GP_TRSF2D_H
#define OCGD_GP_TRSF2D_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Trsf2d.hxx>
#include <gp_TrsfForm.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Mat2d.hxx>
#include <gp_XY.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Trsf2d (2D transformation)
 *
 * Defines a non-persistent transformation in 2D space.
 * The following transformations are implemented:
 * - Translation, Rotation, Scale
 * - Symmetry with respect to a point and a line
 *
 * Complex transformations can be obtained by combining the
 * previous elementary transformations using the method Multiply.
 */
class ocgd_gp_Trsf2d : public RefCounted {
    GDCLASS(ocgd_gp_Trsf2d, RefCounted)

private:
    gp_Trsf2d trsf;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_identity"), &ocgd_gp_Trsf2d::init_identity);
        ClassDB::bind_method(D_METHOD("init_from_3d", "trsf_3d"), &ocgd_gp_Trsf2d::init_from_3d);

        // Mirror transformations
        ClassDB::bind_method(D_METHOD("set_mirror_point", "point"), &ocgd_gp_Trsf2d::set_mirror_point);
        ClassDB::bind_method(D_METHOD("set_mirror_axis", "axis"), &ocgd_gp_Trsf2d::set_mirror_axis);

        // Rotation transformation
        ClassDB::bind_method(D_METHOD("set_rotation", "center", "angle"), &ocgd_gp_Trsf2d::set_rotation);

        // Scale transformation
        ClassDB::bind_method(D_METHOD("set_scale", "center", "scale"), &ocgd_gp_Trsf2d::set_scale);
        ClassDB::bind_method(D_METHOD("set_scale_factor", "scale"), &ocgd_gp_Trsf2d::set_scale_factor);

        // Translation transformations
        ClassDB::bind_method(D_METHOD("set_translation_vector", "vector"), &ocgd_gp_Trsf2d::set_translation_vector);
        ClassDB::bind_method(D_METHOD("set_translation_points", "p1", "p2"), &ocgd_gp_Trsf2d::set_translation_points);
        ClassDB::bind_method(D_METHOD("set_translation_part", "vector"), &ocgd_gp_Trsf2d::set_translation_part);

        // Coordinate system transformations
        ClassDB::bind_method(D_METHOD("set_transformation_to_system", "to_system"), &ocgd_gp_Trsf2d::set_transformation_to_system);
        ClassDB::bind_method(D_METHOD("set_transformation_between_systems", "from_system", "to_system"), &ocgd_gp_Trsf2d::set_transformation_between_systems);

        // Set values
        ClassDB::bind_method(D_METHOD("set_values", "a11", "a12", "a13", "a21", "a22", "a23"), &ocgd_gp_Trsf2d::set_values);

        // Queries
        ClassDB::bind_method(D_METHOD("is_negative"), &ocgd_gp_Trsf2d::is_negative);
        ClassDB::bind_method(D_METHOD("get_form"), &ocgd_gp_Trsf2d::get_form);
        ClassDB::bind_method(D_METHOD("get_scale_factor"), &ocgd_gp_Trsf2d::get_scale_factor);
        ClassDB::bind_method(D_METHOD("get_translation_part"), &ocgd_gp_Trsf2d::get_translation_part);
        ClassDB::bind_method(D_METHOD("get_vectorial_part"), &ocgd_gp_Trsf2d::get_vectorial_part);
        ClassDB::bind_method(D_METHOD("get_rotation_part"), &ocgd_gp_Trsf2d::get_rotation_part);
        ClassDB::bind_method(D_METHOD("get_value", "row", "col"), &ocgd_gp_Trsf2d::get_value);

        // Operations
        ClassDB::bind_method(D_METHOD("invert"), &ocgd_gp_Trsf2d::invert);
        ClassDB::bind_method(D_METHOD("inverted"), &ocgd_gp_Trsf2d::inverted);
        ClassDB::bind_method(D_METHOD("multiply", "other"), &ocgd_gp_Trsf2d::multiply);
        ClassDB::bind_method(D_METHOD("multiplied", "other"), &ocgd_gp_Trsf2d::multiplied);
        ClassDB::bind_method(D_METHOD("pre_multiply", "other"), &ocgd_gp_Trsf2d::pre_multiply);
        ClassDB::bind_method(D_METHOD("power", "n"), &ocgd_gp_Trsf2d::power);
        ClassDB::bind_method(D_METHOD("powered", "n"), &ocgd_gp_Trsf2d::powered);

        // Transform operations
        ClassDB::bind_method(D_METHOD("transforms_xy", "x", "y"), &ocgd_gp_Trsf2d::transforms_xy);
        ClassDB::bind_method(D_METHOD("transforms_vector2", "coords"), &ocgd_gp_Trsf2d::transforms_vector2);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Trsf2d::copy);
    }

public:
    ocgd_gp_Trsf2d() : trsf() {}
    ~ocgd_gp_Trsf2d() = default;

    // Initialization
    void init_identity() {
        trsf = gp_Trsf2d();
    }

    void init_from_3d(Ref<RefCounted> trsf_3d) {
        // Note: This would need ocgd_gp_Trsf binding
        // For now, we'll keep it as a placeholder
    }

    // Mirror transformations
    void set_mirror_point(Vector2 point) {
        gp_Pnt2d p(point.x, point.y);
        trsf.SetMirror(p);
    }

    void set_mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax2d binding
        // For now, we'll skip complex parameter types that aren't bound yet
    }

    // Rotation transformation
    void set_rotation(Vector2 center, double angle) {
        gp_Pnt2d p(center.x, center.y);
        trsf.SetRotation(p, angle);
    }

    // Scale transformation
    void set_scale(Vector2 center, double scale) {
        gp_Pnt2d p(center.x, center.y);
        trsf.SetScale(p, scale);
    }

    void set_scale_factor(double scale) {
        trsf.SetScaleFactor(scale);
    }

    // Translation transformations
    void set_translation_vector(Vector2 vector) {
        gp_Vec2d v(vector.x, vector.y);
        trsf.SetTranslation(v);
    }

    void set_translation_points(Vector2 p1, Vector2 p2) {
        gp_Pnt2d point1(p1.x, p1.y);
        gp_Pnt2d point2(p2.x, p2.y);
        trsf.SetTranslation(point1, point2);
    }

    void set_translation_part(Vector2 vector) {
        gp_Vec2d v(vector.x, vector.y);
        trsf.SetTranslationPart(v);
    }

    // Coordinate system transformations
    void set_transformation_to_system(Ref<RefCounted> to_system) {
        // Note: This would need ocgd_gp_Ax2d binding
    }

    void set_transformation_between_systems(Ref<RefCounted> from_system, Ref<RefCounted> to_system) {
        // Note: This would need ocgd_gp_Ax2d binding
    }

    // Set values
    void set_values(double a11, double a12, double a13,
                   double a21, double a22, double a23) {
        trsf.SetValues(a11, a12, a13, a21, a22, a23);
    }

    // Queries
    bool is_negative() const {
        return trsf.IsNegative();
    }

    int get_form() const {
        return static_cast<int>(trsf.Form());
    }

    double get_scale_factor() const {
        return trsf.ScaleFactor();
    }

    Vector2 get_translation_part() const {
        const gp_XY& xy = trsf.TranslationPart();
        return Vector2(xy.X(), xy.Y());
    }

    Ref<RefCounted> get_vectorial_part() const {
        // Note: This would return ocgd_gp_Mat2d
        return Ref<RefCounted>();
    }

    double get_rotation_part() const {
        return trsf.RotationPart();
    }

    double get_value(int row, int col) const {
        return trsf.Value(row, col);
    }

    // Operations
    void invert() {
        trsf.Invert();
    }

    Ref<ocgd_gp_Trsf2d> inverted() const {
        Ref<ocgd_gp_Trsf2d> result;
        result.instantiate();
        result->set_gp_trsf(trsf.Inverted());
        return result;
    }

    void multiply(Ref<ocgd_gp_Trsf2d> other) {
        if (other.is_valid()) {
            trsf.Multiply(other->get_gp_trsf());
        }
    }

    Ref<ocgd_gp_Trsf2d> multiplied(Ref<ocgd_gp_Trsf2d> other) const {
        Ref<ocgd_gp_Trsf2d> result;
        result.instantiate();
        if (other.is_valid()) {
            result->set_gp_trsf(trsf.Multiplied(other->get_gp_trsf()));
        }
        return result;
    }

    void pre_multiply(Ref<ocgd_gp_Trsf2d> other) {
        if (other.is_valid()) {
            trsf.PreMultiply(other->get_gp_trsf());
        }
    }

    void power(int n) {
        trsf.Power(n);
    }

    Ref<ocgd_gp_Trsf2d> powered(int n) const {
        Ref<ocgd_gp_Trsf2d> result;
        result.instantiate();
        gp_Trsf2d temp_trsf = trsf;
        result->set_gp_trsf(temp_trsf.Powered(n));
        return result;
    }

    // Transform operations
    Array transforms_xy(double x, double y) const {
        Standard_Real tx = x, ty = y;
        trsf.Transforms(tx, ty);
        Array result;
        result.push_back(tx);
        result.push_back(ty);
        return result;
    }

    Vector2 transforms_vector2(Vector2 coords) const {
        gp_XY xy(coords.x, coords.y);
        trsf.Transforms(xy);
        return Vector2(xy.X(), xy.Y());
    }

    // Utility
    Ref<ocgd_gp_Trsf2d> copy() const {
        Ref<ocgd_gp_Trsf2d> result;
        result.instantiate();
        result->set_gp_trsf(trsf);
        return result;
    }

    // Internal access
    const gp_Trsf2d& get_gp_trsf() const { return trsf; }
    void set_gp_trsf(const gp_Trsf2d& t) { trsf = t; }
};

#endif // OCGD_GP_TRSF2D_H
