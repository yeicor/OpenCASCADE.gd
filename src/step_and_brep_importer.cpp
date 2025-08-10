#include "step_and_brep_importer.h"

#include <STEPControl_Reader.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <Poly_Triangulation.hxx>
#include <BRepTools.hxx>
#include <Standard_TypeDef.hxx>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/array_mesh.hpp>

StepAndBRepImporter::StepAndBRepImporter() = default;

StepAndBRepImporter::~StepAndBRepImporter() = default;

void StepAndBRepImporter::_bind_methods() {
}

String StepAndBRepImporter::_get_importer_name() const {
    return "StepAndBrepImporter";
}

String StepAndBRepImporter::_get_visible_name() const {
    return "STEP & BRep Importer";
}

int32_t StepAndBRepImporter::_get_preset_count() const {
    return EditorImportPlugin::_get_preset_count(); // TODO: Presets for different precision levels (low, medium, high)
}

String StepAndBRepImporter::_get_preset_name(int32_t p_preset_index) const {
    return EditorImportPlugin::_get_preset_name(p_preset_index);
}

PackedStringArray StepAndBRepImporter::_get_recognized_extensions() const {
    PackedStringArray extensions;
    extensions.push_back("step");
    extensions.push_back("stp");
    extensions.push_back("brep");
    return extensions;
}

TypedArray<Dictionary> StepAndBRepImporter::_get_import_options(const String &p_path, int32_t p_preset_index) const {
    // TODO: Detailed options (angular deflection, uv generation, etc.)
    return EditorImportPlugin::_get_import_options(p_path, p_preset_index);
}

bool StepAndBRepImporter::_get_option_visibility(const String &p_path, const StringName &p_option_name,
                                                 const Dictionary &p_options) const {
    return EditorImportPlugin::_get_option_visibility(p_path, p_option_name, p_options);
}

String StepAndBRepImporter::_get_save_extension() const {
    return "mesh";
}

String StepAndBRepImporter::_get_resource_type() const {
    return "Mesh";
}

float StepAndBRepImporter::_get_priority() const {
    return EditorImportPlugin::_get_priority();
}

int32_t StepAndBRepImporter::_get_import_order() const {
    return EditorImportPlugin::_get_import_order();
}

int32_t StepAndBRepImporter::_get_format_version() const {
    return EditorImportPlugin::_get_format_version();
}

Error StepAndBRepImporter::_import(const String &p_source_file, const String &p_save_path, const Dictionary &p_options,
                                   const TypedArray<String> &p_platform_variants,
                                   const TypedArray<String> &p_gen_files) const {
    TopoDS_Shape shape;
    if (p_source_file.ends_with(".step") || p_source_file.ends_with(".stp")) {
        STEPControl_Reader reader;
        IFSelect_ReturnStatus status = reader.ReadFile(p_source_file.utf8().get_data());
        if (status != IFSelect_RetDone) {
            ERR_PRINT("Failed to read STEP file.");
            return ERR_FILE_CANT_OPEN;
        }
        if (!reader.TransferRoots()) {
            ERR_PRINT("Failed to transfer STEP roots.");
            return ERR_CANT_CREATE;
        }
        shape = reader.OneShape();
    } else if (p_source_file.ends_with(".brep")) {
        BRep_Builder builder;
        Standard_Boolean result = BRepTools::Read(shape, p_source_file.utf8().get_data(), builder);
        if (!result) {
            ERR_PRINT("Failed to read BREP file.");
            return ERR_FILE_CANT_OPEN;
        }
    } else {
        ERR_PRINT("Unsupported file format. Only STEP and BREP files are supported.");
        return ERR_UNAVAILABLE;
    }

    double linear_deflection = 0.01;
    double angular_deflection = 0.1;
    BRepMesh_IncrementalMesh mesher(shape, linear_deflection, false, angular_deflection, true);

    Ref<ArrayMesh> mesh;
    mesh.instantiate();

    for (TopExp_Explorer faceExp(shape, TopAbs_FACE); faceExp.More(); faceExp.Next()) {
        TopoDS_Face face = TopoDS::Face(faceExp.Current());
        TopLoc_Location the_location; // TODO: Grab the location of the mesh/face?
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, the_location);

        if (triangulation.IsNull())
            continue;

        PackedVector3Array vertices;
        PackedInt32Array indices;

        for (int i = 1; i <= triangulation->NbNodes(); ++i) {
            gp_Pnt p = triangulation->Node(i);
            vertices.push_back(Vector3(p.X(), p.Y(), p.Z()));
        }

        for (int i = 1; i <= triangulation->NbTriangles(); ++i) {
            int n1, n2, n3;
            triangulation->Triangle(i).Get(n1, n2, n3);
            indices.push_back(n1 - 1);
            indices.push_back(n2 - 1);
            indices.push_back(n3 - 1);
        }

        Array arrays;
        arrays.resize(Mesh::ARRAY_MAX);
        arrays[Mesh::ARRAY_VERTEX] = vertices;
        arrays[Mesh::ARRAY_INDEX] = indices;

        mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
    }

    String save_path_with_ext = p_save_path + String(".") + _get_save_extension();
    ResourceSaver *saver = ResourceSaver::get_singleton();
    return saver->save(mesh, save_path_with_ext);
}

bool StepAndBRepImporter::_can_import_threaded() const {
    return EditorImportPlugin::_can_import_threaded();
}
