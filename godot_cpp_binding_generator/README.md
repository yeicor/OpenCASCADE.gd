# Godot C++ Binding Generator

Multi‑pass generator that scans third‑party C++ headers with libclang and emits Godot 4 GDExtension wrapper classes.

This README focuses on the common “three folders” workflow without breaking relative `#include`s:

- Your headers to wrap live in one folder.
- Those headers expect to be included relative to a different include root.
- You want the generated files written to a third folder.

The generator has a dedicated option to support this: `--include-rel-base`.

---

## Prerequisites

- Python 3.9+
- Python bindings for clang and the libclang shared library:
  - pip: `pip install clang`
  - libclang (shared library) available on your system
    - Debian/Ubuntu: `sudo apt-get install libclang-16` (or your LLVM version)
    - macOS (Homebrew): `brew install llvm` (libclang is inside the keg)
    - Windows: Install LLVM and make sure `libclang.dll` is reachable

If libclang is not on your dynamic loader path, use `--libclang-path /path/to/lib` (the directory that contains libclang).

---

## Repository layout and how to run without breaking imports

This project is a Python package named `godot_cpp_binding_generator`. To keep Python imports working, either:

- Run from the package’s parent directory:
  - `cd OpenCASCADE.gd`
  - `python -m godot_cpp_binding_generator.cli ...`
- Or add the parent directory to `PYTHONPATH`:
  - `PYTHONPATH=/path/to/OpenCASCADE.gd python -m godot_cpp_binding_generator.cli ...`

No installation step is required.

---

## Folder roles for the 3‑folder workflow

- `--include-rel-base` (the include root)
  - Directory that header `#include` directives are relative to.
  - It is added first to the clang `-I` search list and is also used to compute clean relative paths when you request `--include-originals`.
- `--headers ...` (the headers to wrap)
  - One or more globs pointing to the concrete headers you want scanned and wrapped.
  - These files may be anywhere; they don’t have to be directly under `--include-rel-base` (but typically are).
- `--out-dir ...` (where generated files go)
  - Any writable directory; it will be created if it doesn’t exist.

You can pass additional include search paths with `-I <path>` as needed.

---

## TL;DR example (Linux/macOS)

Assume:

- Third‑party include root: `../deps/occt/inc`
- The specific headers you want to wrap: everything under `../deps/occt/inc/Geom/*.h` and `../deps/occt/inc/TColStd/*.h`
- Output folder: `../build/gdext`
- You want the original headers included in the generated `.cpp` files

Run:

```bash
cd OpenCASCADE.gd

python -m godot_cpp_binding_generator.cli \
  --headers "../deps/occt/inc/Geom/*.h" "../deps/occt/inc/TColStd/*.h" \
  --include-rel-base "../deps/occt/inc" \
  --out-dir "../build/gdext" \
  --prefix ocgd \
  --include-originals \
  --std c++20 \
  --log-level INFO
```

Notes:

- `--include-rel-base` is inserted first in the `-I` search paths automatically. You can still add more `-I` paths via `-I /extra/include`.
- If libclang is not auto‑found, add `--libclang-path "/usr/lib/llvm-16/lib"` (or your version/path).
- You can pass any extra clang args via `--clang-args ...`, for example: `--clang-args -D__GNUC__=12 -DNDEBUG`.

---

## TL;DR example (Windows PowerShell)

```powershell
Set-Location OpenCASCADE.gd

python -m godot_cpp_binding_generator.cli `
  --headers "..\deps\occt\inc\Geom\*.h" "..\deps\occt\inc\TColStd\*.h" `
  --include-rel-base "..\deps\occt\inc" `
  --out-dir "..\build\gdext" `
  --prefix ocgd `
  --include-originals `
  --std c++20 `
  --libclang-path "C:\Program Files\LLVM\bin" `
  --log-level INFO
```

PowerShell uses backticks for line continuations. Adjust paths and `--libclang-path` for your LLVM installation.

---

## What `--include-rel-base` does

- Adds the directory first in the clang `-I` list, so that quoted/angle includes inside your headers resolve exactly as they do in your native build.
- When you use `--include-originals`, the generator includes each wrapped header in the emitted `.cpp` using a path relative to `--include-rel-base` when possible. This keeps generated includes clean and portable, e.g.:

```cpp
#include "Geom/Geom_TrimmedCurve.hxx"
```

If a header is outside the base, it falls back to an absolute or as‑passed path.

---

## Output

Given `--prefix ocgd`, you’ll get in `--out-dir`:

- `ocgd_utils.h` – small helper utilities
- `ocgd_register.h` and `ocgd_register.cpp` – registration boilerplate for Godot
- One pair per wrapped class:
  - `ocgd_<FlatName>.hpp`
  - `ocgd_<FlatName>.cpp`

“FlatName” is a sanitized, namespace‑collapsed version of the C++ fully qualified name (e.g., `Geom_Geom_TrimmedCurve` for `Geom::Geom_TrimmedCurve`).

---

## Common options

- `--headers / -H` one or more globs of headers to scan (recursive `**/*.h` supported).
- `-I <path>` extra include search paths (can repeat).
- `--include-rel-base <path>` base include root (added first to `-I`; also used for clean relative includes when `--include-originals` is enabled).
- `--out-dir / -o` output directory (default: `out`).
- `--prefix` prefix for generated files and classes (default: `ocgd`).
- `--include-originals` include the original headers in the generated `.cpp` files.
- `--std` C++ standard for libclang parse (default: `c++17`; e.g., `c++20`).
- `--clang-args ...` extra args passed directly to clang (e.g., `-DDEBUG -isystem /path`).
- `--libclang-path` directory containing the libclang shared library (if it isn’t auto‑discoverable).
- `--godot-base` Godot base class for wrappers, e.g., `Object` or `RefCounted` (default: `Object`).
- `--map` path to a JSON file with mapping overrides (type -> mapping).
- `--log-level` `CRITICAL|ERROR|WARNING|INFO|DEBUG` (default: `INFO`).

---

## Tips for large third‑party libraries

- Start small: point `--headers` to a focused subset (one module) to validate parsing and output.
- Add `-I` paths incrementally until libclang can parse with minimal diagnostics.
- If you see many template or smart pointer types (e.g., `Handle<T>`, `std::vector<T>`), consider providing a mapping JSON via `--map` to refine how those are represented.
- Use `--log-level DEBUG` to see which files are parsed and what is discovered/scanned.

---

## Troubleshooting

- “clang.cindex is required” or “Failed to create clang index”:
  - Install Python `clang` package: `pip install clang`.
  - Ensure libclang is installed and add `--libclang-path` pointing to the directory that contains `libclang.so` (Linux), `libclang.dylib` (macOS), or `libclang.dll` (Windows).

- “Failed to parse …”:
  - Add missing include search paths with `-I`.
  - Add macros/defines with `--clang-args -DMY_MACRO=1`.
  - Make sure `--std` matches what your library expects (e.g., `c++20`).

- “No classes discovered”:
  - Double‑check your `--headers` globs.
  - Try `--log-level DEBUG` and verify the reported “API candidate class” lines.
  - Ensure headers actually contain public class/struct declarations (not only forward declarations).

---

## Minimal example

```bash
cd OpenCASCADE.gd

python -m godot_cpp_binding_generator.cli \
  --headers "../vendor/lib/include/my_lib/**/*.h" \
  --include-rel-base "../vendor/lib/include" \
  --out-dir "../gen/gdext" \
  --prefix mylib \
  --include-originals \
  --std c++20
```

You can now add the emitted files from `../gen/gdext` to your Godot GDExtension build.

---

## FAQ

- Do I need to quote the `--headers` globs?
  - The CLI accepts both patterns and concrete paths. Shell expansion or passing the pattern is fine. If your shell expands the glob, it will just receive a long list of files.

- Can I run the CLI from anywhere?
  - Yes, as long as Python can import `godot_cpp_binding_generator`. The simplest is to `cd OpenCASCADE.gd` and run `python -m godot_cpp_binding_generator.cli`.

- Why are generated methods sometimes returning `Variant` or `void*`?
  - The generator prefers conservative mappings that compile even when types are complex or unknown. You can iteratively improve these via the mapping overrides file (`--map`) and by extending the type classification rules.

---