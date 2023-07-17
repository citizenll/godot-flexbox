# ############################################################################ #
# Copyright 2016-2023 citizenl <hi@citizenl.me>
# MIT License.
# ############################################################################ #

@tool
extends EditorPlugin

const Icon = preload("icon.svg")
const FlexContainer = preload("flex_container.gd")
const InspectorPlugin = preload("inspector/inspector_plugin.gd")
var _inspector_plugin
var _toolbar:EditorToolbar
var _interface

func _enter_tree():
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


func create_toolbar():
	_toolbar = EditorToolbar.new()
	_toolbar.plugin = self
	_toolbar.hide()
	add_control_to_container(EditorPlugin.CONTAINER_CANVAS_EDITOR_MENU, _toolbar)



class EditorToolbar extends HBoxContainer:
	var plugin:EditorPlugin
	var anchors_button: FlexboxEditorPopupButton
	var containers_button: FlexboxEditorPopupButton

	var container_h_picker
	var container_v_picker
	
	func _init():
		add_child(VSeparator.new())

		anchors_button = FlexboxEditorPopupButton.new()
		anchors_button.set_tooltip_text("Presets for the direction values  of a Flexbox node.")
		add_child(anchors_button)
		#
		add_child(Space.new(12))
		
		var anchors_label = Label.new()
		anchors_label.text = "Directions"
		anchors_button.get_popup_hbox().add_child(anchors_label)

	func _ready():
		var selection = plugin.get_editor_interface().get_selection()
		selection.selection_changed.connect(_selection_changed.bind(selection))

	func _selection_changed(selection:EditorSelection):
		var has_flexcontainers = false
		var nodes = selection.get_selected_nodes()
		for node in nodes:
			if node is FlexContainer:
				has_flexcontainers = true
				break
		set_visible(has_flexcontainers)


	func _notification(what):
		match what: 
			NOTIFICATION_ENTER_TREE, NOTIFICATION_THEME_CHANGED:
				anchors_button.icon = Icon


class FlexboxEditorPopupButton extends Button:
	var arrow_icon:Texture2D
	var popup_panel:PopupPanel
	var popup_vbox:VBoxContainer
	
	func _init():
		flat = true
		toggle_mode = true
		focus_mode = Control.FOCUS_NONE

		popup_panel = PopupPanel.new()
		popup_panel.theme_type_variation = "ControlEditorPopupPanel"
		add_child(popup_panel)
		popup_panel.about_to_popup.connect(_popup_visibility_changed.bind(true))
		popup_panel.popup_hide.connect(_popup_visibility_changed.bind(false))
		
		popup_vbox = VBoxContainer.new()
		popup_panel.add_child(popup_vbox)
	
	func _toggled(p_pressed):
		if !p_pressed:
			return
		var size = get_size() * get_viewport().get_canvas_transform().get_scale()
		popup_panel.size = Vector2(size.x, 0)
		var gp = get_screen_position()
		gp.y+= size.y
		popup_panel.position = gp
		popup_panel.popup()
	
	func _popup_visibility_changed(p_visible):
		button_pressed = p_visible

	func _notification(what):
		match what:
			NOTIFICATION_ENTER_TREE, NOTIFICATION_THEME_CHANGED:
				arrow_icon = get_theme_icon("select_arrow", "Tree");
			NOTIFICATION_DRAW:
				if is_instance_valid(arrow_icon):
					var arrow_pos = Vector2(26,0)
					arrow_pos.y = get_size().y / 2 - arrow_icon.get_height() / 2;
					draw_texture(arrow_icon, arrow_pos);
			NOTIFICATION_VISIBILITY_CHANGED:
				if is_visible_in_tree():
					popup_panel.hide()
	
	func get_popup_hbox():
		return popup_vbox


class Space extends Control:
	var ms = Vector2(3, 0)
	func _init(x_space):
		ms.x = x_space
	func _get_minimum_size():
		return ms
