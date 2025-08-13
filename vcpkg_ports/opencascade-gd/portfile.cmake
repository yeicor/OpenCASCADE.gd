# HACK: Copy the source files from the current repository to the buildtree (instead of using a clean repo reference).
set(FROM "${CURRENT_PORT_DIR}/../..")
set(TO "${CURRENT_BUILDTREES_DIR}/src/workspace")
file(REMOVE_RECURSE "${TO}")
set(REPOS "/" "/godot-cpp/")
foreach(REPO IN LISTS REPOS)
    file(MAKE_DIRECTORY "${TO}${REPO}")
    message(STATUS "Copying sources from ${FROM}${REPO} to ${TO}${REPO}")
    file(TO_NATIVE_PATH "${FROM}${REPO}" FROM_REPO_NATIVE)
    execute_process(
        COMMAND git ls-files
        WORKING_DIRECTORY "${FROM_REPO_NATIVE}"
        OUTPUT_VARIABLE REPO_FILES
        ERROR_VARIABLE REPO_FILES_ERROR
        RESULT_VARIABLE LS_RESULT
    )
    if(NOT LS_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to list files in ${FROM}${REPO}: ${REPO_FILES_ERROR}")
    endif()
    string(REPLACE "\n" ";" REPO_FILES_LIST "${REPO_FILES}")
    foreach(FILE IN LISTS REPO_FILES_LIST)
        if (NOT FILE)
            continue() # Skip empty lines
        endif()
        message(STATUS "Copying file ${FILE} for ${REPO}")
        if (NOT EXISTS "${FROM}${REPO}${FILE}")
            message(WARNING "File ${FROM}${REPO}${FILE} does not exist, skipping.")
            continue() # Skip files that do not exist
        endif()
        if (IS_DIRECTORY "${FROM}${REPO}${FILE}")
            message(WARNING "File ${FROM}${REPO}${FILE} is a directory, skipping.")
            continue() # Skip files that do not exist
        endif()
        get_filename_component(DIR "${FILE}" DIRECTORY)
        message(STATUS "Creating directory ${TO}${REPO}${DIR} for file ${FILE}")
        file(MAKE_DIRECTORY "${TO}${REPO}${DIR}")
        file(COPY "${FROM}${REPO}${FILE}" DESTINATION "${TO}${REPO}${DIR}")
    endforeach()
    if(NOT EXISTS "${TO}${REPO}CMakeLists.txt")
        file(GLOB MY_FILES "${TO}${REPO}*")
        message(FATAL_ERROR "Missing CMakeLists.txt in ${TO}${REPO} Found files at root: ${MY_FILES}")
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

# Write a timestamp file to this port directory to always rebuild it (needs a remove anyway, but avoids using the binary cache)
string(TIMESTAMP CURRENT_TIME UTC)
file(WRITE "${CURRENT_PORT_DIR}/.last_build" "${CURRENT_TIME}\n")
message(STATUS "Wrote timestamp to ${CURRENT_PORT_DIR}/.last_build")