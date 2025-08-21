# Surface Normals API Example

This document demonstrates how to use the new surface normal computation API in OpenCASCADE.gd.

## Overview

The surface normal API provides automatic normal computation with a simple `compute_normals` parameter for all meshing and extraction operations.

## Simple Auto-Normal Computation (Recommended)

All meshing and extraction methods now support automatic normal computation with a simple boolean parameter:

```gdscript
# Create a shape (example: box)
var primitive_shapes = ocgd_PrimitiveShapes.new()
var box = primitive_shapes.create_box(Vector3(10, 5, 2))

# Method 1: Using BRepMesh_IncrementalMesh with auto-normals
var mesh = ocgd_BRepMesh_IncrementalMesh.new()
mesh.init_with_shape(box, 0.1, false, 0.5, false, true)  # compute_normals = true (default)
mesh.perform()  # Automatically computes normals after meshing

# Method 2: Using MeshDataExtractor with auto-normals
var extractor = ocgd_MeshDataExtractor.new()
var mesh_data = extractor.extract_mesh_data(box, true)  # compute_normals = true (default)
print("Normals computed: ", mesh_data.get("normals", []).size())

# Method 3: Using AdvancedMeshExporter with auto-normals
var exporter = ocgd_AdvancedMeshExporter.new()
exporter.set_export_normals(true)
var mesh_data = exporter.extract_mesh_data(box, true)  # compute_normals = true (default)
```

## Normals are Enabled by Default

All methods have `compute_normals = true` as the default parameter, so you can simply:

```gdscript
# These all automatically compute normals:
var mesh_data = extractor.extract_mesh_data(box)
var face_data = extractor.extract_face_data(face)
var per_face_data = extractor.extract_per_face_data(box)

# Disable normals if not needed (faster):
var mesh_data_no_normals = extractor.extract_mesh_data(box, false)
```

## Surface Utils (Direct API)

For direct surface normal computation at specific UV coordinates:

```gdscript
# Create a surface utility instance
var surface_utils = ocgd_SurfaceUtils.new()

# Get a face from your shape
var explorer = ocgd_TopExp_Explorer.new()
explorer.init(box, "FACE")
var face = explorer.current()

# Get UV bounds for the face
var uv_bounds = surface_utils.get_uv_bounds(face)
print("UV bounds: ", uv_bounds)

# Compute surface normal at specific UV coordinates
var u = 1.0
var v = -4.5
var normal = surface_utils.compute_surface_normal(face, u, v)
print("Surface normal at (", u, ", ", v, "): ", normal)

# Get surface normal axis (point + direction)
var normal_axis = surface_utils.compute_surface_normal_axis(face, u, v)
if normal_axis.get("valid", false):
    print("Point: ", normal_axis["point"])
    print("Direction: ", normal_axis["direction"])

# Check if normal is well-defined at coordinates
var is_defined = surface_utils.is_normal_defined(face, u, v)
print("Normal defined: ", is_defined)
```

## Advanced Usage

### Sampling Multiple Normals

```gdscript
# Sample normals at multiple UV coordinates
var u_samples = [0.0, 0.5, 1.0]
var v_samples = [0.0, 0.5, 1.0]
var normals = surface_utils.sample_surface_normals(face, u_samples, v_samples)

for i in range(normals.size()):
    print("Normal at (", u_samples[i], ", ", v_samples[i], "): ", normals[i])
```

### Surface Properties

```gdscript
# Get surface type
var surface_type = surface_utils.get_surface_type(face)
print("Surface type: ", surface_type)  # e.g., "plane", "cylinder", "sphere"

# Compute curvature information
var curvature = surface_utils.compute_surface_curvature(face, u, v)
if curvature.get("valid", false):
    print("Gaussian curvature: ", curvature.get("gaussian_curvature", 0))
    print("Mean curvature: ", curvature.get("mean_curvature", 0))
    print("Min curvature: ", curvature.get("min_curvature", 0))
    print("Max curvature: ", curvature.get("max_curvature", 0))
```

### Tolerance Configuration

```gdscript
# Set tolerance for surface computations
surface_utils.set_tolerance(1e-8)
var tolerance = surface_utils.get_tolerance()
print("Current tolerance: ", tolerance)

# Also configure enhanced normals tolerance
ocgd_EnhancedNormals.set_tolerance(1e-8)
```

## Surface Normal API Similar to OpenCASCADE

The API follows a pattern similar to OpenCASCADE's Surface::Normal functionality:

```cpp
// OpenCASCADE C++ equivalent:
// gp_Ax1 normal = Surface::Normal(surf, 1.0 /* u */, -4.5 /* v */);

// OpenCASCADE.gd GDScript equivalent:
var normal_axis = surface_utils.compute_surface_normal_axis(face, 1.0, -4.5)
# Returns: {"point": Vector3, "direction": Vector3, "valid": bool}
```

## Key Features

### High-Quality Normals
- **Surface-based computation**: Uses underlying surface geometry and UV coordinates for mathematically accurate results
- **Triangle-based fallback**: Proper connectivity using `Poly_Connect` for smooth normal accumulation
- **Automatic method selection**: Chooses best computation method based on available data

### UV Parameter Support
- **UV bounds querying**: Get valid parameter ranges for surfaces
- **UV-based normal computation**: Compute normals at specific parametric coordinates
- **Surface point evaluation**: Get 3D points from UV parameters

### Error Handling
- **Graceful fallbacks**: Automatically falls back to triangle-based computation when surface computation fails
- **Validation checks**: Verifies normal definition before computation
- **Default values**: Provides sensible defaults for edge cases

## Automatic Normal Computation Features

The `compute_normals` parameter enables intelligent normal computation that:

### Quality Hierarchy
1. **Pre-computed normals** - Uses existing normals if already present
2. **Surface-based computation** - Uses UV coordinates and surface geometry for highest quality
3. **Triangle-based computation** - Reliable fallback using triangle connectivity

### Integration Points
- **BRepMesh_IncrementalMesh**: Computes normals automatically after meshing
- **MeshDataExtractor**: Ensures normals before extraction
- **AdvancedMeshExporter**: Integrates with export pipeline

### Performance Benefits
- Set `compute_normals = false` to skip normal computation for faster processing
- Normals are computed only once and stored in triangulation
- Automatic fallback ensures robust operation

This ensures you always get the best possible normal quality for your specific geometry with minimal setup.

## Troubleshooting Backwards Triangles/Normals

If some faces appear backwards when exported, this has been fixed by ensuring consistent triangle winding order and normal computation.

### How the Fix Works

The system now properly handles OpenCASCADE face orientation in **two coordinated ways**:

1. **Triangle Winding Order**: For `TopAbs_REVERSED` faces, triangle indices are reordered (n1, n3, n2 instead of n1, n2, n3)
2. **Normal Computation**: Uses raw OpenCASCADE normals without additional flipping (since triangle winding handles orientation)

### Previous Duplicate Issue (Now Fixed)

Previously, the system was applying orientation correction **twice**:
- Triangle winding was reversed for `TopAbs_REVERSED` faces ✓
- Normal vectors were also flipped for `TopAbs_REVERSED` faces ✗ (duplicate)

This caused a "double negative" effect where corrections cancelled each other out.

### Current Behavior (Fixed)

Now the system uses **triangle winding order only** to handle face orientation:
- `TopAbs_FORWARD` faces: Standard triangle order (n1, n2, n3) with raw normals
- `TopAbs_REVERSED` faces: Reversed triangle order (n1, n3, n2) with raw normals

### Override Control (If Needed)

In rare cases where you need to override the behavior:

```gdscript
# Check current setting (should be false for correct behavior)
var current_setting = ocgd_SurfaceUtils.get_flip_normals_for_reversed()
print("Normal flipping: ", current_setting)  # Should print "false"

# Override if absolutely necessary (not recommended)
ocgd_SurfaceUtils.set_flip_normals_for_reversed(true)  # Forces additional normal flipping
```

### Debug Information

The system prints helpful debug information:
```
EnhancedNormals: Processing face with orientation: REVERSED (triangle winding handles orientation, normal_flip=false)
EnhancedNormals: Processing face with orientation: FORWARD (triangle winding handles orientation, normal_flip=false)
```

**Note**: With the fix, all faces should render correctly with the default settings. Manual overrides should not be necessary.