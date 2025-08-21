#ifndef OCGD_GP_MAT2D_H
#define OCGD_GP_MAT2D_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gp_Mat2d.hxx>
#include <gp_XY.hxx>

using namespace godot;

/**
 * @brief Wrapper for OpenCASCADE gp_Mat2d (2x2 matrix)
 * 
 * Describes a two column, two row matrix.
 * This sort of object is used in various vectorial or matrix computations.
 */
class ocgd_gp_Mat2d : public RefCounted {
    GDCLASS(ocgd_gp_Mat2d, RefCounted)

private:
    gp_Mat2d mat;

protected:
    static void _bind_methods() {
        // Constructors
        ClassDB::bind_method(D_METHOD("init_default"), &ocgd_gp_Mat2d::init_default);
        ClassDB::bind_method(D_METHOD("init_from_columns", "col1", "col2"), &ocgd_gp_Mat2d::init_from_columns);

        // Setters
        ClassDB::bind_method(D_METHOD("set_col", "col", "value"), &ocgd_gp_Mat2d::set_col);
        ClassDB::bind_method(D_METHOD("set_cols", "col1", "col2"), &ocgd_gp_Mat2d::set_cols);
        ClassDB::bind_method(D_METHOD("set_diagonal", "x1", "x2"), &ocgd_gp_Mat2d::set_diagonal);
        ClassDB::bind_method(D_METHOD("set_identity"), &ocgd_gp_Mat2d::set_identity);
        ClassDB::bind_method(D_METHOD("set_rotation", "angle"), &ocgd_gp_Mat2d::set_rotation);
        ClassDB::bind_method(D_METHOD("set_row", "row", "value"), &ocgd_gp_Mat2d::set_row);
        ClassDB::bind_method(D_METHOD("set_rows", "row1", "row2"), &ocgd_gp_Mat2d::set_rows);
        ClassDB::bind_method(D_METHOD("set_scale", "s"), &ocgd_gp_Mat2d::set_scale);
        ClassDB::bind_method(D_METHOD("set_value", "row", "col", "value"), &ocgd_gp_Mat2d::set_value);

        // Getters
        ClassDB::bind_method(D_METHOD("get_column", "col"), &ocgd_gp_Mat2d::get_column);
        ClassDB::bind_method(D_METHOD("get_determinant"), &ocgd_gp_Mat2d::get_determinant);
        ClassDB::bind_method(D_METHOD("get_diagonal"), &ocgd_gp_Mat2d::get_diagonal);
        ClassDB::bind_method(D_METHOD("get_row", "row"), &ocgd_gp_Mat2d::get_row);
        ClassDB::bind_method(D_METHOD("get_value", "row", "col"), &ocgd_gp_Mat2d::get_value);

        // Queries
        ClassDB::bind_method(D_METHOD("is_singular"), &ocgd_gp_Mat2d::is_singular);

        // Operations
        ClassDB::bind_method(D_METHOD("add", "other"), &ocgd_gp_Mat2d::add);
        ClassDB::bind_method(D_METHOD("added", "other"), &ocgd_gp_Mat2d::added);
        ClassDB::bind_method(D_METHOD("divide", "scalar"), &ocgd_gp_Mat2d::divide);
        ClassDB::bind_method(D_METHOD("divided", "scalar"), &ocgd_gp_Mat2d::divided);
        ClassDB::bind_method(D_METHOD("invert"), &ocgd_gp_Mat2d::invert);
        ClassDB::bind_method(D_METHOD("inverted"), &ocgd_gp_Mat2d::inverted);
        ClassDB::bind_method(D_METHOD("multiply_by_mat", "other"), &ocgd_gp_Mat2d::multiply_by_mat);
        ClassDB::bind_method(D_METHOD("multiplied_by_mat", "other"), &ocgd_gp_Mat2d::multiplied_by_mat);
        ClassDB::bind_method(D_METHOD("multiply_by_scalar", "scalar"), &ocgd_gp_Mat2d::multiply_by_scalar);
        ClassDB::bind_method(D_METHOD("multiplied_by_scalar", "scalar"), &ocgd_gp_Mat2d::multiplied_by_scalar);
        ClassDB::bind_method(D_METHOD("power", "n"), &ocgd_gp_Mat2d::power);
        ClassDB::bind_method(D_METHOD("powered", "n"), &ocgd_gp_Mat2d::powered);
        ClassDB::bind_method(D_METHOD("subtract", "other"), &ocgd_gp_Mat2d::subtract);
        ClassDB::bind_method(D_METHOD("subtracted", "other"), &ocgd_gp_Mat2d::subtracted);
        ClassDB::bind_method(D_METHOD("transpose"), &ocgd_gp_Mat2d::transpose);
        ClassDB::bind_method(D_METHOD("transposed"), &ocgd_gp_Mat2d::transposed);

        // Utility
        ClassDB::bind_method(D_METHOD("copy"), &ocgd_gp_Mat2d::copy);
    }

public:
    ocgd_gp_Mat2d() : mat() {}
    ~ocgd_gp_Mat2d() = default;

    // Initialization methods
    void init_default() {
        mat = gp_Mat2d();
    }

    void init_from_columns(Vector2 col1, Vector2 col2) {
        gp_XY xy1(col1.x, col1.y);
        gp_XY xy2(col2.x, col2.y);
        mat = gp_Mat2d(xy1, xy2);
    }

    // Setters
    void set_col(int col, Vector2 value) {
        gp_XY xy(value.x, value.y);
        mat.SetCol(col, xy);
    }

    void set_cols(Vector2 col1, Vector2 col2) {
        gp_XY xy1(col1.x, col1.y);
        gp_XY xy2(col2.x, col2.y);
        mat.SetCols(xy1, xy2);
    }

    void set_diagonal(double x1, double x2) {
        mat.SetDiagonal(x1, x2);
    }

    void set_identity() {
        mat.SetIdentity();
    }

    void set_rotation(double angle) {
        mat.SetRotation(angle);
    }

    void set_row(int row, Vector2 value) {
        gp_XY xy(value.x, value.y);
        mat.SetRow(row, xy);
    }

    void set_rows(Vector2 row1, Vector2 row2) {
        gp_XY xy1(row1.x, row1.y);
        gp_XY xy2(row2.x, row2.y);
        mat.SetRows(xy1, xy2);
    }

    void set_scale(double s) {
        mat.SetScale(s);
    }

    void set_value(int row, int col, double value) {
        mat.SetValue(row, col, value);
    }

    // Getters
    Vector2 get_column(int col) const {
        gp_XY xy = mat.Column(col);
        return Vector2(xy.X(), xy.Y());
    }

    double get_determinant() const {
        return mat.Determinant();
    }

    Vector2 get_diagonal() const {
        gp_XY xy = mat.Diagonal();
        return Vector2(xy.X(), xy.Y());
    }

    Vector2 get_row(int row) const {
        gp_XY xy = mat.Row(row);
        return Vector2(xy.X(), xy.Y());
    }

    double get_value(int row, int col) const {
        return mat.Value(row, col);
    }

    // Queries
    bool is_singular() const {
        return mat.IsSingular();
    }

    // Operations
    void add(Ref<ocgd_gp_Mat2d> other) {
        if (other.is_valid()) {
            mat.Add(other->get_gp_mat());
        }
    }

    Ref<ocgd_gp_Mat2d> added(Ref<ocgd_gp_Mat2d> other) const {
        Ref<ocgd_gp_Mat2d> result;
        result.instantiate();
        if (other.is_valid()) {
            result->set_gp_mat(mat.Added(other->get_gp_mat()));
        }
        return result;
    }

    void divide(double scalar) {
        mat.Divide(scalar);
    }

    Ref<ocgd_gp_Mat2d> divided(double scalar) const {
        Ref<ocgd_gp_Mat2d> result;
        result.instantiate();
        result->set_gp_mat(mat.Divided(scalar));
        return result;
    }

    void invert() {
        mat.Invert();
    }

    Ref<ocgd_gp_Mat2d> inverted() const {
        Ref<ocgd_gp_Mat2d> result;
        result.instantiate();
        result->set_gp_mat(mat.Inverted());
        return result;
    }

    void multiply_by_mat(Ref<ocgd_gp_Mat2d> other) {
        if (other.is_valid()) {
            mat.Multiply(other->get_gp_mat());
        }
    }

    Ref<ocgd_gp_Mat2d> multiplied_by_mat(Ref<ocgd_gp_Mat2d> other) const {
        Ref<ocgd_gp_Mat2d> result;
        result.instantiate();
        if (other.is_valid()) {
            result->set_gp_mat(mat.Multiplied(other->get_gp_mat()));
        }
        return result;
    }

    void multiply_by_scalar(double scalar) {
        mat.Multiply(scalar);
    }

    Ref<ocgd_gp_Mat2d> multiplied_by_scalar(double scalar) const {
        Ref<ocgd_gp_Mat2d> result;
        result.instantiate();
        result->set_gp_mat(mat.Multiplied(scalar));
        return result;
    }

    void power(int n) {
        mat.Power(n);
    }

    Ref<ocgd_gp_Mat2d> powered(int n) const {
        Ref<ocgd_gp_Mat2d> result;
        result.instantiate();
        result->set_gp_mat(mat.Powered(n));
        return result;
    }

    void subtract(Ref<ocgd_gp_Mat2d> other) {
        if (other.is_valid()) {
            mat.Subtract(other->get_gp_mat());
        }
    }

    Ref<ocgd_gp_Mat2d> subtracted(Ref<ocgd_gp_Mat2d> other) const {
        Ref<ocgd_gp_Mat2d> result;
        result.instantiate();
        if (other.is_valid()) {
            result->set_gp_mat(mat.Subtracted(other->get_gp_mat()));
        }
        return result;
    }

    void transpose() {
        mat.Transpose();
    }

    Ref<ocgd_gp_Mat2d> transposed() const {
        Ref<ocgd_gp_Mat2d> result;
        result.instantiate();
        result->set_gp_mat(mat.Transposed());
        return result;
    }

    // Utility
    Ref<ocgd_gp_Mat2d> copy() const {
        Ref<ocgd_gp_Mat2d> result;
        result.instantiate();
        result->set_gp_mat(mat);
        return result;
    }

    // Internal access
    const gp_Mat2d& get_gp_mat() const { return mat; }
    void set_gp_mat(const gp_Mat2d& m) { mat = m; }
};

#endif // OCGD_GP_MAT2D_H