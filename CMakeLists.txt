MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(OpenCASCADE.gd CXX)

# Check that both CMAKE_BUILD_TYPE and GODOTCPP_TARGET are set correctly (always required)
if (NOT CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(FATAL_ERROR "CMAKE_BUILD_TYPE must be either 'Release' or 'Debug'. Current value: ${CMAKE_BUILD_TYPE}")
endif ()
if (NOT GODOTCPP_TARGET STREQUAL "template_release" AND NOT GODOTCPP_TARGET STREQUAL "template_debug")
    message(FATAL_ERROR "GODOTCPP_TARGET must be either 'template_release' or 'template_debug'. Current value: ${GODOTCPP_TARGET}")
endif ()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}, GODOTCPP_TARGET: ${GODOTCPP_TARGET}")
# No-op when CMAKE_BUILD_TYPE does not match the GODOTCPP_TARGET
if (CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT GODOTCPP_TARGET STREQUAL "template_release")
    message(WARNING "CMAKE_BUILD_TYPE is 'Release' but GODOTCPP_TARGET is not 'template_release'. Ignoring this build to speed up the build process.")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/") # No-op installation
    return()
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT GODOTCPP_TARGET STREQUAL "template_debug")
    message(WARNING "CMAKE_BUILD_TYPE is 'Debug' but GODOTCPP_TARGET is not 'template_debug'. Ignoring this build to speed up the build process.")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/") # No-op installation
    return()
endif ()

# Auto-enable ccache-like functionality if available since we are rebuilding godot-cpp and the bindings frequently
# If ccache, sccache or similar is found, use it as the CMAKE_{C,CXX}_COMPILER_LAUNCHER
find_program(CCACHE_EXECUTABLE ccache)
if(CCACHE_EXECUTABLE)
    message(STATUS "Using ccache as compiler launcher: ${CCACHE_EXECUTABLE}")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
endif()
find_program(SCCACHE_EXECUTABLE sccache)
if(SCCACHE_EXECUTABLE)
    message(STATUS "Using sccache as compiler launcher: ${SCCACHE_EXECUTABLE}")
    set(CMAKE_C_COMPILER_LAUNCHER ${SCCACHE_EXECUTABLE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${SCCACHE_EXECUTABLE})
endif()
if(NOT CCACHE_EXECUTABLE AND NOT SCCACHE_EXECUTABLE)
    message(STATUS "No ccache or sccache found, not using a compiler launcher")
endif()

# Godot C++ bindings
set(GODOTCPP_DISABLE_EXCEPTIONS OFF)  # OpenCASCADE requires exceptions
# FIXME: Godot tries to find cocoa for all apple platforms, open an issue in godot-cpp to fix this
if (APPLE AND IOS)
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp/cmake/macos.cmake" MACOS_CMAKE_CONTENTS)
    string(REPLACE "if(APPLE)" "if(FALSE)" MACOS_CMAKE_CONTENTS "${MACOS_CMAKE_CONTENTS}")
    file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp/cmake/macos.cmake" "${MACOS_CMAKE_CONTENTS}")
    message(WARNING "Modified godot-cpp macos.cmake to not include cocoa for iOS builds:\n${MACOS_CMAKE_CONTENTS}")
endif ()
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp")

# Dependencies
find_package(OpenCASCADE REQUIRED)

# Define the shared library target
file(GLOB_RECURSE SOURCES "src/step_importer/*.cpp")
add_library(${PROJECT_NAME}_importer SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME}_importer PRIVATE ${OpenCASCADE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_importer PRIVATE godot-cpp ${OpenCASCADE_LIBRARIES})

# Define the shared library target
file(GLOB_RECURSE SOURCES "src/gen_bindings/*.cpp")
if (SOURCES) # TODO: automate generating them during build
    add_library(${PROJECT_NAME}_bindings SHARED ${SOURCES})
    target_include_directories(${PROJECT_NAME}_bindings PRIVATE ${OpenCASCADE_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME}_bindings PRIVATE godot-cpp ${OpenCASCADE_LIBRARIES})
endif ()

# Install the complete bindings to the demo project with the correct suffix
get_target_property(GODOTCPP_SUFFIX godot-cpp GODOTCPP_SUFFIX)
set(OUTPUT_NAME "${PROJECT_NAME}${GODOTCPP_SUFFIX}")
if (NOT DEFINED _INSTALL_TO_ROOT)
    set(_INSTALL_TO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()
set(INSTALL_FOLDER "${_INSTALL_TO_ROOT}/demo/addons/${PROJECT_NAME}")
message("Will install shared library to: ${INSTALL_FOLDER}")
set_target_properties(${PROJECT_NAME}_importer PROPERTIES OUTPUT_NAME "${OUTPUT_NAME}")
install(TARGETS ${PROJECT_NAME}_importer LIBRARY DESTINATION "${INSTALL_FOLDER}")
# FIXME: Try harder to install to the proper directory on windows???
