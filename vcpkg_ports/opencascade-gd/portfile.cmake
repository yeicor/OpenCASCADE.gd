# HACK: Copy the source files from the current repository to the buildtree (instead of using a clean repo reference).
set(FROM "${CURRENT_PORT_DIR}/../..")
set(TO "${CURRENT_BUILDTREES_DIR}/src/workspace")
file(MAKE_DIRECTORY "${TO}")
set(REPOS "." "godot-cpp")
foreach(REPO IN LISTS REPOS)
    execute_process(
        COMMAND git -C "${FROM}/${REPO}" archive --format=tar HEAD
        COMMAND tar -x -C "${TO}/${REPO}"
        WORKING_DIRECTORY "${FROM}/${REPO}"
        RESULT_VARIABLE COPY_RESULT
    )
    if(NOT COPY_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to copy sources from ${FROM} to ${TO}")
    endif()
endforeach()
message(STATUS "Copied sources from ${FROM} to ${TO} respecting .gitignore")

if(DEFINED ENV{__OPENCASCADE_GD_CMAKE_ARGS})
    string(REPLACE " " ";" PASSED_CMAKE_ARGS "$ENV{__OPENCASCADE_GD_CMAKE_ARGS}")
else()
    set(PASSED_CMAKE_ARGS "")
endif()
set(PASSED_CMAKE_ARGS "-D_INSTALL_TO_ROOT=${FROM};${PASSED_CMAKE_ARGS}")
message(STATUS "Using cmake args: ${PASSED_CMAKE_ARGS} (override with __OPENCASCADE_GD_CMAKE_ARGS)")
vcpkg_configure_cmake(
    SOURCE_PATH "${TO}"
    OPTIONS ${PASSED_CMAKE_ARGS}
    MAYBE_UNUSED_VARIABLES CMAKE_C_COMPILER_LAUNCHER _INSTALL_TO_ROOT
)

vcpkg_install_cmake()
vcpkg_copy_pdbs()

#vcpkg_cmake_config_fixup(PACKAGE_NAME "my_sample_lib")

vcpkg_install_copyright(FILE_LIST "${TO}/LICENSE")

set(VCPKG_POLICY_EMPTY_INCLUDE_FOLDER enabled)

# Write a timestamp file to this port directory to always rebuild it (using the updated local files)
string(TIMESTAMP CURRENT_TIME UTC)
file(WRITE "${CURRENT_PORT_DIR}/.last_build" "${CURRENT_TIME}\n")
message(STATUS "Wrote timestamp to ${CURRENT_PORT_DIR}/.last_build")