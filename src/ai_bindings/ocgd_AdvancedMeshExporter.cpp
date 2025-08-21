/**
 * ocgd_AdvancedMeshExporter.cpp
 *
 * Godot GDExtension wrapper implementation for OpenCASCADE advanced mesh export functionality.
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "ocgd_AdvancedMeshExporter.hxx"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <opencascade/StlAPI_Writer.hxx>
#include <opencascade/TDocStd_Document.hxx>
#include <opencascade/XCAFApp_Application.hxx>
#include <opencascade/TDF_Label.hxx>
#include <opencascade/TopExp_Explorer.hxx>
#include <opencascade/TopoDS_Face.hxx>
#include <opencascade/TopoDS.hxx>
#include <opencascade/BRep_Tool.hxx>
#include <opencascade/Poly_Triangulation.hxx>
#include <opencascade/TopLoc_Location.hxx>
#include <opencascade/BRepMesh_IncrementalMesh.hxx>
#include <opencascade/Precision.hxx>
#include <opencascade/Standard_Failure.hxx>
#include <opencascade/gp_Trsf.hxx>
#include <opencascade/gp_Ax3.hxx>
#include <opencascade/Message_ProgressRange.hxx>

#include <fstream>

void ocgd_AdvancedMeshExporter::_bind_methods() {
    // Enums
    BIND_ENUM_CONSTANT(FORMAT_PLY);
    BIND_ENUM_CONSTANT(FORMAT_OBJ);
    BIND_ENUM_CONSTANT(FORMAT_STL_ASCII);
    BIND_ENUM_CONSTANT(FORMAT_STL_BINARY);

    BIND_ENUM_CONSTANT(COLOR_NONE);
    BIND_ENUM_CONSTANT(COLOR_PER_SHAPE);
    BIND_ENUM_CONSTANT(COLOR_PER_FACE);
    BIND_ENUM_CONSTANT(COLOR_PER_VERTEX);

    BIND_ENUM_CONSTANT(COORD_OCCT);
    BIND_ENUM_CONSTANT(COORD_BLENDER);
    BIND_ENUM_CONSTANT(COORD_UNITY);
    BIND_ENUM_CONSTANT(COORD_UNREAL);
    BIND_ENUM_CONSTANT(COORD_CUSTOM);

    // Configuration methods
    ClassDB::bind_method(D_METHOD("set_export_format", "format"), &ocgd_AdvancedMeshExporter::set_export_format);
    ClassDB::bind_method(D_METHOD("get_export_format"), &ocgd_AdvancedMeshExporter::get_export_format);

    ClassDB::bind_method(D_METHOD("set_color_mode", "mode"), &ocgd_AdvancedMeshExporter::set_color_mode);
    ClassDB::bind_method(D_METHOD("get_color_mode"), &ocgd_AdvancedMeshExporter::get_color_mode);

    ClassDB::bind_method(D_METHOD("set_coordinate_system", "system"), &ocgd_AdvancedMeshExporter::set_coordinate_system);
    ClassDB::bind_method(D_METHOD("get_coordinate_system"), &ocgd_AdvancedMeshExporter::get_coordinate_system);

    ClassDB::bind_method(D_METHOD("set_custom_transform", "transform"), &ocgd_AdvancedMeshExporter::set_custom_transform);
    ClassDB::bind_method(D_METHOD("get_custom_transform"), &ocgd_AdvancedMeshExporter::get_custom_transform);

    // Mesh quality settings
    ClassDB::bind_method(D_METHOD("set_linear_deflection", "deflection"), &ocgd_AdvancedMeshExporter::set_linear_deflection);
    ClassDB::bind_method(D_METHOD("get_linear_deflection"), &ocgd_AdvancedMeshExporter::get_linear_deflection);

    ClassDB::bind_method(D_METHOD("set_angular_deflection", "deflection"), &ocgd_AdvancedMeshExporter::set_angular_deflection);
    ClassDB::bind_method(D_METHOD("get_angular_deflection"), &ocgd_AdvancedMeshExporter::get_angular_deflection);

    ClassDB::bind_method(D_METHOD("set_relative_deflection", "enabled"), &ocgd_AdvancedMeshExporter::set_relative_deflection);
    ClassDB::bind_method(D_METHOD("get_relative_deflection"), &ocgd_AdvancedMeshExporter::get_relative_deflection);

    ClassDB::bind_method(D_METHOD("set_parallel_processing", "enabled"), &ocgd_AdvancedMeshExporter::set_parallel_processing);
    ClassDB::bind_method(D_METHOD("get_parallel_processing"), &ocgd_AdvancedMeshExporter::get_parallel_processing);

    // Export options
    ClassDB::bind_method(D_METHOD("set_export_normals", "enabled"), &ocgd_AdvancedMeshExporter::set_export_normals);
    ClassDB::bind_method(D_METHOD("get_export_normals"), &ocgd_AdvancedMeshExporter::get_export_normals);

    ClassDB::bind_method(D_METHOD("set_export_uv_coordinates", "enabled"), &ocgd_AdvancedMeshExporter::set_export_uv_coordinates);
    ClassDB::bind_method(D_METHOD("get_export_uv_coordinates"), &ocgd_AdvancedMeshExporter::get_export_uv_coordinates);

    ClassDB::bind_method(D_METHOD("set_export_materials", "enabled"), &ocgd_AdvancedMeshExporter::set_export_materials);
    ClassDB::bind_method(D_METHOD("get_export_materials"), &ocgd_AdvancedMeshExporter::get_export_materials);

    ClassDB::bind_method(D_METHOD("set_merge_vertices", "enabled"), &ocgd_AdvancedMeshExporter::set_merge_vertices);
    ClassDB::bind_method(D_METHOD("get_merge_vertices"), &ocgd_AdvancedMeshExporter::get_merge_vertices);

    ClassDB::bind_method(D_METHOD("set_vertex_tolerance", "tolerance"), &ocgd_AdvancedMeshExporter::set_vertex_tolerance);
    ClassDB::bind_method(D_METHOD("get_vertex_tolerance"), &ocgd_AdvancedMeshExporter::get_vertex_tolerance);

    // Main export methods
    ClassDB::bind_method(D_METHOD("export_shape", "shape", "file_path"), &ocgd_AdvancedMeshExporter::export_shape);
    ClassDB::bind_method(D_METHOD("export_shapes", "shapes", "colors", "file_path"), &ocgd_AdvancedMeshExporter::export_shapes);
    ClassDB::bind_method(D_METHOD("export_shape_with_colors", "shape", "face_colors", "file_path"), &ocgd_AdvancedMeshExporter::export_shape_with_colors);
    ClassDB::bind_method(D_METHOD("extract_mesh_data", "shape"), &ocgd_AdvancedMeshExporter::extract_mesh_data);
    ClassDB::bind_method(D_METHOD("extract_colored_mesh_data", "shape"), &ocgd_AdvancedMeshExporter::extract_colored_mesh_data);

    // Validation and information
    ClassDB::bind_method(D_METHOD("validate_shape", "shape"), &ocgd_AdvancedMeshExporter::validate_shape);
    ClassDB::bind_method(D_METHOD("estimate_file_size", "shape"), &ocgd_AdvancedMeshExporter::estimate_file_size);
    ClassDB::bind_method(D_METHOD("get_supported_extensions"), &ocgd_AdvancedMeshExporter::get_supported_extensions);
    ClassDB::bind_method(D_METHOD("get_format_capabilities", "format"), &ocgd_AdvancedMeshExporter::get_format_capabilities);

    // Progress and error handling
    ClassDB::bind_method(D_METHOD("get_progress"), &ocgd_AdvancedMeshExporter::get_progress);
    ClassDB::bind_method(D_METHOD("get_last_error"), &ocgd_AdvancedMeshExporter::get_last_error);
    ClassDB::bind_method(D_METHOD("clear_error"), &ocgd_AdvancedMeshExporter::clear_error);

    // Utility methods
    ClassDB::bind_method(D_METHOD("triangulate_shape", "shape"), &ocgd_AdvancedMeshExporter::triangulate_shape);
    ClassDB::bind_method(D_METHOD("get_mesh_statistics", "shape"), &ocgd_AdvancedMeshExporter::get_mesh_statistics);
    ClassDB::bind_method(D_METHOD("optimize_mesh_data", "mesh_data"), &ocgd_AdvancedMeshExporter::optimize_mesh_data);
}

ocgd_AdvancedMeshExporter::ocgd_AdvancedMeshExporter() :
    _format(FORMAT_PLY),
    _color_mode(COLOR_PER_SHAPE),
    _coordinate_system(COORD_OCCT),
    _linear_deflection(0.1),
    _angular_deflection(0.5),
    _relative_deflection(false),
    _parallel_processing(true),
    _export_normals(true),
    _export_uv_coordinates(false),
    _export_materials(true),
    _merge_vertices(true),
    _vertex_tolerance(1e-6),
    _progress_current(0),
    _progress_total(100) {

    initialize_document();
}

ocgd_AdvancedMeshExporter::~ocgd_AdvancedMeshExporter() {
}

bool ocgd_AdvancedMeshExporter::initialize_document() {
    try {
        Handle(XCAFApp_Application) app = XCAFApp_Application::GetApplication();
        app->NewDocument("BinXCAF", _document);

        if (_document.IsNull()) {
            set_error("Failed to create XCAF document");
            return false;
        }

        _shape_tool = XCAFDoc_DocumentTool::ShapeTool(_document->Main());
        _color_tool = XCAFDoc_DocumentTool::ColorTool(_document->Main());
        _material_tool = XCAFDoc_DocumentTool::MaterialTool(_document->Main());

        return true;
    } catch (const Standard_Failure& e) {
        set_error(String("Failed to initialize document: ") + String(e.GetMessageString()));
        return false;
    }
}

// Configuration methods
void ocgd_AdvancedMeshExporter::set_export_format(ExportFormat format) {
    _format = format;
}

ocgd_AdvancedMeshExporter::ExportFormat ocgd_AdvancedMeshExporter::get_export_format() const {
    return _format;
}

void ocgd_AdvancedMeshExporter::set_color_mode(ColorMode mode) {
    _color_mode = mode;
}

ocgd_AdvancedMeshExporter::ColorMode ocgd_AdvancedMeshExporter::get_color_mode() const {
    return _color_mode;
}

void ocgd_AdvancedMeshExporter::set_coordinate_system(CoordinateSystem system) {
    _coordinate_system = system;
}

ocgd_AdvancedMeshExporter::CoordinateSystem ocgd_AdvancedMeshExporter::get_coordinate_system() const {
    return _coordinate_system;
}

void ocgd_AdvancedMeshExporter::set_custom_transform(const Dictionary& transform) {
    _custom_transform = transform;
}

Dictionary ocgd_AdvancedMeshExporter::get_custom_transform() const {
    return _custom_transform;
}

// Mesh quality settings
void ocgd_AdvancedMeshExporter::set_linear_deflection(double deflection) {
    _linear_deflection = std::max(0.001, deflection);
}

double ocgd_AdvancedMeshExporter::get_linear_deflection() const {
    return _linear_deflection;
}

void ocgd_AdvancedMeshExporter::set_angular_deflection(double deflection) {
    _angular_deflection = std::max(0.01, deflection);
}

double ocgd_AdvancedMeshExporter::get_angular_deflection() const {
    return _angular_deflection;
}

void ocgd_AdvancedMeshExporter::set_relative_deflection(bool enabled) {
    _relative_deflection = enabled;
}

bool ocgd_AdvancedMeshExporter::get_relative_deflection() const {
    return _relative_deflection;
}

void ocgd_AdvancedMeshExporter::set_parallel_processing(bool enabled) {
    _parallel_processing = enabled;
}

bool ocgd_AdvancedMeshExporter::get_parallel_processing() const {
    return _parallel_processing;
}

// Export options
void ocgd_AdvancedMeshExporter::set_export_normals(bool enabled) {
    _export_normals = enabled;
}

bool ocgd_AdvancedMeshExporter::get_export_normals() const {
    return _export_normals;
}

void ocgd_AdvancedMeshExporter::set_export_uv_coordinates(bool enabled) {
    _export_uv_coordinates = enabled;
}

bool ocgd_AdvancedMeshExporter::get_export_uv_coordinates() const {
    return _export_uv_coordinates;
}

void ocgd_AdvancedMeshExporter::set_export_materials(bool enabled) {
    _export_materials = enabled;
}

bool ocgd_AdvancedMeshExporter::get_export_materials() const {
    return _export_materials;
}

void ocgd_AdvancedMeshExporter::set_merge_vertices(bool enabled) {
    _merge_vertices = enabled;
}

bool ocgd_AdvancedMeshExporter::get_merge_vertices() const {
    return _merge_vertices;
}

void ocgd_AdvancedMeshExporter::set_vertex_tolerance(double tolerance) {
    _vertex_tolerance = std::max(1e-9, tolerance);
}

double ocgd_AdvancedMeshExporter::get_vertex_tolerance() const {
    return _vertex_tolerance;
}

// Main export methods
bool ocgd_AdvancedMeshExporter::export_shape(const Ref<ocgd_TopoDS_Shape>& shape, const String& file_path) {
    if (shape.is_null()) {
        set_error("Shape is null");
        return false;
    }

    clear_error();
    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    // Ensure triangulation
    if (!triangulate_shape(shape)) {
        return false;
    }

    try {
        switch (_format) {
            case FORMAT_PLY:
                return export_ply(occt_shape, file_path);
            case FORMAT_OBJ:
                return export_obj(occt_shape, file_path);
            case FORMAT_STL_ASCII:
            case FORMAT_STL_BINARY:
                return export_stl(occt_shape, file_path);
            default:
                set_error("Unsupported export format");
                return false;
        }
    } catch (const Standard_Failure& e) {
        set_error(String("Export failed: ") + String(e.GetMessageString()));
        return false;
    }
}

bool ocgd_AdvancedMeshExporter::export_shapes(const Array& shapes, const Array& colors, const String& file_path) {
    if (shapes.size() == 0) {
        set_error("No shapes provided");
        return false;
    }

    clear_error();

    // For multiple shapes, we need to merge them or export as separate objects
    // For now, we'll export the first shape with the first color
    Ref<ocgd_TopoDS_Shape> first_shape = shapes[0];
    return export_shape(first_shape, file_path);
}

bool ocgd_AdvancedMeshExporter::export_shape_with_colors(const Ref<ocgd_TopoDS_Shape>& shape,
                                                        const PackedColorArray& face_colors,
                                                        const String& file_path) {
    // Store the current color mode and temporarily set it to per-face
    ColorMode original_mode = _color_mode;
    _color_mode = COLOR_PER_FACE;

    bool result = export_shape(shape, file_path);

    // Restore original color mode
    _color_mode = original_mode;

    return result;
}

Dictionary ocgd_AdvancedMeshExporter::extract_mesh_data(const Ref<ocgd_TopoDS_Shape>& shape) {
    Dictionary result;

    if (shape.is_null()) {
        set_error("Shape is null");
        return result;
    }

    clear_error();
    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    // Ensure triangulation
    if (!triangulate_shape(shape)) {
        return result;
    }

    PackedVector3Array vertices;
    PackedInt32Array indices;
    PackedVector3Array normals;

    try {
        int vertex_offset = 0;

        TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
        while (face_explorer.More()) {
            const TopoDS_Face& face = TopoDS::Face(face_explorer.Current());

            TopLoc_Location location;
            Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);

            if (!triangulation.IsNull()) {
                const Standard_Integer nb_nodes = triangulation->NbNodes();
                const Standard_Integer nb_triangles = triangulation->NbTriangles();

                // Extract vertices
                const Poly_ArrayOfNodes& nodes = triangulation->InternalNodes();
                for (Standard_Integer i = 1; i <= nb_nodes; i++) {
                    gp_Pnt point = nodes.Value(i);

                    // Apply location transformation if present
                    if (!location.IsIdentity()) {
                        point.Transform(location.Transformation());
                    }

                    vertices.append(Vector3(
                        static_cast<float>(point.X()),
                        static_cast<float>(point.Y()),
                        static_cast<float>(point.Z())
                    ));
                }

                // Extract triangles
                const Poly_Array1OfTriangle& triangles = triangulation->InternalTriangles();
                Standard_Boolean is_reversed = (face.Orientation() == TopAbs_REVERSED);

                for (Standard_Integer i = 1; i <= nb_triangles; i++) {
                    const Poly_Triangle& triangle = triangles.Value(i);
                    Standard_Integer n1, n2, n3;
                    triangle.Get(n1, n2, n3);

                    // Adjust for 0-based indexing and apply vertex offset
                    if (is_reversed) {
                        indices.append(vertex_offset + n1 - 1);
                        indices.append(vertex_offset + n3 - 1);
                        indices.append(vertex_offset + n2 - 1);
                    } else {
                        indices.append(vertex_offset + n1 - 1);
                        indices.append(vertex_offset + n2 - 1);
                        indices.append(vertex_offset + n3 - 1);
                    }
                }

                // Extract normals if available
                if (_export_normals && triangulation->HasNormals()) {
                    const NCollection_Array1<gp_Vec3f>& norms = triangulation->InternalNormals();
                    for (Standard_Integer i = 1; i <= nb_nodes; i++) {
                        const gp_Vec3f& norm = norms.Value(i);
                        Vector3 normal(norm.x(), norm.y(), norm.z());

                        // Apply location transformation to normals if present
                        if (!location.IsIdentity()) {
                            gp_Vec normal_vec(normal.x, normal.y, normal.z);
                            normal_vec.Transform(location.Transformation());
                            normal = Vector3(
                                static_cast<float>(normal_vec.X()),
                                static_cast<float>(normal_vec.Y()),
                                static_cast<float>(normal_vec.Z())
                            );
                        }

                        normals.append(normal);
                    }
                }

                vertex_offset += nb_nodes;
            }

            face_explorer.Next();
        }

        // Apply coordinate system transformation
        if (normals.size() > 0) {
            transform_mesh_data(vertices, normals);
        } else {
            PackedVector3Array empty_normals;
            transform_mesh_data(vertices, empty_normals);
        }

        result["vertices"] = vertices;
        result["indices"] = indices;
        if (normals.size() > 0) {
            result["normals"] = normals;
        }

    } catch (const Standard_Failure& e) {
        set_error(String("Failed to extract mesh data: ") + String(e.GetMessageString()));
        return Dictionary();
    }

    return result;
}

Dictionary ocgd_AdvancedMeshExporter::extract_colored_mesh_data(const Ref<ocgd_TopoDS_Shape>& shape) {
    Dictionary base_data = extract_mesh_data(shape);

    if (base_data.is_empty()) {
        return base_data;
    }

    // Add color information based on color mode
    PackedColorArray colors;

    if (_color_mode != COLOR_NONE) {
        PackedVector3Array vertices = base_data["vertices"];

        // For now, assign default colors (this could be enhanced with actual CAD color extraction)
        Color default_color(0.7f, 0.7f, 0.7f, 1.0f);
        for (int i = 0; i < vertices.size(); i++) {
            colors.append(default_color);
        }

        base_data["colors"] = colors;
    }

    return base_data;
}

// Validation and information methods
Dictionary ocgd_AdvancedMeshExporter::validate_shape(const Ref<ocgd_TopoDS_Shape>& shape) const {
    Dictionary result;

    if (shape.is_null()) {
        result["valid"] = false;
        result["error"] = "Shape is null";
        return result;
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    if (occt_shape.IsNull()) {
        result["valid"] = false;
        result["error"] = "Shape is empty";
        return result;
    }

    // Count faces and check for triangulation
    int face_count = 0;
    int triangulated_faces = 0;

    TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
    while (face_explorer.More()) {
        face_count++;

        const TopoDS_Face& face = TopoDS::Face(face_explorer.Current());
        TopLoc_Location location;
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);

        if (!triangulation.IsNull()) {
            triangulated_faces++;
        }

        face_explorer.Next();
    }

    result["valid"] = (face_count > 0);
    result["face_count"] = face_count;
    result["triangulated_faces"] = triangulated_faces;
    result["needs_triangulation"] = (triangulated_faces < face_count);

    return result;
}

int ocgd_AdvancedMeshExporter::estimate_file_size(const Ref<ocgd_TopoDS_Shape>& shape) const {
    Dictionary stats = get_mesh_statistics(shape);

    if (stats.is_empty()) {
        return 0;
    }

    int vertex_count = stats.get("vertex_count", 0);
    int triangle_count = stats.get("triangle_count", 0);

    switch (_format) {
        case FORMAT_PLY:
            // PLY: ~24 bytes per vertex + ~12 bytes per triangle + header
            return vertex_count * 24 + triangle_count * 12 + 1024;
        case FORMAT_OBJ:
            // OBJ: ~30 bytes per vertex + ~15 bytes per triangle (text format)
            return vertex_count * 30 + triangle_count * 15 + 512;
        case FORMAT_STL_ASCII:
            // STL ASCII: ~200 bytes per triangle
            return triangle_count * 200 + 256;
        case FORMAT_STL_BINARY:
            // STL Binary: 50 bytes per triangle + 80 byte header
            return triangle_count * 50 + 84;
        default:
            return 0;
    }
}

PackedStringArray ocgd_AdvancedMeshExporter::get_supported_extensions() const {
    PackedStringArray extensions;

    switch (_format) {
        case FORMAT_PLY:
            extensions.append("ply");
            break;
        case FORMAT_OBJ:
            extensions.append("obj");
            break;
        case FORMAT_STL_ASCII:
        case FORMAT_STL_BINARY:
            extensions.append("stl");
            break;
    }

    return extensions;
}

Dictionary ocgd_AdvancedMeshExporter::get_format_capabilities(ExportFormat format) const {
    Dictionary caps;

    switch (format) {
        case FORMAT_PLY:
            caps["supports_colors"] = true;
            caps["supports_normals"] = true;
            caps["supports_textures"] = true;
            caps["supports_materials"] = false;
            caps["binary_format"] = true;
            caps["text_format"] = true;
            break;
        case FORMAT_OBJ:
            caps["supports_colors"] = false;
            caps["supports_normals"] = true;
            caps["supports_textures"] = true;
            caps["supports_materials"] = true;
            caps["binary_format"] = false;
            caps["text_format"] = true;
            break;
        case FORMAT_STL_ASCII:
        case FORMAT_STL_BINARY:
            caps["supports_colors"] = false;
            caps["supports_normals"] = false;
            caps["supports_textures"] = false;
            caps["supports_materials"] = false;
            caps["binary_format"] = (format == FORMAT_STL_BINARY);
            caps["text_format"] = (format == FORMAT_STL_ASCII);
            break;
    }

    return caps;
}

// Progress and error handling
int ocgd_AdvancedMeshExporter::get_progress() const {
    if (_progress_total == 0) return 0;
    return (100 * _progress_current) / _progress_total;
}

String ocgd_AdvancedMeshExporter::get_last_error() const {
    return _last_error;
}

void ocgd_AdvancedMeshExporter::clear_error() {
    _last_error = "";
}

// Utility methods
bool ocgd_AdvancedMeshExporter::triangulate_shape(const Ref<ocgd_TopoDS_Shape>& shape) const {
    if (shape.is_null()) {
        return false;
    }

    try {
        const TopoDS_Shape& occt_shape = shape->get_occt_shape();

        BRepMesh_IncrementalMesh mesh(occt_shape, _linear_deflection, _relative_deflection, _angular_deflection);
        mesh.SetParallelDefault(_parallel_processing);
        mesh.Perform();

        return mesh.IsDone();

    } catch (const Standard_Failure& e) {
        return false;
    }
}

Dictionary ocgd_AdvancedMeshExporter::get_mesh_statistics(const Ref<ocgd_TopoDS_Shape>& shape) const {
    Dictionary stats;

    if (shape.is_null()) {
        return stats;
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    int vertex_count = 0;
    int triangle_count = 0;
    int face_count = 0;

    TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
    while (face_explorer.More()) {
        face_count++;

        const TopoDS_Face& face = TopoDS::Face(face_explorer.Current());
        TopLoc_Location location;
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);

        if (!triangulation.IsNull()) {
            vertex_count += triangulation->NbNodes();
            triangle_count += triangulation->NbTriangles();
        }

        face_explorer.Next();
    }

    stats["vertex_count"] = vertex_count;
    stats["triangle_count"] = triangle_count;
    stats["face_count"] = face_count;

    return stats;
}

Dictionary ocgd_AdvancedMeshExporter::optimize_mesh_data(const Dictionary& mesh_data) const {
    // For now, return the data as-is
    // This could be enhanced with vertex merging, normal smoothing, etc.
    return mesh_data;
}

// Private helper methods
RWMesh_CoordinateSystemConverter ocgd_AdvancedMeshExporter::create_coordinate_converter() const {
    RWMesh_CoordinateSystemConverter converter;

    // Set up coordinate system conversion based on target system
    switch (_coordinate_system) {
        case COORD_BLENDER:
            // Blender uses Z-up, right-handed (same as OCCT)
            break;
        case COORD_UNITY:
            // Unity uses Y-up, left-handed
            converter.SetInputLengthUnit(1.0);
            converter.SetInputCoordinateSystem(RWMesh_CoordinateSystem_Zup);
            converter.SetOutputCoordinateSystem(RWMesh_CoordinateSystem_Yup);
            break;
        case COORD_UNREAL:
            // Unreal uses Z-up, left-handed
            converter.SetInputLengthUnit(1.0);
            converter.SetInputCoordinateSystem(RWMesh_CoordinateSystem_Zup);
            converter.SetOutputCoordinateSystem(RWMesh_CoordinateSystem_Zup);
            break;
        case COORD_CUSTOM:
            // Custom transformation would be applied here
            break;
        default:
            // Keep OCCT coordinate system
            break;
    }

    return converter;
}

PackedColorArray ocgd_AdvancedMeshExporter::extract_colors(const TopoDS_Shape& shape) const {
    PackedColorArray colors;

    // This is a placeholder implementation
    // Real color extraction would use XCAF tools to get colors from CAD files
    Color default_color(0.7f, 0.7f, 0.7f, 1.0f);

    TopExp_Explorer face_explorer(shape, TopAbs_FACE);
    while (face_explorer.More()) {
        colors.append(default_color);
        face_explorer.Next();
    }

    return colors;
}

void ocgd_AdvancedMeshExporter::transform_mesh_data(PackedVector3Array& vertices,
                                                   PackedVector3Array& normals) const {
    if (_coordinate_system == COORD_OCCT) {
        return; // No transformation needed
    }

    // Apply coordinate system transformation
    for (int i = 0; i < vertices.size(); i++) {
        Vector3& v = vertices[i];

        switch (_coordinate_system) {
            case COORD_UNITY:
                // Convert Z-up right-handed to Y-up left-handed
                std::swap(v.y, v.z);
                v.x = -v.x;
                break;
            case COORD_UNREAL:
                // Convert to Unreal coordinate system (scaled to cm)
                v *= 100.0f; // Convert from meters to centimeters
                break;
            default:
                break;
        }
    }

    // Transform normals if present
    for (int i = 0; i < normals.size(); i++) {
        Vector3& n = normals[i];

        switch (_coordinate_system) {
            case COORD_UNITY:
                std::swap(n.y, n.z);
                n.x = -n.x;
                break;
            default:
                break;
        }
    }
}

bool ocgd_AdvancedMeshExporter::export_ply(const TopoDS_Shape& shape, const String& file_path) {
    try {
        // For now, use a simple PLY export implementation
        // This could be enhanced to use RWPly_CafWriter for full feature support

        Dictionary mesh_data = extract_mesh_data(Ref<ocgd_TopoDS_Shape>());
        if (mesh_data.is_empty()) {
            set_error("Failed to extract mesh data for PLY export");
            return false;
        }

        PackedVector3Array vertices = mesh_data.get("vertices", PackedVector3Array());
        PackedInt32Array indices = mesh_data.get("indices", PackedInt32Array());
        PackedVector3Array normals = mesh_data.get("normals", PackedVector3Array());

        std::ofstream file(file_path.utf8().get_data());
        if (!file.is_open()) {
            set_error("Failed to open PLY file for writing");
            return false;
        }

        // Write PLY header
        file << "ply\n";
        file << "format ascii 1.0\n";
        file << "element vertex " << vertices.size() << "\n";
        file << "property float x\n";
        file << "property float y\n";
        file << "property float z\n";

        if (normals.size() > 0) {
            file << "property float nx\n";
            file << "property float ny\n";
            file << "property float nz\n";
        }

        file << "element face " << (indices.size() / 3) << "\n";
        file << "property list uchar int vertex_indices\n";
        file << "end_header\n";

        // Write vertices
        for (int i = 0; i < vertices.size(); i++) {
            const Vector3& v = vertices[i];
            file << v.x << " " << v.y << " " << v.z;

            if (i < normals.size()) {
                const Vector3& n = normals[i];
                file << " " << n.x << " " << n.y << " " << n.z;
            }

            file << "\n";
        }

        // Write faces
        for (int i = 0; i < indices.size(); i += 3) {
            file << "3 " << indices[i] << " " << indices[i+1] << " " << indices[i+2] << "\n";
        }

        file.close();
        return true;

    } catch (const std::exception& e) {
        set_error(String("PLY export failed: ") + String(e.what()));
        return false;
    }
}

bool ocgd_AdvancedMeshExporter::export_obj(const TopoDS_Shape& shape, const String& file_path) {
    try {
        // Simple OBJ export implementation
        Dictionary mesh_data = extract_mesh_data(Ref<ocgd_TopoDS_Shape>());
        if (mesh_data.is_empty()) {
            set_error("Failed to extract mesh data for OBJ export");
            return false;
        }

        PackedVector3Array vertices = mesh_data.get("vertices", PackedVector3Array());
        PackedInt32Array indices = mesh_data.get("indices", PackedInt32Array());
        PackedVector3Array normals = mesh_data.get("normals", PackedVector3Array());

        std::ofstream file(file_path.utf8().get_data());
        if (!file.is_open()) {
            set_error("Failed to open OBJ file for writing");
            return false;
        }

        file << "# OBJ file exported from OpenCASCADE.gd\n";

        // Write vertices
        for (int i = 0; i < vertices.size(); i++) {
            const Vector3& v = vertices[i];
            file << "v " << v.x << " " << v.y << " " << v.z << "\n";
        }

        // Write normals
        for (int i = 0; i < normals.size(); i++) {
            const Vector3& n = normals[i];
            file << "vn " << n.x << " " << n.y << " " << n.z << "\n";
        }

        // Write faces
        for (int i = 0; i < indices.size(); i += 3) {
            if (normals.size() > 0) {
                file << "f " << (indices[i]+1) << "//" << (indices[i]+1) << " "
                     << (indices[i+1]+1) << "//" << (indices[i+1]+1) << " "
                     << (indices[i+2]+1) << "//" << (indices[i+2]+1) << "\n";
            } else {
                file << "f " << (indices[i]+1) << " " << (indices[i+1]+1) << " " << (indices[i+2]+1) << "\n";
            }
        }

        file.close();
        return true;

    } catch (const std::exception& e) {
        set_error(String("OBJ export failed: ") + String(e.what()));
        return false;
    }
}

bool ocgd_AdvancedMeshExporter::export_stl(const TopoDS_Shape& shape, const String& file_path) {
    try {
        StlAPI_Writer writer;

        if (_format == FORMAT_STL_ASCII) {
            writer.ASCIIMode() = Standard_True;
        } else {
            writer.ASCIIMode() = Standard_False;
        }

        return writer.Write(shape, file_path.utf8().get_data()) == Standard_True;

    } catch (const Standard_Failure& e) {
        set_error(String("STL export failed: ") + String(e.GetMessageString()));
        return false;
    }
}

void ocgd_AdvancedMeshExporter::update_progress(int current, int total) const {
    _progress_current = current;
    _progress_total = total;
}

void ocgd_AdvancedMeshExporter::set_error(const String& error) const {
    _last_error = error;
    UtilityFunctions::printerr("AdvancedMeshExporter: " + error);
}
