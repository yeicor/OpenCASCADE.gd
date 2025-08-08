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

# For every env variable ending in COM, add a corresponding COMSTR one with the same value (i.e. print the command).
if sys.platform != "win32": env.Append(**{k.replace("COM", "COMSTR"): '\n'+v for k, v in env.items() if k.endswith("COM")})

# Build the vcpkg library for the requested platform and architecture.
vcpkg_dir = os.path.join(os.getcwd(), "vcpkg")
vcpkg_platform = { "macos": "osx", "web": "emscripten" }.get(env.get("platform"), env.get("platform"))
vcpkg_architecture = { "x86_64": "x64", "x86_32": "x86", "arm32": "arm" }.get(env.get("arch"), env.get("arch"))
if vcpkg_platform == "android" and vcpkg_architecture == "arm": vcpkg_architecture = "arm-neon" # More up to date NDK
vcpkg_custom_triplet_folder = os.path.join(os.getcwd(), "vcpkg_triplets")
vcpkg_triplet = "{}-{}".format(vcpkg_architecture, vcpkg_platform)
# Disabled optional dependencies:
# - freeimage fails on wasm32: libwebp: ISO C99 and later do not support implicit function declarations.
# - vtk is a large dependency (didn't even try to build it).
# - tbb is just for alternative threading.
subprocess.run([vcpkg_exe, 'install', 'opencascade[freetype,rapidjson]', '--triplet', vcpkg_triplet], check=True, env={})

# Find all the static libraries built by vcpkg to link against.
vcpkg_lib_dir = os.path.join(vcpkg_dir, "installed", vcpkg_triplet, "lib")
vcpkg_include_dir = os.path.join(vcpkg_dir, "installed", vcpkg_triplet, "include")
vcpkg_libs = [os.path.basename(l) for l in os.listdir(vcpkg_lib_dir) if l.endswith((".a", ".lib"))]
vcpkg_libs = [re.sub(r'^lib', '', re.sub(r'\.(a|.lib)$', '', lib)) for lib in vcpkg_libs]
env.Append(LIBPATH=vcpkg_lib_dir, LIBS=vcpkg_libs, CPPPATH=vcpkg_include_dir)
env.Append(CPPPATH=os.path.join(vcpkg_include_dir, "opencascade")) # Add the OpenCASCADE include directory

# Find and generate all sources
sources = Glob("#src/*.cpp")
if env["target"] in ["editor", "template_debug"]:
    sources.append(env.GodotCPPDocData("#src/gen/doc_data.gen.cpp", source=Glob("#doc_classes/*.xml")))

# Build our shared library.
install_dir = f"#demo/addons/{libname}" # Build (install) directly into the demo project
suffix = env['suffix'].replace(".dev", "").replace(".universal", "")
lib_filename = "{}{}{}{}".format(env.subst('$SHLIBPREFIX'), libname, suffix, env.subst('$SHLIBSUFFIX'))
library = env.SharedLibrary(os.path.join(install_dir, lib_filename), source=sources)
Default(library) # Default to simply building the library.

# Follow a little the convention: copy CC, CXX... if they are set in the environment.
for var in ["CC", "CXX", "CPP", "AR", "RANLIB", "STRIP", "CFLAGS", "CXXFLAGS", "CPPFLAGS", "LDFLAGS"]:
    if var in os.environ:
        env[var] = os.environ[var]