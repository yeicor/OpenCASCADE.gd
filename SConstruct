#!/usr/bin/env python
import os
import sys

# Ensure submodules are initialized and updated
os.system("git submodule update --init --recursive")

libname = "OpenCASCADE.gd"

# Custom variables
vars = Variables()
vars.Add('variant_dir', 'WIP: The (possibly out-of-tree) directory to place the variant build in', 'build')
localEnv = Environment(variables = vars)
Help(vars.GenerateHelpText(localEnv))

# Inherit the environment from the godot-cpp SConstruct file.
# TODO: Out-of-tree builds (and clean .gitignore). Godot 4.5: https://github.com/godotengine/godot-cpp/pull/1669/files
env = SConscript("godot-cpp/SConstruct", {'env': localEnv})
env.SConsignFile("${variant_dir}/.sconsign")

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
