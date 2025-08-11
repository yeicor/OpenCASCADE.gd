cmake_minimum_required(VERSION 3.10)
project(OpenCASCADE.gd CXX)

## FIXME: No-op when CMAKE_BUILD_TYPE does not match the GODOTCPP_BUILD_TYPE
#if (CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT GODOTCPP_BUILD_TYPE STREQUAL "template_release")
#    message(WARNING "CMAKE_BUILD_TYPE is 'Release' but GODOTCPP_BUILD_TYPE is not 'template_release'. Ignoring this build to speed up the build process.")
#    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json") # No-op installation
#    return()
#elseif (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT GODOTCPP_BUILD_TYPE STREQUAL "template_debug")
#    message(WARNING "CMAKE_BUILD_TYPE is 'Debug' but GODOTCPP_BUILD_TYPE is not 'template_debug'. Ignoring this build to speed up the build process.")
#    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json") # No-op installation
#    return()
#elseif (NOT CMAKE_BUILD_TYPE)
#    message(FATAL_ERROR "CMAKE_BUILD_TYPE is not set. Please set it to 'Release' or 'Debug'.")
#elseif (NOT GODOTCPP_BUILD_TYPE)
#    message(FATAL_ERROR "GODOTCPP_BUILD_TYPE is not set. Please set it to 'template_release' or 'template_debug'.")
#endif ()

# Godot C++ bindings
set(GODOTCPP_DISABLE_EXCEPTIONS OFF)  # OpenCASCADE requires exceptions
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp")

# Dependencies
find_package(OpenCASCADE REQUIRED)

# Sources for this project (glob)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the shared library target
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCASCADE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE godot-cpp ${OpenCASCADE_LIBRARIES})

# Install to the demo project with the correct suffix
get_target_property(GODOTCPP_SUFFIX godot-cpp GODOTCPP_SUFFIX)
set(OUTPUT_NAME "${PROJECT_NAME}${GODOTCPP_SUFFIX}")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${OUTPUT_NAME}")
if (NOT DEFINED _INSTALL_TO_ROOT)
    set(_INSTALL_TO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${_INSTALL_TO_ROOT}/demo/addons/${PROJECT_NAME}")