extends Node


func _ready() -> void:
	var example = StepIgesBRepImporter.new()
	example.import(ProjectSettings.globalize_path("res://example.stp"))
