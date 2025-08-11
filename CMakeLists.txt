cmake_minimum_required(VERSION 3.10)

# Ensure to pick up the default triplet from the environment if any. This helps
# driving the vcpkg triplet in the same way either when starting vcpkg directly,
# or when letting CMake start vcpkg at configure/generate time.
# Note: this logic must happen before PROJECT command.
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "The vcpkg triplet")
endif()

# Set the C++ standard and project name
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(OpenCASCADE.gd CXX)

# Godot C++ bindings
add_subdirectory(godot-cpp)

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
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${GODOTCPP_SUFFIX}")
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/demo/addons/${PROJECT_NAME}")