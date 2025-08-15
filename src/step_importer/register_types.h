#pragma once

#include <godot_cpp/classes/editor_import_plugin.hpp>

GDExtensionBool opencascade_gd_library_init_step_importer(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                                          GDExtensionClassLibraryPtr p_library,
                                                          GDExtensionInitialization* r_initialization);
