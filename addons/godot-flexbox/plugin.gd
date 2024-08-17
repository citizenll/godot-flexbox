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
	IconAssets.generate_icons(is_dark_theme())
	_inspector_plugin = InspectorPlugin.new()
	var	EDSCALE = get_editor_interface().get_editor_scale()
	var image = Icon.get_image()
	image.resize(20*EDSCALE, 20*EDSCALE,Image.INTERPOLATE_NEAREST)
	var icon = ImageTexture.new().create_from_image(image)
	add_custom_type("FlexContainer", "Container", FlexContainer, icon)
	add_inspector_plugin(_inspector_plugin)
	get_window().theme_changed.connect(_theme_changed)
	_interface = get_editor_interface()
	create_toolbar(EDSCALE)


func _exit_tree():
	remove_inspector_plugin(_inspector_plugin)
	remove_custom_type("FlexContainer")
	remove_control_from_container(EditorPlugin.CONTAINER_CANVAS_EDITOR_MENU, _toolbar)
	_inspector_plugin = null
	_toolbar = null


func create_toolbar(EDSCALE):
	_toolbar = EditorToolbar.new(EDSCALE)
	_toolbar.plugin = self
	_toolbar.undo_redo = get_undo_redo()
	_toolbar.hide()
	add_control_to_container(EditorPlugin.CONTAINER_CANVAS_EDITOR_MENU, _toolbar)


func is_dark_theme():
	var settings = get_editor_interface().get_editor_settings() as EditorSettings
	var AUTO_COLOR = 0;
	var LIGHT_COLOR = 2;
	var base_color = settings.get("interface/theme/base_color");
	var icon_font_color_setting = settings.get("interface/theme/icon_and_font_color");
	return (icon_font_color_setting == AUTO_COLOR && base_color.get_luminance() < 0.5) || icon_font_color_setting == LIGHT_COLOR;


func _theme_changed():
	IconAssets.generate_icons(is_dark_theme())
