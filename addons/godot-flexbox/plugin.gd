# ############################################################################ #
# Copyright 2016-2023 citizenl <hi@citizenl.me>
# MIT License.
# ############################################################################ #

@tool
extends EditorPlugin
#
const Icon = preload("icon.svg")
const FlexContainer = preload("flex_container.gd")
const InspectorPlugin = preload("inspector/inspector_plugin.gd")
var _inspector_plugin
var _toolbar:EditorToolbar
var _interface

func _enter_tree():
	IconAssets.generate_icons()
	_inspector_plugin = InspectorPlugin.new()
	add_custom_type("FlexContainer", "Container", FlexContainer, Icon)
	add_inspector_plugin(_inspector_plugin)
	
	create_toolbar()


func _exit_tree():
	remove_inspector_plugin(_inspector_plugin)
	remove_custom_type("FlexContainer")
	remove_control_from_container(EditorPlugin.CONTAINER_CANVAS_EDITOR_MENU, _toolbar)
	_inspector_plugin = null
	_toolbar = null
	IconAssets.icons_map = null


func create_toolbar():
	_toolbar = EditorToolbar.new()
	_toolbar.plugin = self
	_toolbar.hide()
	add_control_to_container(EditorPlugin.CONTAINER_CANVAS_EDITOR_MENU, _toolbar)
