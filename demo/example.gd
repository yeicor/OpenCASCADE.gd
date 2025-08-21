@tool
extends EditorScript 


func _run() -> void:
	var t1 = Time.get_ticks_usec()
	var reader = ocgd_CADFileImporter.new()
	var ret = reader.analyze_file(ProjectSettings.globalize_path("res://example.stp"))
	print("analyze_file took " + str((Time.get_ticks_usec() - t1) / 1000.0) + "ms and returned " + str(ret))
	t1 = Time.get_ticks_usec()
	ret = reader.import_file_with_metadata(ProjectSettings.globalize_path("res://example.stp"))
	print("import_file took " + str((Time.get_ticks_usec() - t1) / 1000.0) + "ms and returned " + str(ret))
	for shape_ in ret["all_shapes"]:
		print("Processing shape " + str(shape_))
		var shape: ocgd_TopoDS_Shape = shape_
		var mde = ocgd_MeshDataExtractor.new()
		t1 = Time.get_ticks_usec()
		ret = mde.extract_mesh_data_with_purpose(shape, ocgd_MeshDataExtractor.MESH_PURPOSE_PRESENTATION)
		print("extract_mesh_data_with_purpose took " + str((Time.get_ticks_usec() - t1) / 1000.0) + "ms and returned " + str(ret))
		var exporter = ocgd_STLExporter.new()
		t1 = Time.get_ticks_usec()
		ret = exporter.can_export_shape(shape)
		print("can_export_shape took " + str((Time.get_ticks_usec() - t1) / 1000.0) + "ms and returned " + str(ret))
		t1 = Time.get_ticks_usec()
		ret = exporter.validate_shape_for_export(shape)
		print("validate_shape_for_export took " + str((Time.get_ticks_usec() - t1) / 1000.0) + "ms and returned " + str(ret))
		t1 = Time.get_ticks_usec()
		ret = exporter.export_to_bytes(shape, false)
		print("export_to_bytes took " + str((Time.get_ticks_usec() - t1) / 1000.0) + "ms and returned " + str(ret))
