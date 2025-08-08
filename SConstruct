#!/usr/bin/env python
import os
import sys
import re
import subprocess

libname = "OpenCASCADE.gd"

# Ensure submodules are initialized and updated
subprocess.run(["git", "submodule", "update", "--init", "--recursive"], check=True)

# Ensure vcpkg is installed
if not os.path.exists("vcpkg/vcpkg"):
    subprocess.run(["./vcpkg/bootstrap-vcpkg.sh", "--disableMetrics"], check=True)

# Custom variables
vars = Variables()
vars.Add('variant_dir', 'WIP: The (possibly out-of-tree) directory to place the variant build in', 'build')
localEnv = Environment(variables = vars)
Help(vars.GenerateHelpText(localEnv))

# Inherit the environment from the godot-cpp SConstruct file.
# TODO: Out-of-tree builds (and clean .gitignore). Godot 4.5: https://github.com/godotengine/godot-cpp/pull/1669/files
env = SConscript("godot-cpp/SConstruct", {'env': localEnv})
env.SConsignFile("${variant_dir}/.sconsign")

# Build the vcpkg library for the requested platform and architecture.
vcpkg_dir = os.path.join(os.getcwd(), "vcpkg")
vcpkg_platform = { "macos": "osx" }.get(env.get("platform"), env.get("platform"))
vcpkg_architecture = { "x86_64": "x64", "x86_32": "x86", "arm32": "arm" }.get(env.get("arch"), env.get("arch"))
vcpkg_triplet = "{}-{}".format(vcpkg_architecture, vcpkg_platform)
subprocess.run(['./vcpkg', 'install', 'opencascade[freetype,rapidjson,freeimage]', '--triplet', vcpkg_triplet], check=True, cwd=vcpkg_dir)

# Find all the static libraries built by vcpkg to link against.
vcpkg_include_dir = os.path.join(vcpkg_dir, "installed", vcpkg_triplet, "include")
vcpkg_lib_dir = os.path.join(vcpkg_dir, "installed", vcpkg_triplet, "lib")
vcpkg_libs = [os.path.basename(l) for l in os.listdir(vcpkg_lib_dir) if l.endswith((".a", ".lib"))]
vcpkg_libs = [re.replace(r'^lib', '', re.replace(r'\.(a|.lib)$', '', lib)) for lib in vcpkg_libs]
env.Append(LIBPATH=vcpkg_lib_dir, LIBS=vcpkg_libs, INCLUDE=vcpkg_include_dir)

# Find and generate all sources
sources = Glob("#src/*.cpp")
if env["target"] in ["editor", "template_debug"]:
    sources.append(env.GodotCPPDocData("#src/gen/doc_data.gen.cpp", source=Glob("#doc_classes/*.xml")))

# Build our shared library.
install_dir = "#demo/addons/OpenCASCADE.gd" # Build (install) directly into the demo project
suffix = env['suffix'].replace(".dev", "").replace(".universal", "")
lib_filename = "{}{}{}{}".format(env.subst('$SHLIBPREFIX'), libname, suffix, env.subst('$SHLIBSUFFIX'))
library = env.SharedLibrary(os.path.join(install_dir, lib_filename), source=sources)
Default(library) # Default to simply building the library.
