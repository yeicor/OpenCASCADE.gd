@tool
extends EditorScript 


func _run() -> void:
	var t1 = Time.get_ticks_usec()
	var p1 = ocgd_gp_Pnt.new()
	p1.SetCoordXYZ(1.0, 2.0, 2.0)
	assert(abs(p1.Distance(ocgd_gp_Pnt.new()) - 3.0) < 0.00001, "Bad point distance")
	print("Point distance test ok in " + str((Time.get_ticks_usec() - t1) / 1000.0) + "ms")
	
