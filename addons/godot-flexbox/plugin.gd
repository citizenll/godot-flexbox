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
#
const DisplayFlexColumn = preload("assets/DisplayFlexColumn.svg")
const DisplayFlexRow = preload("assets/DisplayFlexRow.svg")
const ArrowReverseIcon = preload("assets/ArrowReverseIcon.svg")
#
const AlignItemsStartRowIcon = preload("assets/AlignItemsStartRowIcon.svg")
const AlignItemsCenterRowIcon = preload("assets/AlignItemsCenterRowIcon.svg")
const AlignItemsEndRowIcon = preload("assets/AlignItemsEndRowIcon.svg")
const AlignItemsStretchRowIcon = preload("assets/AlignItemsStretchRowIcon.svg")
const AlignItemsBaselineRowIcon = preload("assets/AlignItemsBaselineRowIcon.svg")
#
const JustifyContentStartColumnIcon = preload("assets/JustifyContentStartColumnIcon.svg")
const JustifyContentCenterColumnIcon = preload("assets/JustifyContentCenterColumnIcon.svg")
const JustifyContentEndColumnIcon = preload("assets/JustifyContentEndColumnIcon.svg")
const JustifyContentSpaceBetweenColumnIcon = preload("assets/JustifyContentSpaceBetweenColumnIcon.svg")
const JustifyContentSpaceAroundColumnIcon = preload("assets/JustifyContentSpaceAroundColumnIcon.svg")
const JustifyContentSpaceEvenlyColumnIcon = preload("assets/JustifyContentSpaceEvenlyColumnIcon.svg")
#

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
	var anchors_button: EditorPopupButton
	var containers_button: EditorPopupButton

	var container_h_picker
	var container_v_picker
	
	func _init():
		add_child(VSeparator.new())

		anchors_button = EditorPopupButton.new()
		anchors_button.set_tooltip_text("Presets for the direction values  of a Flexbox node.")
		add_child(anchors_button)
		#
		add_child(Space.new(12))
		
		var anchors_label = Label.new()
		anchors_label.text = "Flexbox Presets"
		anchors_button.get_popup_hbox().add_child(anchors_label)
		var flex_picker = FlexPresetPicker.new()
		flex_picker.set_h_size_flags(SIZE_SHRINK_CENTER);
		anchors_button.get_popup_hbox().add_child(flex_picker)
		flex_picker.anchors_preset_selected.connect(_anchors_preset_selected)


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

	
	func _anchors_preset_selected(p_category, p_preset):
		pass


	func _notification(what):
		match what: 
			NOTIFICATION_ENTER_TREE, NOTIFICATION_THEME_CHANGED:
				anchors_button.icon = Icon


class EditorPopupButton extends Button:
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


class EditorPresetPicker extends MarginContainer:
	var grid_separation = 0
	var preset_buttons = {}
	
	func _add_button(p_category, p_preset, b):
		if preset_buttons.get(p_category) == null:
			preset_buttons[p_category] = {}
		preset_buttons[p_category][p_preset] = b


	func _add_row_button(p_row, p_category, p_preset, p_name):
		var b = Button.new()
		b.toggle_mode = true
		b.set_custom_minimum_size(Vector2(36, 36))
		b.set_icon_alignment(HORIZONTAL_ALIGNMENT_CENTER)
		b.set_tooltip_text(p_name)
		b.set_flat(true)
		p_row.add_child(b)
		b.pressed.connect(_preset_button_pressed.bind(p_category, p_preset))
		_add_button(p_category, p_preset, b)
		
		
	func _add_text_button(p_row, p_category, p_preset, p_name):
		var b = Button.new()
		b.toggle_mode = true
		b.set("theme_override_font_sizes/font_size",12)
		b.set_custom_minimum_size(Vector2(36, 36))
		b.set_text(p_name)
		p_row.add_child(b)
		b.pressed.connect(_preset_button_pressed.bind(p_category, p_preset))
		_add_button(p_category, p_preset, b)


	func _add_separator(p_box, p_separator):
		p_separator.add_theme_constant_override("separation", grid_separation)
		p_separator.set_custom_minimum_size(Vector2(1, 1))
		p_box.add_child(p_separator)
	
	func _preset_button_pressed(p_category, p_preset):
		print("catgory: ", p_category, "preset: ", p_preset)


class FlexPresetPicker extends EditorPresetPicker:
	signal anchors_preset_selected

	enum DirectionPreset {
		Column,ColumnReverse,Row,RowReverse,Reverse
	}

	enum WrapPreset {
		NoWrap,Wrap,WrapReverse
	}

	enum AlignContentPreset {
		Auto,FlexStart,Center,FlexEnd,Stretch,Baseline,SpaceBetween,SpaceAround
	}

	enum JustifyPreset {
		FlexStart,Center,FlexEnd,SpaceBetween,SpaceAround,SpaceEvenly
	}

	enum AlignItemsPreset {
		Auto,FlexStart,Center,FlexEnd,Stretch,Baseline,SpaceBetween,SpaceAround
	}

	const Category = {
		Direction = "flex_direction",
		Align = "align_items",
		Justify = "justify_content",
		AlignContent = "align_content",
	}

	const Reverse = "reverse"

	const LABEL_WIDTH = 70

	func _init():
		var main_vb = VBoxContainer.new()
		main_vb.add_theme_constant_override("separation", grid_separation)
		add_child(main_vb)

		#direction
		var dir_row = HBoxContainer.new()
		dir_row.set_alignment(HBoxContainer.ALIGNMENT_BEGIN)
		dir_row.add_theme_constant_override("separation", grid_separation)
		main_vb.add_child(dir_row)
		
		var direction_label = Label.new()
		direction_label.set_text("Direction")
		direction_label.custom_minimum_size.x = LABEL_WIDTH
		dir_row.add_child(direction_label)
		_add_row_button(dir_row, Category.Direction, DirectionPreset.Row, " Row ")
		_add_row_button(dir_row, Category.Direction, DirectionPreset.Column, " Column ")
		_add_separator(dir_row, VSeparator.new());
		_add_row_button(dir_row, Reverse, DirectionPreset.Reverse, "Reverse");
		
		#align
		var align_row = HBoxContainer.new()
		align_row.set_alignment(HBoxContainer.ALIGNMENT_BEGIN)
		align_row.add_theme_constant_override("separation", grid_separation)
		main_vb.add_child(align_row)

		var align_label = Label.new()
		align_label.custom_minimum_size.x = LABEL_WIDTH
		align_label.set_text("Align")
		align_row.add_child(align_label)
		_add_row_button(align_row, Category.Align, AlignItemsPreset.FlexStart, "Start");
		_add_row_button(align_row, Category.Align, AlignItemsPreset.Center, "Center");
		_add_row_button(align_row, Category.Align, AlignItemsPreset.FlexEnd, "End");
		_add_row_button(align_row, Category.Align, AlignItemsPreset.Stretch, "Stretch");
		_add_row_button(align_row, Category.Align, AlignItemsPreset.Baseline, "Baseline");

		#justify
		var justify_row = HBoxContainer.new()
		justify_row.set_alignment(HBoxContainer.ALIGNMENT_BEGIN)
		justify_row.add_theme_constant_override("separation", grid_separation)
		main_vb.add_child(justify_row)

		var justify_label = Label.new()
		justify_label.custom_minimum_size.x = LABEL_WIDTH
		justify_label.set_text("Justify")
		justify_row.add_child(justify_label)
		_add_row_button(justify_row, Category.Justify, JustifyPreset.FlexStart, "Start");
		_add_row_button(justify_row, Category.Justify, JustifyPreset.Center, "Center");
		_add_row_button(justify_row, Category.Justify, JustifyPreset.FlexEnd, "End");
		_add_row_button(justify_row, Category.Justify, JustifyPreset.SpaceBetween, "SpaceBetween");
		_add_row_button(justify_row, Category.Justify, JustifyPreset.SpaceAround, "SpaceAround");
		_add_row_button(justify_row, Category.Justify, JustifyPreset.SpaceEvenly, "SpaceEvenly");

		#
		_add_separator(main_vb, HSeparator.new());
		var content_row = HBoxContainer.new()
		content_row.set_alignment(HBoxContainer.ALIGNMENT_BEGIN)
		content_row.add_theme_constant_override("separation", grid_separation)
		main_vb.add_child(content_row)

		var content_label = Label.new()
		content_label.custom_minimum_size.x = LABEL_WIDTH
		content_label.set_text("Content")
		content_row.add_child(content_label)
		_add_row_button(content_row, Category.AlignContent, AlignContentPreset.FlexStart, "Start");
		_add_row_button(content_row, Category.AlignContent, AlignContentPreset.Center, "Center");
		_add_row_button(content_row, Category.AlignContent, AlignContentPreset.FlexEnd, "End");
		_add_row_button(content_row, Category.AlignContent, AlignContentPreset.Stretch, "Stretch");
		_add_row_button(content_row, Category.AlignContent, AlignContentPreset.Baseline, "Baseline");
		_add_row_button(content_row, Category.AlignContent, AlignContentPreset.SpaceBetween, "SpaceBetween");
		_add_row_button(content_row, Category.AlignContent, AlignContentPreset.SpaceAround, "SpaceAround");



	func _preset_button_pressed(p_category, p_preset):
		anchors_preset_selected.emit(p_category, p_preset)
		if p_category == Reverse:
			return
		for b in preset_buttons[p_category].values():
			b.button_pressed = (b == preset_buttons[p_category][p_preset])
	

	func _notification(what):
		match what:
			NOTIFICATION_ENTER_TREE,NOTIFICATION_THEME_CHANGED:
				#
				preset_buttons[Category.Direction][DirectionPreset.Column].icon = DisplayFlexColumn
				preset_buttons[Category.Direction][DirectionPreset.Row].icon = DisplayFlexRow
				preset_buttons[Reverse][DirectionPreset.Reverse].icon = ArrowReverseIcon
				#
				preset_buttons[Category.Align][AlignItemsPreset.FlexStart].icon = AlignItemsStartRowIcon
				preset_buttons[Category.Align][AlignItemsPreset.Center].icon = AlignItemsCenterRowIcon
				preset_buttons[Category.Align][AlignItemsPreset.FlexEnd].icon = AlignItemsEndRowIcon
				preset_buttons[Category.Align][AlignItemsPreset.Stretch].icon = AlignItemsStretchRowIcon
				preset_buttons[Category.Align][AlignItemsPreset.Baseline].icon = AlignItemsBaselineRowIcon

				#
				preset_buttons[Category.Justify][JustifyPreset.FlexStart].icon = JustifyContentStartColumnIcon
				preset_buttons[Category.Justify][JustifyPreset.Center].icon = JustifyContentCenterColumnIcon
				preset_buttons[Category.Justify][JustifyPreset.FlexEnd].icon = JustifyContentEndColumnIcon
				preset_buttons[Category.Justify][JustifyPreset.SpaceBetween].icon = JustifyContentSpaceBetweenColumnIcon
				preset_buttons[Category.Justify][JustifyPreset.SpaceAround].icon = JustifyContentSpaceAroundColumnIcon
				preset_buttons[Category.Justify][JustifyPreset.SpaceEvenly].icon = JustifyContentSpaceEvenlyColumnIcon
				#

				preset_buttons[Category.AlignContent][AlignContentPreset.FlexStart].icon = JustifyContentStartColumnIcon
