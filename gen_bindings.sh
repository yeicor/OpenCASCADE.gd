#!/bin/bash
# Regenerate the OpenCASCADE bindings for Godot
# Usage: ./gen_bindings.sh
set -e

# Check requirements
this_script_dir=$(dirname "$(realpath "$0")")
if [ ! -d "$this_script_dir" ]; then
    echo "Error: The script directory '$this_script_dir' does not exist."
    exit 1
fi
cd "$this_script_dir" || exit 1

generator_project_dir=${generator_project_dir:-"../gdextension-binding-generator"}
if [ ! -d "$generator_project_dir" ]; then
    echo "Error: The generator project directory '$generator_project_dir' does not exist. Clone it from git@github.com:yeicor/gdextension-binding-generator.git"
    exit 1
fi

poetry_cmd=$(command -v poetry)
if [ -z "$poetry_cmd" ]; then
    echo "Error: Poetry is not installed. Please install it to run this script."
    exit 1
fi

clang_cmd=$(command -v clang)
if [ -z "$clang_cmd" ]; then
    echo "Error: Clang is not installed. Please install it to run this script."
    exit 1
fi

vcpkg_include_dir="$this_script_dir/vcpkg/installed/x64-linux/include"
if [ ! -d "$vcpkg_include_dir" ]; then
    echo "Error: The vcpkg include directory '$vcpkg_include_dir' does not exist. Please run 'vcpkg/vcpkg install' first (initialize the vcpkg submodule first if necessary)."
    exit 1
fi

clang_args="-I$vcpkg_include_dir"
clang_args+=" -I$vcpkg_include_dir/opencascade"
clang_args+=" -I$(clang -print-resource-dir)/include"
clang_args+=" -std=c++17"

# Run the generator
cd "$generator_project_dir" || exit 1
set -ex
$poetry_cmd run generate-godot-bindings \
    --prefix ocgd_ -v \
    --headers "$vcpkg_include_dir/opencascade/Standard.hxx" \
    --include-filter "$vcpkg_include_dir/opencascade/" \
    --clang-args "$clang_args" \
    --output-dir "$this_script_dir/src/gen_bindings" \
    $@
