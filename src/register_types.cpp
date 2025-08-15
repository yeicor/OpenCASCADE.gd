#include "step_importer/register_types.h"
#include "gen_bindings/ocgd_register.h"
extern "C" {
// Initialization
GDExtensionBool GDE_EXPORT opencascade_gd_library_init(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization *r_initialization) {
  opencascade_gd_library_init_step_importer(p_get_proc_address, p_library, r_initialization);
  ocgd_gd_library_init_bindings(p_get_proc_address, p_library, r_initialization);
}
}
