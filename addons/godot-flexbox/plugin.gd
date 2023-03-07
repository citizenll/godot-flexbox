# ############################################################################ #
# Copyright 2016-2023 citizenl <hi@citizenl.me>
# MIT License.
# ############################################################################ #

@tool
extends EditorPlugin

const Icon = preload("icon.svg")
const FlexContainer = preload("flex_container.gd")
# const InspectorPlugin = preload("inspector/inspector_plugin.gd")
var _inspector_plugin


func _enter_tree():
	_get_theme()
	# _inspector_plugin = InspectorPlugin.new()
	add_custom_type("FlexContainer", "Container", FlexContainer, Icon)
	# add_inspector_plugin(_inspector_plugin)


func _exit_tree():
	# remove_inspector_plugin(_inspector_plugin)
	remove_custom_type("FlexContainer")
	_inspector_plugin = null



func _get_theme():
	var editor_settings = get_editor_interface().get_editor_settings()
	var my_theme = editor_settings.get_setting("interface/theme/preset")
	#print("my_theme:", my_theme)
	
