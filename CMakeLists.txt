cmake_minimum_required(VERSION 3.10)

# Set the C++ standard and project name
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(OpenCASCADE.gd CXX)

# Godot C++ bindings
add_subdirectory(godot-cpp)
set(GODOTCPP_DISABLE_EXCEPTIONS OFF)  # Required for OpenCASCADE

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