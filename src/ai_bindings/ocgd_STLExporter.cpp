/**
 * ocgd_STLExporter.cpp
 *
 * Godot GDExtension wrapper implementation for OpenCASCADE STL export functionality.
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "ocgd_STLExporter.hxx"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <opencascade/StlAPI_Writer.hxx>
#include <opencascade/TopoDS_Shape.hxx>
#include <opencascade/TopoDS.hxx>
#include <opencascade/TopExp_Explorer.hxx>
#include <opencascade/BRep_Tool.hxx>
#include <opencascade/Poly_Triangulation.hxx>
#include <opencascade/Message_ProgressRange.hxx>
#include <opencascade/BRepBndLib.hxx>
#include <opencascade/Bnd_Box.hxx>
#include <opencascade/BRepCheck_Analyzer.hxx>
#include <opencascade/TopLoc_Location.hxx>

#include <fstream>

using namespace godot;

ocgd_STLExporter::ocgd_STLExporter() {
    _writer = new StlAPI_Writer();
    _owns_writer = true;
}

ocgd_STLExporter::~ocgd_STLExporter() {
    if (_owns_writer && _writer != nullptr) {
        delete _writer;
        _writer = nullptr;
    }
}

void ocgd_STLExporter::set_ascii_mode(bool ascii_mode) {
    if (_writer != nullptr) {
        _writer->ASCIIMode() = ascii_mode ? Standard_True : Standard_False;
    }
}

bool ocgd_STLExporter::get_ascii_mode() const {
    if (_writer != nullptr) {
        return _writer->ASCIIMode() == Standard_True;
    }
    return true; // Default to ASCII mode
}

bool ocgd_STLExporter::write_file(const Ref<ocgd_TopoDS_Shape>& shape, const String& filename) {
    if (shape.is_null() || shape->is_null()) {
        UtilityFunctions::printerr("STLExporter: Cannot export null shape");
        return false;
    }

    if (_writer == nullptr) {
        UtilityFunctions::printerr("STLExporter: Writer not initialized");
        return false;
    }

    std::string std_filename = filename.utf8().get_data();
    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    Message_ProgressRange progress;
    Standard_Boolean result = _writer->Write(occt_shape, std_filename.c_str(), progress);

    return result == Standard_True;
}

bool ocgd_STLExporter::write_file_with_progress(const Ref<ocgd_TopoDS_Shape>& shape,
                                               const String& filename,
                                               const String& progress_callback) {
    // For now, just use the regular write method
    // Progress callback implementation would require more complex integration
    return write_file(shape, filename);
}

bool ocgd_STLExporter::can_export_shape(const Ref<ocgd_TopoDS_Shape>& shape) const {
    if (shape.is_null() || shape->is_null()) {
        return false;
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    // Check if shape has any faces that can be triangulated
    TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
    if (!face_explorer.More()) {
        return false; // No faces to export
    }

    // Check if shape is valid
    BRepCheck_Analyzer analyzer(occt_shape);
    return analyzer.IsValid() == Standard_True;
}

int ocgd_STLExporter::get_estimated_file_size(const Ref<ocgd_TopoDS_Shape>& shape, bool ascii_mode) const {
    if (shape.is_null() || shape->is_null()) {
        return 0;
    }

    int triangle_count = get_triangle_count(shape);
    if (triangle_count == 0) {
        return 0;
    }

    if (ascii_mode) {
        // ASCII STL format: ~200 bytes per triangle (including normal and vertices)
        return triangle_count * 200 + 100; // 100 bytes for header
    } else {
        // Binary STL format: 50 bytes per triangle + 80 byte header + 4 byte count
        return triangle_count * 50 + 84;
    }
}

int ocgd_STLExporter::get_triangle_count(const Ref<ocgd_TopoDS_Shape>& shape) const {
    if (shape.is_null() || shape->is_null()) {
        return 0;
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();
    int total_triangles = 0;

    TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
    while (face_explorer.More()) {
        const TopoDS_Face& face = TopoDS::Face(face_explorer.Current());

        TopLoc_Location location;
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);

        if (!triangulation.IsNull()) {
            total_triangles += triangulation->NbTriangles();
        }

        face_explorer.Next();
    }

    return total_triangles;
}

String ocgd_STLExporter::validate_shape_for_export(const Ref<ocgd_TopoDS_Shape>& shape) const {
    if (shape.is_null()) {
        return "Shape reference is null";
    }

    if (shape->is_null()) {
        return "Shape is null";
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    // Check if shape has faces
    TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
    if (!face_explorer.More()) {
        return "Shape has no faces to export";
    }

    // Check if shape is valid
    BRepCheck_Analyzer analyzer(occt_shape);
    if (analyzer.IsValid() != Standard_True) {
        return "Shape has geometric errors";
    }

    // Check if any faces have triangulation
    bool has_triangulation = false;
    face_explorer.ReInit();
    while (face_explorer.More()) {
        const TopoDS_Face& face = TopoDS::Face(face_explorer.Current());

        TopLoc_Location location;
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);

        if (!triangulation.IsNull()) {
            has_triangulation = true;
            break;
        }

        face_explorer.Next();
    }

    if (!has_triangulation) {
        return "Shape has no triangulation data (run mesh generation first)";
    }

    return ""; // Valid for export
}

PackedByteArray ocgd_STLExporter::export_to_bytes(const Ref<ocgd_TopoDS_Shape>& shape, bool ascii_mode) const {
    PackedByteArray result;

    if (shape.is_null() || shape->is_null()) {
        UtilityFunctions::printerr("STLExporter: Cannot export null shape to bytes");
        return result;
    }

    // Create a temporary filename
    String temp_filename = "temp_export.stl";

    // Set ASCII mode
    bool original_mode = get_ascii_mode();
    const_cast<ocgd_STLExporter*>(this)->set_ascii_mode(ascii_mode);

    // Export to file
    bool success = const_cast<ocgd_STLExporter*>(this)->write_file(shape, temp_filename);

    if (success) {
        // Read file back into byte array
        std::ifstream file(temp_filename.utf8().get_data(), std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);

            result.resize(size);
            file.read(reinterpret_cast<char*>(result.ptrw()), size);
            file.close();

            // Clean up temp file
            std::remove(temp_filename.utf8().get_data());
        }
    }

    // Restore original mode
    const_cast<ocgd_STLExporter*>(this)->set_ascii_mode(original_mode);

    return result;
}

bool ocgd_STLExporter::quick_export(const Ref<ocgd_TopoDS_Shape>& shape, const String& filename, bool ascii_mode) {
    set_ascii_mode(ascii_mode);
    return write_file(shape, filename);
}

int ocgd_STLExporter::export_multiple_shapes(const Array& shapes, const String& base_filename, bool ascii_mode) {
    int success_count = 0;

    set_ascii_mode(ascii_mode);

    for (int i = 0; i < shapes.size(); i++) {
        Ref<ocgd_TopoDS_Shape> shape = shapes[i];
        if (shape.is_null()) {
            continue;
        }

        // Generate filename with index
        String filename = base_filename;
        int dot_pos = filename.rfind(".");
        if (dot_pos != -1) {
            String base = filename.substr(0, dot_pos);
            String ext = filename.substr(dot_pos);
            filename = base + "_" + String::num(i) + ext;
        } else {
            filename = filename + "_" + String::num(i) + ".stl";
        }

        if (write_file(shape, filename)) {
            success_count++;
        }
    }

    return success_count;
}

double ocgd_STLExporter::get_recommended_deflection(const Ref<ocgd_TopoDS_Shape>& shape, double quality_factor) const {
    if (shape.is_null() || shape->is_null()) {
        return 0.1; // Default deflection
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    // Get bounding box
    Bnd_Box bounding_box;
    BRepBndLib::Add(occt_shape, bounding_box);

    if (bounding_box.IsVoid()) {
        return 0.1;
    }

    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    bounding_box.Get(xmin, ymin, zmin, xmax, ymax, zmax);

    // Calculate diagonal length
    double dx = xmax - xmin;
    double dy = ymax - ymin;
    double dz = zmax - zmin;
    double diagonal = sqrt(dx*dx + dy*dy + dz*dz);

    // Recommend deflection as a fraction of the diagonal
    // quality_factor: 0.1 = coarse (1/100), 1.0 = fine (1/1000)
    double base_fraction = 0.01; // 1/100 for coarse
    double fine_fraction = 0.001; // 1/1000 for fine

    double fraction = base_fraction + (fine_fraction - base_fraction) * quality_factor;

    return diagonal * fraction;
}

void ocgd_STLExporter::_bind_methods() {
    // ASCII mode property
    ClassDB::bind_method(D_METHOD("set_ascii_mode", "ascii_mode"), &ocgd_STLExporter::set_ascii_mode);
    ClassDB::bind_method(D_METHOD("get_ascii_mode"), &ocgd_STLExporter::get_ascii_mode);
    ClassDB::add_property("ocgd_STLExporter", PropertyInfo(Variant::BOOL, "ascii_mode"), "set_ascii_mode", "get_ascii_mode");

    // Export methods
    ClassDB::bind_method(D_METHOD("write_file", "shape", "filename"), &ocgd_STLExporter::write_file);
    ClassDB::bind_method(D_METHOD("write_file_with_progress", "shape", "filename", "progress_callback"), &ocgd_STLExporter::write_file_with_progress, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("quick_export", "shape", "filename", "ascii_mode"), &ocgd_STLExporter::quick_export, DEFVAL(true));
    ClassDB::bind_method(D_METHOD("export_to_bytes", "shape", "ascii_mode"), &ocgd_STLExporter::export_to_bytes);
    ClassDB::bind_method(D_METHOD("export_multiple_shapes", "shapes", "base_filename", "ascii_mode"), &ocgd_STLExporter::export_multiple_shapes, DEFVAL(true));

    // Validation and analysis methods
    ClassDB::bind_method(D_METHOD("can_export_shape", "shape"), &ocgd_STLExporter::can_export_shape);
    ClassDB::bind_method(D_METHOD("validate_shape_for_export", "shape"), &ocgd_STLExporter::validate_shape_for_export);
    ClassDB::bind_method(D_METHOD("get_triangle_count", "shape"), &ocgd_STLExporter::get_triangle_count);
    ClassDB::bind_method(D_METHOD("get_estimated_file_size", "shape", "ascii_mode"), &ocgd_STLExporter::get_estimated_file_size);
    ClassDB::bind_method(D_METHOD("get_recommended_deflection", "shape", "quality_factor"), &ocgd_STLExporter::get_recommended_deflection, DEFVAL(0.5));
}
