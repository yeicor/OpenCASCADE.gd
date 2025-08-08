#!/usr/bin/env python
import os
import sys
import re
import subprocess
import shutil

libname = "OpenCASCADE.gd"

# Ensure submodules are initialized and updated
subprocess.run(["git", "submodule", "update", "--init", "--recursive"], check=True)

# Ensure vcpkg is installed
vcpkg_root = os.path.join(os.getcwd(), "vcpkg")
vcpkg_exe = os.path.join(vcpkg_root, "vcpkg.exe" if sys.platform == "win32" else "vcpkg")
if not os.path.exists(vcpkg_exe):
    vcpkg_bootstrap = os.path.join(vcpkg_root, "bootstrap-vcpkg.bat" if sys.platform == "win32" else "bootstrap-vcpkg.sh")
    subprocess.run([vcpkg_bootstrap], shell=True, check=True)


# Custom variables
vars = Variables()
vars.Add('variant_dir', 'WIP: The (possibly out-of-tree) directory to place the variant build in', 'build')
vars.Add('skip_vcpkg_install', 'Skip installing vcpkg dependencies (for debugging)', False)
localEnv = Environment(variables = vars)
Help(vars.GenerateHelpText(localEnv))

# Inherit the environment from the godot-cpp SConstruct file.
# TODO: Out-of-tree builds (and clean .gitignore). Godot 4.5: https://github.com/godotengine/godot-cpp/pull/1669/files
env = SConscript("godot-cpp/SConstruct", {'env': localEnv})
env.SConsignFile("${variant_dir}/.sconsign")

# Print the compilers SCons found
print("CC (C compiler):", env.get('CC', 'Not found'))
print("CXX (C++ compiler):", env.get('CXX', 'Not found'))
print("AS (Assembler):", env.get('AS', 'Not found'))
print("LINK (Linker):", env.get('LINK', 'Not found'))
print("AR (Archiver):", env.get('AR', 'Not found'))
print(env.Dump())

# For every env variable ending in COM, add a corresponding COMSTR one with the same value.
to_add = {}
for key, value in env.items():
    if key.endswith("COM"):
        to_add[key.replace("COM", "COMSTR")] = '\n' + value
env.Append(**to_add)

if not env.get("skip_vcpkg_install"):
    # Build the vcpkg library for the requested platform and architecture.
    vcpkg_dir = os.path.join(os.getcwd(), "vcpkg")
    vcpkg_platform = { "macos": "osx", "web": "emscripten" }.get(env.get("platform"), env.get("platform"))
    vcpkg_architectures = { "x86_64": "x64", "x86_32": "x86", "arm32": "arm", "universal": "x64,arm64" } \
            .get(env.get("arch"), env.get("arch"))
    for vcpkg_architecture in vcpkg_architectures.split(","):
        vcpkg_triplet = "{}-{}".format(vcpkg_architecture, vcpkg_platform)
        # Disabled optional dependencies:
        # - freeimage fails on wasm32: libwebp: ISO C99 and later do not support implicit function declarations.
        # - vtk is a large dependency (didn't even try to build it).
        # - tbb is just for alternative threading.
        subprocess.run([vcpkg_exe, 'install', 'opencascade[freetype,rapidjson]', '--triplet', vcpkg_triplet], check=True)

    # Find all the static libraries built by vcpkg to link against.
    vcpkg_lib_dir = os.path.join(vcpkg_dir, "installed", vcpkg_triplet, "lib")
    if ',' in vcpkg_architectures: # Hack for universal macs: need to combine multiple architectures into one for each library.
        vcpkg_triplet = 'universal-osx'
        new_vcpkg_lib_dir = os.path.join(vcpkg_dir, "installed", vcpkg_triplet, "lib")
        def lipo_cmd(lib_fname):
            return ['lipo'] + \
                [os.path.join(vcpkg_dir, "installed", arch, lib_fname) for a in vcpkg_architectures.split(',')] + \
                ['-output', os.path.join(new_vcpkg_lib_dir, lib_fname), '-create']
        for lib_fname in os.listdir(vcpkg_lib_dir):
            if lib_fname.endswith(".a"):
                subprocess.run(lipo_cmd(lib_fname), check=True)
        shutil.copytree(vcpkg_include_dir, os.path.join(new_vcpkg_lib_dir, "..", "include")) # Copy any arch's headers
        vcpkg_lib_dir = new_vcpkg_lib_dir
    vcpkg_include_dir = os.path.join(vcpkg_dir, "installed", vcpkg_triplet, "include")
    vcpkg_libs = [os.path.basename(l) for l in os.listdir(vcpkg_lib_dir) if l.endswith((".a", ".lib"))]
    vcpkg_libs = [re.sub(r'^lib', '', re.sub(r'\.(a|.lib)$', '', lib)) for lib in vcpkg_libs]
    env.Append(LIBPATH=vcpkg_lib_dir, LIBS=vcpkg_libs, INCLUDE=vcpkg_include_dir)

# Find and generate all sources
sources = Glob("#src/*.cpp")
if env["target"] in ["editor", "template_debug"]:
    sources.append(env.GodotCPPDocData("#src/gen/doc_data.gen.cpp", source=Glob("#doc_classes/*.xml")))

# Build our shared library.
install_dir = f"#demo/addons/${libname}" # Build (install) directly into the demo project
suffix = env['suffix'].replace(".dev", "").replace(".universal", "")
lib_filename = "{}{}{}{}".format(env.subst('$SHLIBPREFIX'), libname, suffix, env.subst('$SHLIBSUFFIX'))
library = env.SharedLibrary(os.path.join(install_dir, lib_filename), source=sources)
Default(library) # Default to simply building the library.
