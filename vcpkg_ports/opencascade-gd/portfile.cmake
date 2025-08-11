#vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO yeicor/OpenCASCADE.gd
    REF "90414c4fe87ae0614b7709323ddebe415f51d7de"
    SHA512 94cc2f7a2a513490c7ae71708ec5a625e5a523041ebf7226ceabf70ee04f5b339b2361cba655787079b3a03e046ef77ea4475312e3ea87cef262d9ce8e92228c
    HEAD_REF main
)

vcpkg_from_github(
    OUT_SOURCE_PATH GODOT_SOURCE_PATH
    REPO "godotengine/godot-cpp"
    REF "godot-4.4-stable"
    SHA512 "3c97d6f0bbd952977d8085483d538b650d44ee0f9c6d84215128d9702d071b23a91bacab3a5259320f89d11884b3a5d5b638bc757c11d7447c000223fa976de8"
    HEAD_REF "master"
    PATCHES "godot-cpp.patch"
)

# Move the Godot C++ bindings to the expected location
file(COPY "${GODOT_SOURCE_PATH}/" DESTINATION "${SOURCE_PATH}/godot-cpp/")

vcpkg_configure_cmake(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

#vcpkg_cmake_config_fixup(PACKAGE_NAME "my_sample_lib")

#file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

#file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
#vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")