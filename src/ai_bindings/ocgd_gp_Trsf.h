#ifndef OCGD_GP_TRSF_H
#define OCGD_GP_TRSF_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Trsf.hxx>
#include <gp_TrsfForm.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Mat.hxx>
#include <gp_XYZ.hxx>
#include <gp_Quaternion.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Trsf (3D transformation)
 * 
 * Defines a non-persistent transformation in 3D space.
 * The following transformations are implemented:
 * - Translation, Rotation, Scale
 * - Symmetry with respect to a point, a line, a plane
 * 
 * Complex transformations can be obtained by combining the
 * previous elementary transformations using the method Multiply.
 */
class ocgd_gp_Trsf : public RefCounted {
    GDCLASS(ocgd_gp_Trsf, RefCounted)

private:
    gp_Trsf trsf;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_identity"), &ocgd_gp_Trsf::init_identity);

        // Mirror transformations
        ClassDB::bind_method(D_METHOD("set_mirror_point", "point"), &ocgd_gp_Trsf::set_mirror_point);
        ClassDB::bind_method(D_METHOD("set_mirror_axis", "axis"), &ocgd_gp_Trsf::set_mirror_axis);
        ClassDB::bind_method(D_METHOD("set_mirror_plane", "plane"), &ocgd_gp_Trsf::set_mirror_plane);

        // Rotation transformations
        ClassDB::bind_method(D_METHOD("set_rotation_axis", "axis", "angle"), &ocgd_gp_Trsf::set_rotation_axis);
        ClassDB::bind_method(D_METHOD("set_rotation_quaternion", "quaternion"), &ocgd_gp_Trsf::set_rotation_quaternion);
        ClassDB::bind_method(D_METHOD("set_rotation_part_quaternion", "quaternion"), &ocgd_gp_Trsf::set_rotation_part_quaternion);

        // Scale transformation
        ClassDB::bind_method(D_METHOD("set_scale", "center", "scale"), &ocgd_gp_Trsf::set_scale);
        ClassDB::bind_method(D_METHOD("set_scale_factor", "scale"), &ocgd_gp_Trsf::set_scale_factor);

        // Translation transformations
        ClassDB::bind_method(D_METHOD("set_translation_vector", "vector"), &ocgd_gp_Trsf::set_translation_vector);
        ClassDB::bind_method(D_METHOD("set_translation_points", "p1", "p2"), &ocgd_gp_Trsf::set_translation_points);
        ClassDB::bind_method(D_METHOD("set_translation_part", "vector"), &ocgd_gp_Trsf::set_translation_part);

        // Coordinate system transformations
        ClassDB::bind_method(D_METHOD("set_displacement", "from_system", "to_system"), &ocgd_gp_Trsf::set_displacement);
        ClassDB::bind_method(D_METHOD("set_transformation_to_system", "to_system"), &ocgd_gp_Trsf::set_transformation_to_system);
        ClassDB::bind_method(D_METHOD("set_transformation_between_systems", "from_system", "to_system"), &ocgd_gp_Trsf::set_transformation_between_systems);
        ClassDB::bind_method(D_METHOD("set_transformation_quaternion_vector", "rotation", "translation"), &ocgd_gp_Trsf::set_transformation_quaternion_vector);

        // Set form and values
        ClassDB::bind_method(D_METHOD("set_form", "form"), &ocgd_gp_Trsf::set_form);
        ClassDB::bind_method(D_METHOD("set_values", "a11", "a12", "a13", "a14", "a21", "a22", "a23", "a24", "a31", "a32", "a33", "a34"), &ocgd_gp_Trsf::set_values);

        // Queries
        ClassDB::bind_method(D_METHOD("is_negative"), &ocgd_gp_Trsf::is_negative);
        ClassDB::bind_method(D_METHOD("get_form"), &ocgd_gp_Trsf::get_form);
        ClassDB::bind_method(D_METHOD("get_scale_factor"), &ocgd_gp_Trsf::get_scale_factor);
        ClassDB::bind_method(D_METHOD("get_translation_part"), &ocgd_gp_Trsf::get_translation_part);
        ClassDB::bind_method(D_METHOD("get_rotation"), &ocgd_gp_Trsf::get_rotation);
        ClassDB::bind_method(D_METHOD("get_rotation_axis_angle"), &ocgd_gp_Trsf::get_rotation_axis_angle);
        ClassDB::bind_method(D_METHOD("get_vectorial_part"), &ocgd_gp_Trsf::get_vectorial_part);
        ClassDB::bind_method(D_METHOD("get_value", "row", "col"), &ocgd_gp_Trsf::get_value);

        // Operations
        ClassDB::bind_method(D_METHOD("invert"), &ocgd_gp_Trsf::invert);
        ClassDB::bind_method(D_METHOD("inverted"), &ocgd_gp_Trsf::inverted);
        ClassDB::bind_method(D_METHOD("multiply", "other"), &ocgd_gp_Trsf::multiply);
        ClassDB::bind_method(D_METHOD("multiplied", "other"), &ocgd_gp_Trsf::multiplied);
        ClassDB::bind_method(D_METHOD("pre_multiply", "other"), &ocgd_gp_Trsf::pre_multiply);
        ClassDB::bind_method(D_METHOD("power", "n"), &ocgd_gp_Trsf::power);
        ClassDB::bind_method(D_METHOD("powered", "n"), &ocgd_gp_Trsf::powered);

        // Transform operations
        ClassDB::bind_method(D_METHOD("transforms_xyz", "x", "y", "z"), &ocgd_gp_Trsf::transforms_xyz);
        ClassDB::bind_method(D_METHOD("transforms_vector3", "coords"), &ocgd_gp_Trsf::transforms_vector3);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Trsf::copy);
    }

public:
    ocgd_gp_Trsf() : trsf() {}
    ~ocgd_gp_Trsf() = default;

    // Initialization
    void init_identity() {
        trsf = gp_Trsf();
    }

    // Mirror transformations
    void set_mirror_point(Vector3 point) {
        gp_Pnt p(point.x, point.y, point.z);
        trsf.SetMirror(p);
    }

    void set_mirror_axis(Ref<RefCounted> axis) {
        // Note: This would need ocgd_gp_Ax1 binding
        // For now, we'll skip complex parameter types that aren't bound yet
    }

    void set_mirror_plane(Ref<RefCounted> plane) {
        // Note: This would need ocgd_gp_Ax2 binding
        // For now, we'll skip complex parameter types that aren't bound yet
    }

    // Rotation transformations
    void set_rotation_axis(Ref<RefCounted> axis, double angle) {
        // Note: This would need ocgd_gp_Ax1 binding
    }

    void set_rotation_quaternion(Array quaternion) {
        if (quaternion.size() >= 4) {
            gp_Quaternion q(
                quaternion[1].operator double(),  // x
                quaternion[2].operator double(),  // y
                quaternion[3].operator double(),  // z
                quaternion[0].operator double()   // w
            );
            trsf.SetRotation(q);
        }
    }

    void set_rotation_part_quaternion(Array quaternion) {
        if (quaternion.size() >= 4) {
            gp_Quaternion q(
                quaternion[1].operator double(),  // x
                quaternion[2].operator double(),  // y
                quaternion[3].operator double(),  // z
                quaternion[0].operator double()   // w
            );
            trsf.SetRotationPart(q);
        }
    }

    // Scale transformation
    void set_scale(Vector3 center, double scale) {
        gp_Pnt p(center.x, center.y, center.z);
        trsf.SetScale(p, scale);
    }

    void set_scale_factor(double scale) {
        trsf.SetScaleFactor(scale);
    }

    // Translation transformations
    void set_translation_vector(Vector3 vector) {
        gp_Vec v(vector.x, vector.y, vector.z);
        trsf.SetTranslation(v);
    }

    void set_translation_points(Vector3 p1, Vector3 p2) {
        gp_Pnt point1(p1.x, p1.y, p1.z);
        gp_Pnt point2(p2.x, p2.y, p2.z);
        trsf.SetTranslation(point1, point2);
    }

    void set_translation_part(Vector3 vector) {
        gp_Vec v(vector.x, vector.y, vector.z);
        trsf.SetTranslationPart(v);
    }

    // Coordinate system transformations
    void set_displacement(Ref<RefCounted> from_system, Ref<RefCounted> to_system) {
        // Note: This would need ocgd_gp_Ax3 binding
    }

    void set_transformation_to_system(Ref<RefCounted> to_system) {
        // Note: This would need ocgd_gp_Ax3 binding
    }

    void set_transformation_between_systems(Ref<RefCounted> from_system, Ref<RefCounted> to_system) {
        // Note: This would need ocgd_gp_Ax3 binding
    }

    void set_transformation_quaternion_vector(Array rotation, Vector3 translation) {
        if (rotation.size() >= 4) {
            gp_Quaternion q(
                rotation[1].operator double(),  // x
                rotation[2].operator double(),  // y
                rotation[3].operator double(),  // z
                rotation[0].operator double()   // w
            );
            gp_Vec t(translation.x, translation.y, translation.z);
            trsf.SetTransformation(q, t);
        }
    }

    // Set form and values
    void set_form(int form) {
        trsf.SetForm(static_cast<gp_TrsfForm>(form));
    }

    void set_values(double a11, double a12, double a13, double a14,
                   double a21, double a22, double a23, double a24,
                   double a31, double a32, double a33, double a34) {
        trsf.SetValues(a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34);
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

    Vector3 get_translation_part() const {
        const gp_XYZ& xyz = trsf.TranslationPart();
        return Vector3(xyz.X(), xyz.Y(), xyz.Z());
    }

    Array get_rotation() const {
        gp_Quaternion q = trsf.GetRotation();
        Array result;
        result.push_back(q.W());  // w
        result.push_back(q.X());  // x
        result.push_back(q.Y());  // y
        result.push_back(q.Z());  // z
        return result;
    }

    Dictionary get_rotation_axis_angle() const {
        gp_XYZ axis;
        Standard_Real angle;
        Dictionary result;
        bool has_rotation = trsf.GetRotation(axis, angle);
        result["has_rotation"] = has_rotation;
        if (has_rotation) {
            result["axis"] = Vector3(axis.X(), axis.Y(), axis.Z());
            result["angle"] = angle;
        }
        return result;
    }

    Ref<RefCounted> get_vectorial_part() const {
        // Note: This would return ocgd_gp_Mat
        return Ref<RefCounted>();
    }

    double get_value(int row, int col) const {
        return trsf.Value(row, col);
    }

    // Operations
    void invert() {
        trsf.Invert();
    }

    Ref<ocgd_gp_Trsf> inverted() const {
        Ref<ocgd_gp_Trsf> result;
        result.instantiate();
        result->set_gp_trsf(trsf.Inverted());
        return result;
    }

    void multiply(Ref<ocgd_gp_Trsf> other) {
        if (other.is_valid()) {
            trsf.Multiply(other->get_gp_trsf());
        }
    }

    Ref<ocgd_gp_Trsf> multiplied(Ref<ocgd_gp_Trsf> other) const {
        Ref<ocgd_gp_Trsf> result;
        result.instantiate();
        if (other.is_valid()) {
            result->set_gp_trsf(trsf.Multiplied(other->get_gp_trsf()));
        }
        return result;
    }

    void pre_multiply(Ref<ocgd_gp_Trsf> other) {
        if (other.is_valid()) {
            trsf.PreMultiply(other->get_gp_trsf());
        }
    }

    void power(int n) {
        trsf.Power(n);
    }

    Ref<ocgd_gp_Trsf> powered(int n) const {
        Ref<ocgd_gp_Trsf> result;
        result.instantiate();
        gp_Trsf temp_trsf = trsf;
        result->set_gp_trsf(temp_trsf.Powered(n));
        return result;
    }

    // Transform operations
    Array transforms_xyz(double x, double y, double z) const {
        Standard_Real tx = x, ty = y, tz = z;
        trsf.Transforms(tx, ty, tz);
        Array result;
        result.push_back(tx);
        result.push_back(ty);
        result.push_back(tz);
        return result;
    }

    Vector3 transforms_vector3(Vector3 coords) const {
        gp_XYZ xyz(coords.x, coords.y, coords.z);
        trsf.Transforms(xyz);
        return Vector3(xyz.X(), xyz.Y(), xyz.Z());
    }

    // Utility
    Ref<ocgd_gp_Trsf> copy() const {
        Ref<ocgd_gp_Trsf> result;
        result.instantiate();
        result->set_gp_trsf(trsf);
        return result;
    }

    // Internal access
    const gp_Trsf& get_gp_trsf() const { return trsf; }
    void set_gp_trsf(const gp_Trsf& t) { trsf = t; }
};

#endif // OCGD_GP_TRSF_H