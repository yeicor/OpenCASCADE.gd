/**
 * ocgd_STEPCAFControl_Reader.cpp
 *
 * Godot GDExtension wrapper implementation for OpenCASCADE STEPCAFControl_Reader class.
 *
 * This file is part of OpenCASCADE.gd bindings.
 */

#include "ocgd_STEPCAFControl_Reader.hxx"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <opencascade/STEPCAFControl_Reader.hxx>
#include <opencascade/TDocStd_Document.hxx>
#include <opencascade/TDocStd_Application.hxx>
#include <opencascade/XCAFApp_Application.hxx>
#include <opencascade/XCAFDoc_DocumentTool.hxx>
#include <opencascade/XCAFDoc_ShapeTool.hxx>
#include <opencascade/TDataStd_Name.hxx>
#include <opencascade/TDF_LabelSequence.hxx>
#include <opencascade/Message_ProgressRange.hxx>

#include <sstream>

using namespace godot;

ocgd_STEPCAFControl_Reader::ocgd_STEPCAFControl_Reader() {
    _reader = new STEPCAFControl_Reader();
    _owns_reader = true;
    _document.Nullify();
}

ocgd_STEPCAFControl_Reader::~ocgd_STEPCAFControl_Reader() {
    if (_owns_reader && _reader != nullptr) {
        delete _reader;
        _reader = nullptr;
    }
}

void ocgd_STEPCAFControl_Reader::init() {
    if (_reader != nullptr) {
        Handle(XSControl_WorkSession) WS = new XSControl_WorkSession;
        _reader->Init(WS, Standard_True);
    }
}

int ocgd_STEPCAFControl_Reader::read_file(const String& filename) {
    if (_reader == nullptr) {
        UtilityFunctions::printerr("STEPCAFControl_Reader is not initialized");
        return -1;
    }

    std::string std_filename = filename.utf8().get_data();
    IFSelect_ReturnStatus status = _reader->ReadFile(std_filename.c_str());
    return static_cast<int>(status);
}

int ocgd_STEPCAFControl_Reader::read_stream(const String& name, const PackedByteArray& data) {
    if (_reader == nullptr) {
        UtilityFunctions::printerr("STEPCAFControl_Reader is not initialized");
        return -1;
    }

    std::string std_name = name.utf8().get_data();
    
    // Convert PackedByteArray to std::stringstream
    std::stringstream stream;
    const uint8_t* raw_data = data.ptr();
    size_t size = data.size();
    
    for (size_t i = 0; i < size; ++i) {
        stream << static_cast<char>(raw_data[i]);
    }
    
    IFSelect_ReturnStatus status = _reader->ReadStream(std_name.c_str(), stream);
    return static_cast<int>(status);
}

int ocgd_STEPCAFControl_Reader::nb_roots_for_transfer() {
    if (_reader == nullptr) {
        UtilityFunctions::printerr("STEPCAFControl_Reader is not initialized");
        return 0;
    }
    
    return _reader->NbRootsForTransfer();
}

Ref<ocgd_TopoDS_Shape> ocgd_STEPCAFControl_Reader::transfer_one_root(int num) {
    if (_reader == nullptr) {
        UtilityFunctions::printerr("STEPCAFControl_Reader is not initialized");
        return Ref<ocgd_TopoDS_Shape>();
    }

    // Create a new document for the transfer if needed
    if (_document.IsNull()) {
        Handle(TDocStd_Application) app = XCAFApp_Application::GetApplication();
        app->NewDocument("MDTV-XCAF", _document);
    }
    
    Message_ProgressRange progress;
    Standard_Boolean result = _reader->TransferOneRoot(num, _document, progress);
    
    if (result == Standard_False) {
        return Ref<ocgd_TopoDS_Shape>();
    }
    
    // Get the shape tool and find root shapes
    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(_document->Main());
    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);
    
    if (rootLabels.Length() > 0) {
        TopoDS_Shape shape = shapeTool->GetShape(rootLabels.Value(1));
        Ref<ocgd_TopoDS_Shape> result_shape = memnew(ocgd_TopoDS_Shape);
        result_shape->set_occt_shape(shape);
        return result_shape;
    }
    
    return Ref<ocgd_TopoDS_Shape>();
}

Ref<ocgd_TopoDS_Shape> ocgd_STEPCAFControl_Reader::transfer() {
    if (_reader == nullptr) {
        UtilityFunctions::printerr("STEPCAFControl_Reader is not initialized");
        return Ref<ocgd_TopoDS_Shape>();
    }

    // Create a new document for the transfer if needed
    if (_document.IsNull()) {
        Handle(TDocStd_Application) app = XCAFApp_Application::GetApplication();
        app->NewDocument("MDTV-XCAF", _document);
    }
    
    Message_ProgressRange progress;
    Standard_Boolean result = _reader->Transfer(_document, progress);
    
    if (result == Standard_False) {
        return Ref<ocgd_TopoDS_Shape>();
    }
    
    // Get the shape tool and find root shapes
    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(_document->Main());
    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);
    
    if (rootLabels.Length() > 0) {
        TopoDS_Shape shape = shapeTool->GetShape(rootLabels.Value(1));
        Ref<ocgd_TopoDS_Shape> result_shape = memnew(ocgd_TopoDS_Shape);
        result_shape->set_occt_shape(shape);
        return result_shape;
    }
    
    return Ref<ocgd_TopoDS_Shape>();
}

Ref<ocgd_TopoDS_Shape> ocgd_STEPCAFControl_Reader::perform(const String& filename) {
    if (_reader == nullptr) {
        UtilityFunctions::printerr("STEPCAFControl_Reader is not initialized");
        return Ref<ocgd_TopoDS_Shape>();
    }

    std::string std_filename = filename.utf8().get_data();
    
    // Create a new document for the transfer
    Handle(TDocStd_Application) app = XCAFApp_Application::GetApplication();
    app->NewDocument("MDTV-XCAF", _document);
    
    Message_ProgressRange progress;
    Standard_Boolean result = _reader->Perform(std_filename.c_str(), _document, progress);
    
    if (result == Standard_False) {
        return Ref<ocgd_TopoDS_Shape>();
    }
    
    // Get the shape tool and find root shapes
    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(_document->Main());
    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);
    
    if (rootLabels.Length() > 0) {
        TopoDS_Shape shape = shapeTool->GetShape(rootLabels.Value(1));
        Ref<ocgd_TopoDS_Shape> result_shape = memnew(ocgd_TopoDS_Shape);
        result_shape->set_occt_shape(shape);
        return result_shape;
    }
    
    return Ref<ocgd_TopoDS_Shape>();
}

Array ocgd_STEPCAFControl_Reader::get_root_shapes() const {
    Array shapes;
    
    if (_document.IsNull()) {
        return shapes;
    }
    
    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(_document->Main());
    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);
    
    for (Standard_Integer i = 1; i <= rootLabels.Length(); i++) {
        TopoDS_Shape shape = shapeTool->GetShape(rootLabels.Value(i));
        if (!shape.IsNull()) {
            Ref<ocgd_TopoDS_Shape> result_shape = memnew(ocgd_TopoDS_Shape);
            result_shape->set_occt_shape(shape);
            shapes.append(result_shape);
        }
    }
    
    return shapes;
}

String ocgd_STEPCAFControl_Reader::get_main_label() const {
    if (_document.IsNull()) {
        return "";
    }
    
    TDF_Label mainLabel = _document->Main();
    Handle(TDataStd_Name) name;
    if (mainLabel.FindAttribute(TDataStd_Name::GetID(), name)) {
        TCollection_ExtendedString extName = name->Get();
        return String(extName.ToExtString());
    }
    
    return "Main";
}

void ocgd_STEPCAFControl_Reader::set_color_mode(bool colormode) {
    if (_reader != nullptr) {
        _reader->SetColorMode(colormode ? Standard_True : Standard_False);
    }
}

bool ocgd_STEPCAFControl_Reader::get_color_mode() const {
    if (_reader != nullptr) {
        return _reader->GetColorMode() == Standard_True;
    }
    return false;
}

void ocgd_STEPCAFControl_Reader::set_name_mode(bool namemode) {
    if (_reader != nullptr) {
        _reader->SetNameMode(namemode ? Standard_True : Standard_False);
    }
}

bool ocgd_STEPCAFControl_Reader::get_name_mode() const {
    if (_reader != nullptr) {
        return _reader->GetNameMode() == Standard_True;
    }
    return false;
}

void ocgd_STEPCAFControl_Reader::set_layer_mode(bool layermode) {
    if (_reader != nullptr) {
        _reader->SetLayerMode(layermode ? Standard_True : Standard_False);
    }
}

bool ocgd_STEPCAFControl_Reader::get_layer_mode() const {
    if (_reader != nullptr) {
        return _reader->GetLayerMode() == Standard_True;
    }
    return false;
}

void ocgd_STEPCAFControl_Reader::set_props_mode(bool propsmode) {
    if (_reader != nullptr) {
        _reader->SetPropsMode(propsmode ? Standard_True : Standard_False);
    }
}

bool ocgd_STEPCAFControl_Reader::get_props_mode() const {
    if (_reader != nullptr) {
        return _reader->GetPropsMode() == Standard_True;
    }
    return false;
}

void ocgd_STEPCAFControl_Reader::set_meta_mode(bool metamode) {
    if (_reader != nullptr) {
        _reader->SetMetaMode(metamode ? Standard_True : Standard_False);
    }
}

bool ocgd_STEPCAFControl_Reader::get_meta_mode() const {
    if (_reader != nullptr) {
        return _reader->GetMetaMode() == Standard_True;
    }
    return false;
}

void ocgd_STEPCAFControl_Reader::set_product_meta_mode(bool productmetamode) {
    if (_reader != nullptr) {
        _reader->SetProductMetaMode(productmetamode ? Standard_True : Standard_False);
    }
}

bool ocgd_STEPCAFControl_Reader::get_product_meta_mode() const {
    if (_reader != nullptr) {
        return _reader->GetProductMetaMode() == Standard_True;
    }
    return false;
}

void ocgd_STEPCAFControl_Reader::set_shuo_mode(bool shuomode) {
    if (_reader != nullptr) {
        _reader->SetSHUOMode(shuomode ? Standard_True : Standard_False);
    }
}

bool ocgd_STEPCAFControl_Reader::get_shuo_mode() const {
    if (_reader != nullptr) {
        return _reader->GetSHUOMode() == Standard_True;
    }
    return false;
}

void ocgd_STEPCAFControl_Reader::set_gdt_mode(bool gdtmode) {
    if (_reader != nullptr) {
        _reader->SetGDTMode(gdtmode ? Standard_True : Standard_False);
    }
}

bool ocgd_STEPCAFControl_Reader::get_gdt_mode() const {
    if (_reader != nullptr) {
        return _reader->GetGDTMode() == Standard_True;
    }
    return false;
}

void ocgd_STEPCAFControl_Reader::set_mat_mode(bool matmode) {
    if (_reader != nullptr) {
        _reader->SetMatMode(matmode ? Standard_True : Standard_False);
    }
}

bool ocgd_STEPCAFControl_Reader::get_mat_mode() const {
    if (_reader != nullptr) {
        return _reader->GetMatMode() == Standard_True;
    }
    return false;
}

void ocgd_STEPCAFControl_Reader::set_view_mode(bool viewmode) {
    if (_reader != nullptr) {
        _reader->SetViewMode(viewmode ? Standard_True : Standard_False);
    }
}

bool ocgd_STEPCAFControl_Reader::get_view_mode() const {
    if (_reader != nullptr) {
        return _reader->GetViewMode() == Standard_True;
    }
    return false;
}

void ocgd_STEPCAFControl_Reader::_bind_methods() {
    // Constructor and setup methods
    ClassDB::bind_method(D_METHOD("init"), &ocgd_STEPCAFControl_Reader::init);
    
    // File I/O methods
    ClassDB::bind_method(D_METHOD("read_file", "filename"), &ocgd_STEPCAFControl_Reader::read_file);
    ClassDB::bind_method(D_METHOD("read_stream", "name", "data"), &ocgd_STEPCAFControl_Reader::read_stream);
    ClassDB::bind_method(D_METHOD("nb_roots_for_transfer"), &ocgd_STEPCAFControl_Reader::nb_roots_for_transfer);
    
    // Transfer methods
    ClassDB::bind_method(D_METHOD("transfer_one_root", "num"), &ocgd_STEPCAFControl_Reader::transfer_one_root);
    ClassDB::bind_method(D_METHOD("transfer"), &ocgd_STEPCAFControl_Reader::transfer);
    ClassDB::bind_method(D_METHOD("perform", "filename"), &ocgd_STEPCAFControl_Reader::perform);
    
    // Shape access methods
    ClassDB::bind_method(D_METHOD("get_root_shapes"), &ocgd_STEPCAFControl_Reader::get_root_shapes);
    ClassDB::bind_method(D_METHOD("get_main_label"), &ocgd_STEPCAFControl_Reader::get_main_label);
    
    // Mode setters and getters
    ClassDB::bind_method(D_METHOD("set_color_mode", "colormode"), &ocgd_STEPCAFControl_Reader::set_color_mode);
    ClassDB::bind_method(D_METHOD("get_color_mode"), &ocgd_STEPCAFControl_Reader::get_color_mode);
    ClassDB::add_property("ocgd_STEPCAFControl_Reader", PropertyInfo(Variant::BOOL, "color_mode"), "set_color_mode", "get_color_mode");
    
    ClassDB::bind_method(D_METHOD("set_name_mode", "namemode"), &ocgd_STEPCAFControl_Reader::set_name_mode);
    ClassDB::bind_method(D_METHOD("get_name_mode"), &ocgd_STEPCAFControl_Reader::get_name_mode);
    ClassDB::add_property("ocgd_STEPCAFControl_Reader", PropertyInfo(Variant::BOOL, "name_mode"), "set_name_mode", "get_name_mode");
    
    ClassDB::bind_method(D_METHOD("set_layer_mode", "layermode"), &ocgd_STEPCAFControl_Reader::set_layer_mode);
    ClassDB::bind_method(D_METHOD("get_layer_mode"), &ocgd_STEPCAFControl_Reader::get_layer_mode);
    ClassDB::add_property("ocgd_STEPCAFControl_Reader", PropertyInfo(Variant::BOOL, "layer_mode"), "set_layer_mode", "get_layer_mode");
    
    ClassDB::bind_method(D_METHOD("set_props_mode", "propsmode"), &ocgd_STEPCAFControl_Reader::set_props_mode);
    ClassDB::bind_method(D_METHOD("get_props_mode"), &ocgd_STEPCAFControl_Reader::get_props_mode);
    ClassDB::add_property("ocgd_STEPCAFControl_Reader", PropertyInfo(Variant::BOOL, "props_mode"), "set_props_mode", "get_props_mode");
    
    ClassDB::bind_method(D_METHOD("set_meta_mode", "metamode"), &ocgd_STEPCAFControl_Reader::set_meta_mode);
    ClassDB::bind_method(D_METHOD("get_meta_mode"), &ocgd_STEPCAFControl_Reader::get_meta_mode);
    ClassDB::add_property("ocgd_STEPCAFControl_Reader", PropertyInfo(Variant::BOOL, "meta_mode"), "set_meta_mode", "get_meta_mode");
    
    ClassDB::bind_method(D_METHOD("set_product_meta_mode", "productmetamode"), &ocgd_STEPCAFControl_Reader::set_product_meta_mode);
    ClassDB::bind_method(D_METHOD("get_product_meta_mode"), &ocgd_STEPCAFControl_Reader::get_product_meta_mode);
    ClassDB::add_property("ocgd_STEPCAFControl_Reader", PropertyInfo(Variant::BOOL, "product_meta_mode"), "set_product_meta_mode", "get_product_meta_mode");
    
    ClassDB::bind_method(D_METHOD("set_shuo_mode", "shuomode"), &ocgd_STEPCAFControl_Reader::set_shuo_mode);
    ClassDB::bind_method(D_METHOD("get_shuo_mode"), &ocgd_STEPCAFControl_Reader::get_shuo_mode);
    ClassDB::add_property("ocgd_STEPCAFControl_Reader", PropertyInfo(Variant::BOOL, "shuo_mode"), "set_shuo_mode", "get_shuo_mode");
    
    ClassDB::bind_method(D_METHOD("set_gdt_mode", "gdtmode"), &ocgd_STEPCAFControl_Reader::set_gdt_mode);
    ClassDB::bind_method(D_METHOD("get_gdt_mode"), &ocgd_STEPCAFControl_Reader::get_gdt_mode);
    ClassDB::add_property("ocgd_STEPCAFControl_Reader", PropertyInfo(Variant::BOOL, "gdt_mode"), "set_gdt_mode", "get_gdt_mode");
    
    ClassDB::bind_method(D_METHOD("set_mat_mode", "matmode"), &ocgd_STEPCAFControl_Reader::set_mat_mode);
    ClassDB::bind_method(D_METHOD("get_mat_mode"), &ocgd_STEPCAFControl_Reader::get_mat_mode);
    ClassDB::add_property("ocgd_STEPCAFControl_Reader", PropertyInfo(Variant::BOOL, "mat_mode"), "set_mat_mode", "get_mat_mode");
    
    ClassDB::bind_method(D_METHOD("set_view_mode", "viewmode"), &ocgd_STEPCAFControl_Reader::set_view_mode);
    ClassDB::bind_method(D_METHOD("get_view_mode"), &ocgd_STEPCAFControl_Reader::get_view_mode);
    ClassDB::add_property("ocgd_STEPCAFControl_Reader", PropertyInfo(Variant::BOOL, "view_mode"), "set_view_mode", "get_view_mode");
}