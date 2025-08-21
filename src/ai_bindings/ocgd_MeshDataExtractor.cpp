/**
 * ocgd_MeshDataExtractor.cpp
 *
 * Godot GDExtension wrapper implementation for extracting triangulation data from OpenCASCADE shapes.
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "ocgd_MeshDataExtractor.hxx"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <opencascade/TopoDS.hxx>
#include <opencascade/TopExp_Explorer.hxx>
#include <opencascade/BRep_Tool.hxx>
#include <opencascade/Poly_Triangulation.hxx>
#include <opencascade/Poly_Triangle.hxx>
#include <opencascade/TColgp_Array1OfPnt.hxx>
#include <opencascade/TColgp_Array1OfPnt2d.hxx>
#include <opencascade/TShort_Array1OfShortReal.hxx>
#include <opencascade/gp_Pnt.hxx>
#include <opencascade/gp_Pnt2d.hxx>
#include <opencascade/gp_Vec.hxx>
#include <opencascade/TopLoc_Location.hxx>
#include <opencascade/gp_Trsf.hxx>
#include <opencascade/Bnd_Box.hxx>
#include <opencascade/BRepBndLib.hxx>
#include <opencascade/TopoDS.hxx>

#include <vector>
#include <cmath>

using namespace godot;

ocgd_MeshDataExtractor::ocgd_MeshDataExtractor() {
    _include_normals = true;
    _include_uvs = true;
    _merge_vertices = false;
    _vertex_merge_tolerance = 1e-6;
}

ocgd_MeshDataExtractor::~ocgd_MeshDataExtractor() {
    // No cleanup needed
}

void ocgd_MeshDataExtractor::set_include_normals(bool include) {
    _include_normals = include;
}

bool ocgd_MeshDataExtractor::get_include_normals() const {
    return _include_normals;
}

void ocgd_MeshDataExtractor::set_include_uvs(bool include) {
    _include_uvs = include;
}

bool ocgd_MeshDataExtractor::get_include_uvs() const {
    return _include_uvs;
}

void ocgd_MeshDataExtractor::set_merge_vertices(bool merge) {
    _merge_vertices = merge;
}

bool ocgd_MeshDataExtractor::get_merge_vertices() const {
    return _merge_vertices;
}

void ocgd_MeshDataExtractor::set_vertex_merge_tolerance(double tolerance) {
    _vertex_merge_tolerance = tolerance;
}

double ocgd_MeshDataExtractor::get_vertex_merge_tolerance() const {
    return _vertex_merge_tolerance;
}

Dictionary ocgd_MeshDataExtractor::extract_mesh_data(const Ref<ocgd_TopoDS_Shape>& shape) {
    return extract_mesh_data_with_purpose(shape, MESH_PURPOSE_NONE);
}

Dictionary ocgd_MeshDataExtractor::extract_mesh_data_with_purpose(const Ref<ocgd_TopoDS_Shape>& shape, int purpose) {
    Dictionary result;
    PackedVector3Array all_vertices;
    PackedInt32Array all_triangles;
    PackedVector3Array all_normals;
    PackedVector2Array all_uvs;

    if (shape.is_null() || shape->is_null()) {
        UtilityFunctions::printerr("MeshDataExtractor: Cannot extract from null shape");
        return result;
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    // Explore all faces in the shape
    TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
    int vertex_offset = 0;

    while (face_explorer.More()) {
        const TopoDS_Face& face = TopoDS::Face(face_explorer.Current());

        // Get triangulation for this face
        Handle(Poly_Triangulation) triangulation = get_face_triangulation(face, purpose);
        if (triangulation.IsNull()) {
            face_explorer.Next();
            continue;
        }

        // Get face location
        TopLoc_Location face_location;
        BRep_Tool::Triangulation(face, face_location);

        // Convert triangulation to Godot arrays
        Dictionary face_data = convert_triangulation_to_dict(triangulation, face_location);

        if (face_data.has("vertices")) {
            PackedVector3Array face_vertices = face_data["vertices"];
            PackedInt32Array face_triangles = face_data["triangles"];

            // Append vertices
            for (int i = 0; i < face_vertices.size(); i++) {
                all_vertices.append(face_vertices[i]);
            }

            // Append triangles with vertex offset
            for (int i = 0; i < face_triangles.size(); i++) {
                all_triangles.append(face_triangles[i] + vertex_offset);
            }

            // Append normals if available and requested
            if (_include_normals && face_data.has("normals")) {
                PackedVector3Array face_normals = face_data["normals"];
                for (int i = 0; i < face_normals.size(); i++) {
                    all_normals.append(face_normals[i]);
                }
            }

            // Append UVs if available and requested
            if (_include_uvs && face_data.has("uvs")) {
                PackedVector2Array face_uvs = face_data["uvs"];
                for (int i = 0; i < face_uvs.size(); i++) {
                    all_uvs.append(face_uvs[i]);
                }
            }

            vertex_offset += face_vertices.size();
        }

        face_explorer.Next();
    }

    // Apply vertex merging if requested
    if (_merge_vertices && all_vertices.size() > 0) {
        merge_duplicate_vertices(all_vertices, all_triangles, all_normals, all_uvs);
    }

    // Build result dictionary
    result["vertices"] = all_vertices;
    result["triangles"] = all_triangles;

    if (_include_normals && all_normals.size() > 0) {
        result["normals"] = all_normals;
    }

    if (_include_uvs && all_uvs.size() > 0) {
        result["uvs"] = all_uvs;
    }

    return result;
}

Dictionary ocgd_MeshDataExtractor::extract_face_data(const Ref<ocgd_TopoDS_Shape>& face) {
    Dictionary result;

    if (face.is_null() || face->is_null()) {
        UtilityFunctions::printerr("MeshDataExtractor: Cannot extract from null face");
        return result;
    }

    const TopoDS_Shape& occt_shape = face->get_occt_shape();
    if (occt_shape.ShapeType() != TopAbs_FACE) {
        UtilityFunctions::printerr("MeshDataExtractor: Shape is not a face");
        return result;
    }

    const TopoDS_Face& occt_face = TopoDS::Face(occt_shape);

    // Get triangulation
    TopLoc_Location location;
    Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(occt_face, location);

    if (triangulation.IsNull()) {
        UtilityFunctions::printerr("MeshDataExtractor: Face has no triangulation");
        return result;
    }

    return convert_triangulation_to_dict(triangulation, location);
}

PackedVector3Array ocgd_MeshDataExtractor::extract_vertices(const Ref<ocgd_TopoDS_Shape>& shape) {
    Dictionary mesh_data = extract_mesh_data(shape);
    if (mesh_data.has("vertices")) {
        return mesh_data["vertices"];
    }
    return PackedVector3Array();
}

PackedInt32Array ocgd_MeshDataExtractor::extract_triangles(const Ref<ocgd_TopoDS_Shape>& shape) {
    Dictionary mesh_data = extract_mesh_data(shape);
    if (mesh_data.has("triangles")) {
        return mesh_data["triangles"];
    }
    return PackedInt32Array();
}

PackedVector3Array ocgd_MeshDataExtractor::extract_normals(const Ref<ocgd_TopoDS_Shape>& shape) {
    Dictionary mesh_data = extract_mesh_data(shape);
    if (mesh_data.has("normals")) {
        return mesh_data["normals"];
    }
    return PackedVector3Array();
}

PackedVector2Array ocgd_MeshDataExtractor::extract_uvs(const Ref<ocgd_TopoDS_Shape>& shape) {
    Dictionary mesh_data = extract_mesh_data(shape);
    if (mesh_data.has("uvs")) {
        return mesh_data["uvs"];
    }
    return PackedVector2Array();
}

Array ocgd_MeshDataExtractor::extract_per_face_data(const Ref<ocgd_TopoDS_Shape>& shape) {
    Array result;

    if (shape.is_null() || shape->is_null()) {
        UtilityFunctions::printerr("MeshDataExtractor: Cannot extract from null shape");
        return result;
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    // Explore all faces in the shape
    TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
    int face_index = 0;

    while (face_explorer.More()) {
        const TopoDS_Face& face = TopoDS::Face(face_explorer.Current());

        // Create a shape wrapper for this face
        Ref<ocgd_TopoDS_Shape> face_shape = memnew(ocgd_TopoDS_Shape);
        face_shape->set_occt_shape(face);

        // Extract data for this face
        Dictionary face_data = extract_face_data(face_shape);
        face_data["face_index"] = face_index;

        result.append(face_data);

        face_explorer.Next();
        face_index++;
    }

    return result;
}

bool ocgd_MeshDataExtractor::has_triangulation(const Ref<ocgd_TopoDS_Shape>& shape) const {
    if (shape.is_null() || shape->is_null()) {
        return false;
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    // Check if any face has triangulation
    TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
    while (face_explorer.More()) {
        const TopoDS_Face& face = TopoDS::Face(face_explorer.Current());

        TopLoc_Location location;
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);

        if (!triangulation.IsNull()) {
            return true;
        }

        face_explorer.Next();
    }

    return false;
}

bool ocgd_MeshDataExtractor::face_has_triangulation(const Ref<ocgd_TopoDS_Shape>& face) const {
    if (face.is_null() || face->is_null()) {
        return false;
    }

    const TopoDS_Shape& occt_shape = face->get_occt_shape();
    if (occt_shape.ShapeType() != TopAbs_FACE) {
        return false;
    }

    const TopoDS_Face& occt_face = TopoDS::Face(occt_shape);

    TopLoc_Location location;
    Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(occt_face, location);

    return !triangulation.IsNull();
}

Dictionary ocgd_MeshDataExtractor::get_triangulation_stats(const Ref<ocgd_TopoDS_Shape>& shape) const {
    Dictionary stats;

    if (shape.is_null() || shape->is_null()) {
        stats["face_count"] = 0;
        stats["triangle_count"] = 0;
        stats["vertex_count"] = 0;
        return stats;
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    int face_count = 0;
    int total_triangles = 0;
    int total_vertices = 0;

    TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
    while (face_explorer.More()) {
        const TopoDS_Face& face = TopoDS::Face(face_explorer.Current());

        TopLoc_Location location;
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);

        if (!triangulation.IsNull()) {
            face_count++;
            total_triangles += triangulation->NbTriangles();
            total_vertices += triangulation->NbNodes();
        }

        face_explorer.Next();
    }

    stats["face_count"] = face_count;
    stats["triangle_count"] = total_triangles;
    stats["vertex_count"] = total_vertices;

    return stats;
}

Dictionary ocgd_MeshDataExtractor::get_triangulation_bounds(const Ref<ocgd_TopoDS_Shape>& shape) const {
    Dictionary bounds;

    if (shape.is_null() || shape->is_null()) {
        bounds["min"] = Vector3(0, 0, 0);
        bounds["max"] = Vector3(0, 0, 0);
        return bounds;
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    Bnd_Box bounding_box;
    BRepBndLib::Add(occt_shape, bounding_box);

    if (bounding_box.IsVoid()) {
        bounds["min"] = Vector3(0, 0, 0);
        bounds["max"] = Vector3(0, 0, 0);
        return bounds;
    }

    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    bounding_box.Get(xmin, ymin, zmin, xmax, ymax, zmax);

    bounds["min"] = Vector3(
        static_cast<float>(xmin),
        static_cast<float>(ymin),
        static_cast<float>(zmin)
    );
    bounds["max"] = Vector3(
        static_cast<float>(xmax),
        static_cast<float>(ymax),
        static_cast<float>(zmax)
    );

    return bounds;
}

double ocgd_MeshDataExtractor::get_triangulated_area(const Ref<ocgd_TopoDS_Shape>& shape) const {
    if (shape.is_null() || shape->is_null()) {
        return 0.0;
    }

    const TopoDS_Shape& occt_shape = shape->get_occt_shape();

    double total_area = 0.0;

    TopExp_Explorer face_explorer(occt_shape, TopAbs_FACE);
    while (face_explorer.More()) {
        const TopoDS_Face& face = TopoDS::Face(face_explorer.Current());

        TopLoc_Location location;
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);

        if (!triangulation.IsNull()) {
            total_area += compute_triangulated_area(triangulation, location);
        }

        face_explorer.Next();
    }

    return total_area;
}

Handle(Poly_Triangulation) ocgd_MeshDataExtractor::get_face_triangulation(const TopoDS_Face& face, int purpose) const {
    TopLoc_Location location;
    return BRep_Tool::Triangulation(face, location, static_cast<Poly_MeshPurpose>(purpose));
}

Dictionary ocgd_MeshDataExtractor::convert_triangulation_to_dict(const Handle(Poly_Triangulation)& triangulation,
                                                               const TopLoc_Location& location) const {
    Dictionary result;

    if (triangulation.IsNull()) {
        return result;
    }

    const Standard_Integer nb_nodes = triangulation->NbNodes();
    const Standard_Integer nb_triangles = triangulation->NbTriangles();

    // Extract vertices
    PackedVector3Array vertices;
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
    PackedInt32Array triangles;
    const Poly_Array1OfTriangle& tris = triangulation->InternalTriangles();

    for (Standard_Integer i = 1; i <= nb_triangles; i++) {
        const Poly_Triangle& triangle = tris.Value(i);
        Standard_Integer n1, n2, n3;
        triangle.Get(n1, n2, n3);

        // Convert to 0-based indexing
        triangles.append(n1 - 1);
        triangles.append(n2 - 1);
        triangles.append(n3 - 1);
    }

    result["vertices"] = vertices;
    result["triangles"] = triangles;

    // Extract normals if available and requested
    if (_include_normals && triangulation->HasNormals()) {
        PackedVector3Array normals;
        const NCollection_Array1<gp_Vec3f>& norms = triangulation->InternalNormals();

        for (Standard_Integer i = 1; i <= nb_nodes; i++) {
            const gp_Vec3f& norm = norms.Value(i);

            Vector3 normal(
                static_cast<float>(norm.x()),
                static_cast<float>(norm.y()),
                static_cast<float>(norm.z())
            );

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

        result["normals"] = normals;
    }

    // Extract UV coordinates if available and requested
    if (_include_uvs && triangulation->HasUVNodes()) {
        PackedVector2Array uvs;
        const Poly_ArrayOfUVNodes& uv_nodes = triangulation->InternalUVNodes();

        for (Standard_Integer i = 1; i <= nb_nodes; i++) {
            const gp_Pnt2d& uv = uv_nodes.Value(i);
            uvs.append(Vector2(
                static_cast<float>(uv.X()),
                static_cast<float>(uv.Y())
            ));
        }

        result["uvs"] = uvs;
    }

    return result;
}

void ocgd_MeshDataExtractor::apply_transformation(PackedVector3Array& vertices, const gp_Trsf& transformation) const {
    for (int i = 0; i < vertices.size(); i++) {
        Vector3 vertex = vertices[i];
        gp_Pnt point(vertex.x, vertex.y, vertex.z);
        point.Transform(transformation);
        vertices[i] = Vector3(
            static_cast<float>(point.X()),
            static_cast<float>(point.Y()),
            static_cast<float>(point.Z())
        );
    }
}

void ocgd_MeshDataExtractor::merge_duplicate_vertices(PackedVector3Array& vertices, PackedInt32Array& triangles,
                                                     PackedVector3Array& normals, PackedVector2Array& uvs) const {
    if (vertices.size() == 0) return;

    // Simple vertex merging implementation
    std::vector<int> index_map(vertices.size());
    std::vector<Vector3> unique_vertices;
    std::vector<Vector3> unique_normals;
    std::vector<Vector2> unique_uvs;

    const double tolerance_sq = _vertex_merge_tolerance * _vertex_merge_tolerance;

    for (int i = 0; i < vertices.size(); i++) {
        const Vector3& vertex = vertices[i];
        bool found = false;

        // Look for existing vertex within tolerance
        for (int j = 0; j < unique_vertices.size(); j++) {
            const Vector3& existing = unique_vertices[j];
            double dist_sq = vertex.distance_squared_to(existing);

            if (dist_sq < tolerance_sq) {
                index_map[i] = j;
                found = true;
                break;
            }
        }

        if (!found) {
            // Add new unique vertex
            index_map[i] = unique_vertices.size();
            unique_vertices.push_back(vertex);

            if (i < normals.size()) {
                unique_normals.push_back(normals[i]);
            }

            if (i < uvs.size()) {
                unique_uvs.push_back(uvs[i]);
            }
        }
    }

    // Rebuild arrays with unique vertices
    vertices.clear();
    normals.clear();
    uvs.clear();

    for (const Vector3& vertex : unique_vertices) {
        vertices.append(vertex);
    }

    for (const Vector3& normal : unique_normals) {
        normals.append(normal);
    }

    for (const Vector2& uv : unique_uvs) {
        uvs.append(uv);
    }

    // Update triangle indices
    for (int i = 0; i < triangles.size(); i++) {
        triangles[i] = index_map[triangles[i]];
    }
}

double ocgd_MeshDataExtractor::compute_triangulated_area(const Handle(Poly_Triangulation)& triangulation,
                                                       const TopLoc_Location& location) const {
    if (triangulation.IsNull()) {
        return 0.0;
    }

    const Poly_ArrayOfNodes& nodes = triangulation->InternalNodes();
    const Poly_Array1OfTriangle& triangles = triangulation->InternalTriangles();

    double total_area = 0.0;

    for (Standard_Integer i = 1; i <= triangulation->NbTriangles(); i++) {
        const Poly_Triangle& triangle = triangles.Value(i);
        Standard_Integer n1, n2, n3;
        triangle.Get(n1, n2, n3);

        gp_Pnt p1 = nodes.Value(n1);
        gp_Pnt p2 = nodes.Value(n2);
        gp_Pnt p3 = nodes.Value(n3);

        // Apply location transformation if present
        if (!location.IsIdentity()) {
            p1.Transform(location.Transformation());
            p2.Transform(location.Transformation());
            p3.Transform(location.Transformation());
        }

        // Calculate triangle area using cross product
        gp_Vec v1(p1, p2);
        gp_Vec v2(p1, p3);
        gp_Vec cross = v1.Crossed(v2);
        double area = 0.5 * cross.Magnitude();

        total_area += area;
    }

    return total_area;
}

void ocgd_MeshDataExtractor::_bind_methods() {
    // Property getters and setters
    ClassDB::bind_method(D_METHOD("set_include_normals", "include"), &ocgd_MeshDataExtractor::set_include_normals);
    ClassDB::bind_method(D_METHOD("get_include_normals"), &ocgd_MeshDataExtractor::get_include_normals);
    ClassDB::add_property("ocgd_MeshDataExtractor", PropertyInfo(Variant::BOOL, "include_normals"), "set_include_normals", "get_include_normals");

    ClassDB::bind_method(D_METHOD("set_include_uvs", "include"), &ocgd_MeshDataExtractor::set_include_uvs);
    ClassDB::bind_method(D_METHOD("get_include_uvs"), &ocgd_MeshDataExtractor::get_include_uvs);
    ClassDB::add_property("ocgd_MeshDataExtractor", PropertyInfo(Variant::BOOL, "include_uvs"), "set_include_uvs", "get_include_uvs");

    ClassDB::bind_method(D_METHOD("set_merge_vertices", "merge"), &ocgd_MeshDataExtractor::set_merge_vertices);
    ClassDB::bind_method(D_METHOD("get_merge_vertices"), &ocgd_MeshDataExtractor::get_merge_vertices);
    ClassDB::add_property("ocgd_MeshDataExtractor", PropertyInfo(Variant::BOOL, "merge_vertices"), "set_merge_vertices", "get_merge_vertices");

    ClassDB::bind_method(D_METHOD("set_vertex_merge_tolerance", "tolerance"), &ocgd_MeshDataExtractor::set_vertex_merge_tolerance);
    ClassDB::bind_method(D_METHOD("get_vertex_merge_tolerance"), &ocgd_MeshDataExtractor::get_vertex_merge_tolerance);
    ClassDB::add_property("ocgd_MeshDataExtractor", PropertyInfo(Variant::FLOAT, "vertex_merge_tolerance"), "set_vertex_merge_tolerance", "get_vertex_merge_tolerance");

    // Main extraction methods
    ClassDB::bind_method(D_METHOD("extract_mesh_data", "shape"), &ocgd_MeshDataExtractor::extract_mesh_data);
    ClassDB::bind_method(D_METHOD("extract_face_data", "face"), &ocgd_MeshDataExtractor::extract_face_data);
    ClassDB::bind_method(D_METHOD("extract_vertices", "shape"), &ocgd_MeshDataExtractor::extract_vertices);
    ClassDB::bind_method(D_METHOD("extract_triangles", "shape"), &ocgd_MeshDataExtractor::extract_triangles);
    ClassDB::bind_method(D_METHOD("extract_normals", "shape"), &ocgd_MeshDataExtractor::extract_normals);
    ClassDB::bind_method(D_METHOD("extract_uvs", "shape"), &ocgd_MeshDataExtractor::extract_uvs);
    ClassDB::bind_method(D_METHOD("extract_per_face_data", "shape"), &ocgd_MeshDataExtractor::extract_per_face_data);
    ClassDB::bind_method(D_METHOD("extract_mesh_data_with_purpose", "shape", "purpose"), &ocgd_MeshDataExtractor::extract_mesh_data_with_purpose);

    // Query methods
    ClassDB::bind_method(D_METHOD("has_triangulation", "shape"), &ocgd_MeshDataExtractor::has_triangulation);
    ClassDB::bind_method(D_METHOD("face_has_triangulation", "face"), &ocgd_MeshDataExtractor::face_has_triangulation);
    ClassDB::bind_method(D_METHOD("get_triangulation_stats", "shape"), &ocgd_MeshDataExtractor::get_triangulation_stats);
    ClassDB::bind_method(D_METHOD("get_triangulation_bounds", "shape"), &ocgd_MeshDataExtractor::get_triangulation_bounds);
    ClassDB::bind_method(D_METHOD("get_triangulated_area", "shape"), &ocgd_MeshDataExtractor::get_triangulated_area);

    // Mesh purpose enum constants
    BIND_ENUM_CONSTANT(MESH_PURPOSE_NONE);
    BIND_ENUM_CONSTANT(MESH_PURPOSE_USER);
    BIND_ENUM_CONSTANT(MESH_PURPOSE_PRESENTATION);
    BIND_ENUM_CONSTANT(MESH_PURPOSE_CALCULATION);
    BIND_ENUM_CONSTANT(MESH_PURPOSE_ACTIVE);
    BIND_ENUM_CONSTANT(MESH_PURPOSE_LOADED);
    BIND_ENUM_CONSTANT(MESH_PURPOSE_ANY_FALLBACK);
}
