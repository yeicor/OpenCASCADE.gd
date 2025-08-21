#ifndef OCGD_GP_QUATERNION_H
#define OCGD_GP_QUATERNION_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Quaternion.hxx>
#include <gp_Vec.hxx>
#include <gp_Mat.hxx>
#include <gp_EulerSequence.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Quaternion (quaternion rotation)
 * 
 * Represents operation of rotation in 3d space as quaternion
 * and implements operations with rotations basing on
 * quaternion mathematics.
 * 
 * In addition, provides methods for conversion to and from other
 * representations of rotation (3*3 matrix, vector and
 * angle, Euler angles)
 */
class ocgd_gp_Quaternion : public RefCounted {
    GDCLASS(ocgd_gp_Quaternion, RefCounted)

private:
    gp_Quaternion quat;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_identity"), &ocgd_gp_Quaternion::init_identity);
        ClassDB::bind_method(D_METHOD("init_components", "x", "y", "z", "w"), &ocgd_gp_Quaternion::init_components);
        ClassDB::bind_method(D_METHOD("init_from_vectors", "vec_from", "vec_to"), &ocgd_gp_Quaternion::init_from_vectors);
        ClassDB::bind_method(D_METHOD("init_from_vectors_with_help", "vec_from", "vec_to", "help_cross_vec"), &ocgd_gp_Quaternion::init_from_vectors_with_help);
        ClassDB::bind_method(D_METHOD("init_from_axis_angle", "axis", "angle"), &ocgd_gp_Quaternion::init_from_axis_angle);
        ClassDB::bind_method(D_METHOD("init_from_matrix", "matrix"), &ocgd_gp_Quaternion::init_from_matrix);

        // Setters
        ClassDB::bind_method(D_METHOD("set_components", "x", "y", "z", "w"), &ocgd_gp_Quaternion::set_components);
        ClassDB::bind_method(D_METHOD("set_rotation_vectors", "vec_from", "vec_to"), &ocgd_gp_Quaternion::set_rotation_vectors);
        ClassDB::bind_method(D_METHOD("set_rotation_vectors_with_help", "vec_from", "vec_to", "help_cross_vec"), &ocgd_gp_Quaternion::set_rotation_vectors_with_help);
        ClassDB::bind_method(D_METHOD("set_vector_and_angle", "axis", "angle"), &ocgd_gp_Quaternion::set_vector_and_angle);
        ClassDB::bind_method(D_METHOD("set_matrix", "matrix"), &ocgd_gp_Quaternion::set_matrix);
        ClassDB::bind_method(D_METHOD("set_euler_angles", "order", "alpha", "beta", "gamma"), &ocgd_gp_Quaternion::set_euler_angles);
        ClassDB::bind_method(D_METHOD("set_identity"), &ocgd_gp_Quaternion::set_identity);

        // Getters
        ClassDB::bind_method(D_METHOD("get_x"), &ocgd_gp_Quaternion::get_x);
        ClassDB::bind_method(D_METHOD("get_y"), &ocgd_gp_Quaternion::get_y);
        ClassDB::bind_method(D_METHOD("get_z"), &ocgd_gp_Quaternion::get_z);
        ClassDB::bind_method(D_METHOD("get_w"), &ocgd_gp_Quaternion::get_w);
        ClassDB::bind_method(D_METHOD("get_vector_and_angle"), &ocgd_gp_Quaternion::get_vector_and_angle);
        ClassDB::bind_method(D_METHOD("get_matrix"), &ocgd_gp_Quaternion::get_matrix);
        ClassDB::bind_method(D_METHOD("get_euler_angles", "order"), &ocgd_gp_Quaternion::get_euler_angles);

        // Queries
        ClassDB::bind_method(D_METHOD("is_equal", "other"), &ocgd_gp_Quaternion::is_equal);
        ClassDB::bind_method(D_METHOD("square_norm"), &ocgd_gp_Quaternion::square_norm);
        ClassDB::bind_method(D_METHOD("norm"), &ocgd_gp_Quaternion::norm);

        // Operations
        ClassDB::bind_method(D_METHOD("reverse"), &ocgd_gp_Quaternion::reverse);
        ClassDB::bind_method(D_METHOD("reversed"), &ocgd_gp_Quaternion::reversed);
        ClassDB::bind_method(D_METHOD("invert"), &ocgd_gp_Quaternion::invert);
        ClassDB::bind_method(D_METHOD("inverted"), &ocgd_gp_Quaternion::inverted);
        ClassDB::bind_method(D_METHOD("scale", "factor"), &ocgd_gp_Quaternion::scale);
        ClassDB::bind_method(D_METHOD("scaled", "factor"), &ocgd_gp_Quaternion::scaled);
        ClassDB::bind_method(D_METHOD("stabilize_length"), &ocgd_gp_Quaternion::stabilize_length);
        ClassDB::bind_method(D_METHOD("normalize"), &ocgd_gp_Quaternion::normalize);
        ClassDB::bind_method(D_METHOD("normalized"), &ocgd_gp_Quaternion::normalized);
        ClassDB::bind_method(D_METHOD("negated"), &ocgd_gp_Quaternion::negated);
        ClassDB::bind_method(D_METHOD("added", "other"), &ocgd_gp_Quaternion::added);
        ClassDB::bind_method(D_METHOD("subtracted", "other"), &ocgd_gp_Quaternion::subtracted);
        ClassDB::bind_method(D_METHOD("multiplied", "other"), &ocgd_gp_Quaternion::multiplied);
        ClassDB::bind_method(D_METHOD("multiply", "other"), &ocgd_gp_Quaternion::multiply);

        // Interpolation
        ClassDB::bind_method(D_METHOD("slerp", "other", "t"), &ocgd_gp_Quaternion::slerp);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Quaternion::copy);
        ClassDB::bind_method(D_METHOD("to_array"), &ocgd_gp_Quaternion::to_array);
        ClassDB::bind_method(D_METHOD("from_array", "array"), &ocgd_gp_Quaternion::from_array);
    }

public:
    ocgd_gp_Quaternion() : quat() {}
    ~ocgd_gp_Quaternion() = default;

    // Initialization methods
    void init_identity() {
        quat = gp_Quaternion();
    }

    void init_components(double x, double y, double z, double w) {
        quat = gp_Quaternion(x, y, z, w);
    }

    void init_from_vectors(Vector3 vec_from, Vector3 vec_to) {
        gp_Vec v1(vec_from.x, vec_from.y, vec_from.z);
        gp_Vec v2(vec_to.x, vec_to.y, vec_to.z);
        quat = gp_Quaternion(v1, v2);
    }

    void init_from_vectors_with_help(Vector3 vec_from, Vector3 vec_to, Vector3 help_cross_vec) {
        gp_Vec v1(vec_from.x, vec_from.y, vec_from.z);
        gp_Vec v2(vec_to.x, vec_to.y, vec_to.z);
        gp_Vec help(help_cross_vec.x, help_cross_vec.y, help_cross_vec.z);
        quat = gp_Quaternion(v1, v2, help);
    }

    void init_from_axis_angle(Vector3 axis, double angle) {
        gp_Vec v(axis.x, axis.y, axis.z);
        quat = gp_Quaternion(v, angle);
    }

    void init_from_matrix(Ref<RefCounted> matrix) {
        // Note: This would need ocgd_gp_Mat binding
        // For now, create identity
        quat = gp_Quaternion();
    }

    // Setters
    void set_components(double x, double y, double z, double w) {
        quat.Set(x, y, z, w);
    }

    void set_rotation_vectors(Vector3 vec_from, Vector3 vec_to) {
        gp_Vec v1(vec_from.x, vec_from.y, vec_from.z);
        gp_Vec v2(vec_to.x, vec_to.y, vec_to.z);
        quat.SetRotation(v1, v2);
    }

    void set_rotation_vectors_with_help(Vector3 vec_from, Vector3 vec_to, Vector3 help_cross_vec) {
        gp_Vec v1(vec_from.x, vec_from.y, vec_from.z);
        gp_Vec v2(vec_to.x, vec_to.y, vec_to.z);
        gp_Vec help(help_cross_vec.x, help_cross_vec.y, help_cross_vec.z);
        quat.SetRotation(v1, v2, help);
    }

    void set_vector_and_angle(Vector3 axis, double angle) {
        gp_Vec v(axis.x, axis.y, axis.z);
        quat.SetVectorAndAngle(v, angle);
    }

    void set_matrix(Ref<RefCounted> matrix) {
        // Note: This would need ocgd_gp_Mat binding
    }

    void set_euler_angles(int order, double alpha, double beta, double gamma) {
        quat.SetEulerAngles(static_cast<gp_EulerSequence>(order), alpha, beta, gamma);
    }

    void set_identity() {
        quat.SetIdent();
    }

    // Getters
    double get_x() const {
        return quat.X();
    }

    double get_y() const {
        return quat.Y();
    }

    double get_z() const {
        return quat.Z();
    }

    double get_w() const {
        return quat.W();
    }

    Dictionary get_vector_and_angle() const {
        gp_Vec axis;
        Standard_Real angle;
        quat.GetVectorAndAngle(axis, angle);
        Dictionary result;
        result["axis"] = Vector3(axis.X(), axis.Y(), axis.Z());
        result["angle"] = angle;
        return result;
    }

    Ref<RefCounted> get_matrix() const {
        // Note: This would return ocgd_gp_Mat
        return Ref<RefCounted>();
    }

    Array get_euler_angles(int order) const {
        Standard_Real alpha, beta, gamma;
        quat.GetEulerAngles(static_cast<gp_EulerSequence>(order), alpha, beta, gamma);
        Array result;
        result.push_back(alpha);
        result.push_back(beta);
        result.push_back(gamma);
        return result;
    }

    // Queries
    bool is_equal(Ref<ocgd_gp_Quaternion> other) const {
        if (other.is_valid()) {
            return quat.IsEqual(other->get_gp_quaternion());
        }
        return false;
    }

    double square_norm() const {
        return quat.SquareNorm();
    }

    double norm() const {
        return quat.Norm();
    }

    // Operations
    void reverse() {
        quat.Reverse();
    }

    Ref<ocgd_gp_Quaternion> reversed() const {
        Ref<ocgd_gp_Quaternion> result;
        result.instantiate();
        result->set_gp_quaternion(quat.Reversed());
        return result;
    }

    void invert() {
        quat.Invert();
    }

    Ref<ocgd_gp_Quaternion> inverted() const {
        Ref<ocgd_gp_Quaternion> result;
        result.instantiate();
        result->set_gp_quaternion(quat.Inverted());
        return result;
    }

    void scale(double factor) {
        quat.Scale(factor);
    }

    Ref<ocgd_gp_Quaternion> scaled(double factor) const {
        Ref<ocgd_gp_Quaternion> result;
        result.instantiate();
        result->set_gp_quaternion(quat.Scaled(factor));
        return result;
    }

    void stabilize_length() {
        quat.StabilizeLength();
    }

    void normalize() {
        quat.Normalize();
    }

    Ref<ocgd_gp_Quaternion> normalized() const {
        Ref<ocgd_gp_Quaternion> result;
        result.instantiate();
        result->set_gp_quaternion(quat.Normalized());
        return result;
    }

    Ref<ocgd_gp_Quaternion> negated() const {
        Ref<ocgd_gp_Quaternion> result;
        result.instantiate();
        result->set_gp_quaternion(quat.Negated());
        return result;
    }

    Ref<ocgd_gp_Quaternion> added(Ref<ocgd_gp_Quaternion> other) const {
        Ref<ocgd_gp_Quaternion> result;
        result.instantiate();
        if (other.is_valid()) {
            result->set_gp_quaternion(quat.Added(other->get_gp_quaternion()));
        }
        return result;
    }

    Ref<ocgd_gp_Quaternion> subtracted(Ref<ocgd_gp_Quaternion> other) const {
        Ref<ocgd_gp_Quaternion> result;
        result.instantiate();
        if (other.is_valid()) {
            result->set_gp_quaternion(quat.Subtracted(other->get_gp_quaternion()));
        }
        return result;
    }

    Ref<ocgd_gp_Quaternion> multiplied(Ref<ocgd_gp_Quaternion> other) const {
        Ref<ocgd_gp_Quaternion> result;
        result.instantiate();
        if (other.is_valid()) {
            result->set_gp_quaternion(quat.Multiplied(other->get_gp_quaternion()));
        }
        return result;
    }

    void multiply(Ref<ocgd_gp_Quaternion> other) {
        if (other.is_valid()) {
            quat.Multiply(other->get_gp_quaternion());
        }
    }

    // Interpolation
    Ref<ocgd_gp_Quaternion> slerp(Ref<ocgd_gp_Quaternion> other, double t) const {
        Ref<ocgd_gp_Quaternion> result;
        result.instantiate();
        if (other.is_valid()) {
            // Simple implementation - for proper SLERP, would need more complex math
            gp_Quaternion interp = quat.Added(other->get_gp_quaternion().Subtracted(quat).Scaled(t));
            interp.Normalize();
            result->set_gp_quaternion(interp);
        }
        return result;
    }

    // Utility
    Ref<ocgd_gp_Quaternion> copy() const {
        Ref<ocgd_gp_Quaternion> result;
        result.instantiate();
        result->set_gp_quaternion(quat);
        return result;
    }

    Array to_array() const {
        Array result;
        result.push_back(quat.W());  // w first (common convention)
        result.push_back(quat.X());
        result.push_back(quat.Y());
        result.push_back(quat.Z());
        return result;
    }

    void from_array(Array array) {
        if (array.size() >= 4) {
            quat.Set(
                array[1].operator double(),  // x
                array[2].operator double(),  // y
                array[3].operator double(),  // z
                array[0].operator double()   // w
            );
        }
    }

    // Internal access
    const gp_Quaternion& get_gp_quaternion() const { return quat; }
    void set_gp_quaternion(const gp_Quaternion& q) { quat = q; }
};

#endif // OCGD_GP_QUATERNION_H