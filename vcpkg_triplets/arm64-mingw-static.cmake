include("triplets/community/arm64-mingw-static.cmake")

# CUSTOM:
if(PORT STREQUAL "libpng") # For libpng, disable PNG_ARM_NEON
    set(VCPKG_CMAKE_CONFIGURE_OPTIONS ${VCPKG_CMAKE_CONFIGURE_OPTIONS}
        -DPNG_ARM_NEON=OFF  # Disable PNG_ARM_NEON for libpng
    )
endif()