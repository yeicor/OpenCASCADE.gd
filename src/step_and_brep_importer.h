#pragma once

#include <godot_cpp/classes/editor_import_plugin.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

class StepAndBRepImporter : public EditorImportPlugin {
    GDCLASS(StepAndBRepImporter, EditorImportPlugin)

protected:
    static void _bind_methods();

public:
    StepAndBRepImporter() = default;
    ~StepAndBRepImporter() override = default;

    // TODO: Remove demo method
    void print_type(const Variant &p_variant) const;

    String _get_importer_name() const override;
    String _get_visible_name() const override;
    // virtual int32_t _get_preset_count() const; // TODO: Presets for different precision levels
    // virtual String _get_preset_name(int32_t p_preset_index) const;
    PackedStringArray _get_recognized_extensions() const override;
    // virtual TypedArray<Dictionary> _get_import_options(const String &p_path, int32_t p_preset_index) const; // TODO: Detailed options (angular deflection, uv generation, etc.)
    // virtual bool _get_option_visibility(const String &p_path, const StringName &p_option_name, const Dictionary &p_options) const;
    String _get_save_extension() const override;
    String _get_resource_type() const override;
    // virtual float _get_priority() const;
    // virtual int32_t _get_import_order() const;
    // virtual int32_t _get_format_version() const; // Increment this if the importer changes in a way that is not backward compatible.
    virtual Error _import(const String &p_source_file, const String &p_save_path, const Dictionary &p_options, const TypedArray<String> &p_platform_variants, const TypedArray<String> &p_gen_files) const;
    // virtual bool _can_import_threaded() const;
};
