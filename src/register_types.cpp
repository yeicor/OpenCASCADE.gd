#include "step_importer/register_types.h"
#ifdef OCGD_AUTO_BINDINGS_ENABLED
#include "gen_bindings/register_types.h"
#endif

extern "C" {
    // Initialization
    GDExtensionBool GDE_EXPORT opencascade_gd_library_init(
        GDExtensionInterfaceGetProcAddress p_get_proc_address,
        GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization *r_initialization) {
        GDExtensionBool r = opencascade_gd_library_init_step_importer(p_get_proc_address, p_library, r_initialization);
        if (!r) return r;
#ifdef OCGD_AUTO_BINDINGS_ENABLED
        return ocgd_library_init(p_get_proc_address, p_library, r_initialization);
#else
        return true;
#endif
    }
}
