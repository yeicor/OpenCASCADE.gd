# Auto-Normal Integration Summary

This document summarizes the comprehensive auto-normal integration that has been added to OpenCASCADE.gd, providing seamless surface normal computation for any shape with a simple `compute_normals` parameter.

## What Was Implemented

### 1. Enhanced Normal Computation Engine (`ocgd_EnhancedNormals`)

**Purpose**: The missing implementation that was being referenced but didn't exist
**Location**: `src/ai_bindings/ocgd_EnhancedNormals.{hxx,cpp}`

**Key Methods**:
- `compute_and_store_normals(face, triangulation)` - Automatic normal computation for triangulations
- `extract_normals_as_vector3_array(face, triangulation, location)` - Extract normals with transformations
- `compute_surface_normal_at_uv(face, u, v)` - Direct surface normal at UV coordinates
- `get_normal_computation_info(face, triangulation)` - Diagnostic information

**Algorithm Hierarchy**:
1. **Surface-based computation** - Uses `BRepLProp_SLProps` with UV coordinates for mathematically accurate normals
2. **Triangle-based computation** - Uses `Poly_Connect` for proper normal accumulation as fallback
3. **Error handling** - Graceful fallbacks with sensible defaults

### 2. Direct Surface Normal API (`ocgd_SurfaceUtils`)

**Purpose**: Direct surface normal computation similar to OpenCASCADE's `Surface::Normal(surf, u, v)`
**Location**: `src/ai_bindings/ocgd_SurfaceUtils.{hxx,cpp}`

**Key Methods**:
- `compute_surface_normal(face, u, v)` → Vector3 normal
- `compute_surface_normal_axis(face, u, v)` → {point, direction, valid} (like gp_Ax1)
- `get_uv_bounds(face)` → {u_min, u_max, v_min, v_max}
- `compute_surface_point(face, u, v)` → Vector3 point
- `compute_surface_curvature(face, u, v)` → curvature info
- `sample_surface_normals(face, u_array, v_array)` → Vector3 array

**Example Usage** (equivalent to C++ `gp_Ax1 normal = Surface::Normal(surf, 1.0, -4.5)`):
```gdscript
var surface_utils = ocgd_SurfaceUtils.new()
var normal_axis = surface_utils.compute_surface_normal_axis(face, 1.0, -4.5)
# Returns: {"point": Vector3, "direction": Vector3, "valid": bool}
```

### 3. Auto-Normal Integration in Core Classes

#### A. BRepMesh_IncrementalMesh

**New Parameter**: `compute_normals` (default: true)
**Updated Method**: `init_with_shape(shape, linear_deflection, is_relative, angular_deflection, is_in_parallel, compute_normals)`

**Behavior**:
- Stores shape reference for normal computation
- Automatically computes normals after `perform()` if enabled
- New methods: `compute_normals()`, `get_compute_normals()`, `set_compute_normals()`

**Usage**:
```gdscript
var mesh = ocgd_BRepMesh_IncrementalMesh.new()
mesh.init_with_shape(shape, 0.1, false, 0.5, false, true)  # auto-normals enabled
mesh.perform()  # Automatically computes normals
```

#### B. MeshDataExtractor

**New Parameter**: `compute_normals` (default: true) for all extraction methods
**Updated Methods**:
- `extract_mesh_data(shape, compute_normals)`
- `extract_face_data(face, compute_normals)`
- `extract_per_face_data(shape, compute_normals)`
- `extract_mesh_data_with_purpose(shape, purpose, compute_normals)`

**Behavior**:
- Calls `ensure_triangulation()` with normal computation before extraction
- Integrates seamlessly with existing workflow

**Usage**:
```gdscript
var extractor = ocgd_MeshDataExtractor.new()
var mesh_data = extractor.extract_mesh_data(shape)  # normals auto-computed
var mesh_data_fast = extractor.extract_mesh_data(shape, false)  # skip normals
```

#### C. AdvancedMeshExporter

**New Parameter**: `compute_normals` (default: true)
**Updated Methods**:
- `extract_mesh_data(shape, compute_normals)`
- `triangulate_shape(shape, compute_normals)`

**Behavior**:
- Integrates normal computation with existing export pipeline
- Respects `_export_normals` setting combined with `compute_normals` parameter

**Usage**:
```gdscript
var exporter = ocgd_AdvancedMeshExporter.new()
exporter.set_export_normals(true)
var mesh_data = exporter.extract_mesh_data(shape)  # auto-normals enabled
```

## Key Features

### 1. Automatic Quality Selection
The system automatically chooses the best normal computation method:
1. **Pre-computed normals** - Uses existing if available
2. **Surface-based** - Uses UV coordinates and surface properties (highest quality)
3. **Triangle-based** - Uses triangle connectivity (reliable fallback)

### 2. UV Parameter Support
All surface normal computation uses UV coordinates, enabling:
- Mathematically accurate normals from surface geometry
- Support for complex surface types (planes, cylinders, spheres, NURBS, etc.)
- Proper normal orientation handling

### 3. Seamless Integration
- **Default enabled**: All methods have `compute_normals = true` by default
- **No breaking changes**: Existing code continues to work
- **Optional disable**: Set `compute_normals = false` for faster processing when normals aren't needed

### 4. Error Resilience
- Graceful fallbacks between computation methods
- Sensible default normals for edge cases
- Comprehensive error logging and diagnostic information

## Usage Examples

### Simple Cases (Recommended)
```gdscript
# All of these automatically compute normals:
var mesh_data = extractor.extract_mesh_data(shape)
var face_data = extractor.extract_face_data(face)
var mesh_data = exporter.extract_mesh_data(shape)

# Disable for performance when normals not needed:
var vertices_only = extractor.extract_mesh_data(shape, false)
```

### Advanced Surface Normal Queries
```gdscript
var surface_utils = ocgd_SurfaceUtils.new()

# Get UV bounds
var bounds = surface_utils.get_uv_bounds(face)

# Compute normal at specific UV
var normal = surface_utils.compute_surface_normal(face, 1.0, -4.5)

# Get surface properties
var curvature = surface_utils.compute_surface_curvature(face, 1.0, -4.5)
var surface_type = surface_utils.get_surface_type(face)  # "plane", "cylinder", etc.
```

### Manual Control
```gdscript
# Manual triangulation with normals
var mesh = ocgd_BRepMesh_IncrementalMesh.new()
mesh.init_with_shape(shape, 0.1, false, 0.5, false, false)  # no auto-normals
mesh.perform()
mesh.compute_normals()  # compute manually

# Configure tolerance
ocgd_EnhancedNormals.set_tolerance(1e-8)
surface_utils.set_tolerance(1e-8)
```

## Performance Considerations

### When to Enable Normals (Default)
- Creating meshes for rendering
- Export to formats that support normals
- Surface analysis and visualization
- Quality mesh generation

### When to Disable Normals
- Pure geometric analysis (no rendering)
- Volume/area calculations
- Boolean operations on shapes
- Performance-critical applications where normals aren't needed

## Technical Implementation

### OpenCASCADE Integration
- Uses `BRepLProp_SLProps` for surface-based normal computation
- Uses `BRepAdaptor_Surface` for surface property access
- Uses `Poly_Connect` for triangle connectivity in fallback mode
- Handles `TopLoc_Location` transformations correctly

### Memory Management
- Normals are stored directly in OpenCASCADE triangulation structures
- No additional memory overhead beyond standard triangulation
- Computed once and reused across multiple extractions

### Thread Safety
- All static methods in `ocgd_EnhancedNormals` are thread-safe
- `ocgd_SurfaceUtils` instances are independent and thread-safe
- OpenCASCADE triangulation access follows OCCT thread safety guidelines

## Migration Guide

### Existing Code
No changes required - all existing code continues to work with enhanced normal quality.

### New Code
Simply use the default parameters:
```gdscript
# Old approach - still works
var mesh_data = extractor.extract_mesh_data(shape)

# New explicit approach - same result
var mesh_data = extractor.extract_mesh_data(shape, true)

# Performance optimization when normals not needed
var fast_data = extractor.extract_mesh_data(shape, false)
```

## Files Added/Modified

### New Files
- `src/ai_bindings/ocgd_EnhancedNormals.hxx`
- `src/ai_bindings/ocgd_EnhancedNormals.cpp`
- `src/ai_bindings/ocgd_SurfaceUtils.hxx`
- `src/ai_bindings/ocgd_SurfaceUtils.cpp`
- `examples/surface_normals_example.md`

### Modified Files
- `src/ai_bindings/ocgd_BRepMesh_IncrementalMesh.{hxx,cpp}` - Added auto-normal computation
- `src/ai_bindings/ocgd_MeshDataExtractor.{hxx,cpp}` - Added compute_normals parameter
- `src/ai_bindings/ocgd_AdvancedMeshExporter.{hxx,cpp}` - Added compute_normals parameter
- `src/ai_bindings/register_types.cpp` - Added new class registrations

## Triangle Winding Order Fix

### Problem Identified
Some faces appeared backwards when exported due to **duplicate orientation correction**:
1. Triangle winding order was being reversed for `TopAbs_REVERSED` faces ✓
2. Normal vectors were also being flipped for `TopAbs_REVERSED` faces ✗ (duplicate!)

This "double negative" effect caused the corrections to cancel each other out, resulting in backwards triangles.

### Root Cause Analysis
- **AdvancedMeshExporter**: Already correctly reversed triangle indices (n1, n3, n2) for `TopAbs_REVERSED` faces
- **MeshDataExtractor**: Was missing triangle winding order handling entirely  
- **EnhancedNormals**: Was also flipping normal vectors for `TopAbs_REVERSED` faces (redundant)
- **SurfaceUtils**: Was also flipping normal vectors for `TopAbs_REVERSED` faces (redundant)

### Solution Implemented
**Cleaned up duplicate code and unified the approach:**

1. **Triangle Winding Order** (Primary Fix):
   - AdvancedMeshExporter: ✓ Already correct - kept existing logic
   - MeshDataExtractor: ✓ Added missing face orientation handling
   - Both now consistently reverse triangle indices for `TopAbs_REVERSED` faces

2. **Normal Computation** (Simplified):
   - EnhancedNormals: ✓ Removed duplicate normal flipping (default: `false`)
   - SurfaceUtils: ✓ Uses same configurable setting
   - Both now use raw OpenCASCADE normals by default

3. **Consistent Behavior**:
   - All extraction methods now handle face orientation the same way
   - Triangle winding order is the **primary** mechanism for orientation
   - Normal flipping is **optional override** only (default: disabled)

### Code Changes Made

**MeshDataExtractor.cpp**:
```cpp
// Added face orientation check in convert_triangulation_to_dict
Standard_Boolean is_reversed = (face != nullptr && face->Orientation() == TopAbs_REVERSED);

if (is_reversed) {
    triangles.append(n1 - 1);
    triangles.append(n3 - 1);  // Swapped n2/n3
    triangles.append(n2 - 1);
} else {
    triangles.append(n1 - 1);
    triangles.append(n2 - 1);
    triangles.append(n3 - 1);
}
```

**EnhancedNormals.cpp**:
```cpp
// Changed default from true to false
bool ocgd_EnhancedNormals::s_flip_normals_for_reversed = false;

// Added clear documentation
// Triangle winding order already handles orientation, only flip if explicitly requested
```

### Benefits of the Fix
- **Eliminates duplicate corrections** that were cancelling each other out
- **Consistent behavior** across all mesh extraction methods
- **Proper triangle winding** ensures correct face culling and lighting
- **Configurable override** available if needed for edge cases
- **Clear debug output** shows orientation handling

## Backwards Normal Fix

### Previous Issue (Now Resolved)
Triangle winding order and normal computation were applying corrections independently, causing some faces to appear backwards.

### Current Behavior (Fixed)
- **Triangle winding order**: Primary mechanism for handling face orientation
- **Normal computation**: Uses raw OpenCASCADE values (no duplicate flipping)
- **Override available**: `set_flip_normals_for_reversed(true)` if needed (default: `false`)

### Debug Information
```
EnhancedNormals: Processing face with orientation: REVERSED (triangle winding handles orientation, normal_flip=false)
```

### Usage (Normally Not Needed)
```gdscript
# Check current setting (should be false)
var setting = ocgd_SurfaceUtils.get_flip_normals_for_reversed()

# Override only if absolutely necessary
ocgd_SurfaceUtils.set_flip_normals_for_reversed(true)
```

## Conclusion

This integration provides a complete, production-ready surface normal computation system that:

1. **Just works** - Normals are computed automatically with sensible defaults
2. **High quality** - Uses surface-based computation when possible
3. **Robust** - Graceful fallbacks ensure reliable operation
4. **Flexible** - Can be disabled for performance or controlled manually
5. **Compatible** - No breaking changes to existing code
6. **Correctly oriented** - Triangle winding order ensures proper face orientation
7. **Unified behavior** - All extraction methods handle orientation consistently

The API follows OpenCASCADE patterns while providing Godot-friendly interfaces, making high-quality surface normal computation accessible with minimal setup and correct triangle orientation out of the box.