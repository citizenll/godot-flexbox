class_name EditorToolbar
extends HBoxContainer

const Icon = preload("icon.svg")

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

var plugin:EditorPlugin
var presets_button: EditorPopupButton

var container_h_picker
var container_v_picker

var presets = {}
var _current_node:FlexContainer
var _selection:EditorSelection
var undo_redo:EditorUndoRedoManager
var flex_picker:FlexPresetPicker
var EDSCALE = 1

func _init(p_editor_scale):
	EDSCALE = p_editor_scale
	presets_button = EditorPopupButton.new(EDSCALE)
	presets_button.set_tooltip_text("Presets for the direction values  of a Flexbox node.")
	add_child(presets_button)
	#
	var presets_label = Label.new()
	presets_label.text = "Flexbox Presets"
	presets_button.get_popup_hbox().add_child(presets_label)
	#set default direction
	presets["flex_direction"] = 2


func _ready():
	flex_picker = FlexPresetPicker.new(EDSCALE)
	flex_picker.set_h_size_flags(SIZE_SHRINK_CENTER);
	presets_button.get_popup_hbox().add_child(flex_picker)
	flex_picker.flexbox_preset_selected.connect(_flexbox_preset_selected)
	
	_selection = plugin.get_editor_interface().get_selection()
	_selection.selection_changed.connect(_selection_changed)
	_selection_changed()


func _exit_tree():
	_selection.selection_changed.disconnect(_selection_changed)


func _selection_changed():
	var nodes = _selection.get_selected_nodes()
	if nodes.size()<=0:
		_current_node = null
		return
	
	var node = nodes[0]
	if node is FlexContainer:
		_current_node =node
		set_visible(true)
		flex_picker.map_state(_current_node.state)
	else:
		_current_node = null
		set_visible(false)


func _flexbox_preset_selected(p_category, p_preset, p_state):
	var old_preset = presets.get(p_category, null)
	#flex direction reverse
	if p_category == "reverse":
		p_category = "flex_direction"
		old_preset = presets.get(p_category, null)
		if p_state:
			var is_row = old_preset == DirectionPreset.Row or old_preset == DirectionPreset.RowReverse
			p_preset = DirectionPreset.RowReverse if is_row else DirectionPreset.ColumnReverse
		else:#reverse
			p_preset = DirectionPreset.Row if old_preset == DirectionPreset.RowReverse else DirectionPreset.Column
		presets[p_category] = p_preset
	else:
		presets[p_category] = p_preset if p_state else -1
	
	undo_redo.create_action(p_category)
	undo_redo.add_do_method(_current_node, "edit_set_state", presets)
	undo_redo.add_undo_method(_current_node, "edit_set_state", _current_node.edit_get_state())
	undo_redo.commit_action()


func _notification(what):
	match what: 
		NOTIFICATION_ENTER_TREE, NOTIFICATION_THEME_CHANGED:
			pass

class EditorPopupButton extends Button:
	var arrow_icon:Texture2D
	var popup_panel:PopupPanel
	var popup_vbox:VBoxContainer
	
	func _init(EDSCALE):
		flat = true
		toggle_mode = true
		focus_mode = Control.FOCUS_NONE
		texture_filter = CanvasItem.TEXTURE_FILTER_NEAREST
		var image = Icon.get_image()
		image.resize(image.get_width(), image.get_width(),Image.INTERPOLATE_NEAREST)
		var resize_icon = ImageTexture.new().create_from_image(image)
		icon = resize_icon

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
				#arrow_icon = get_theme_icon("select_arrow", "Tree");
				pass
			NOTIFICATION_DRAW:
				pass
				#if is_instance_valid(arrow_icon):
					#var arrow_pos = Vector2(26,0)
					#arrow_pos.y = get_size().y / 2 - arrow_icon.get_height() / 2;
					#draw_texture(arrow_icon, arrow_pos);
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
	var EDSCALE = 1
	var BASE_SIZE = Vector2(32,32)
	func _init(scale):
		EDSCALE = scale
	
	func _add_button(p_category, p_preset, b):
		if preset_buttons.get(p_category) == null:
			preset_buttons[p_category] = {}
		preset_buttons[p_category][p_preset] = b


	func _add_row_button(p_row, p_category, p_preset, p_name):
		var b = Button.new()
		b.auto_translate = false
		b.toggle_mode = true
		b.set_custom_minimum_size(BASE_SIZE * EDSCALE)
		b.set_size(BASE_SIZE * EDSCALE)
		b.set_icon_alignment(HORIZONTAL_ALIGNMENT_CENTER)
		b.set_tooltip_text(p_name)
		b.set_flat(true)
		b.texture_filter = CanvasItem.TEXTURE_FILTER_NEAREST
		b.expand_icon = true
		p_row.add_child(b)
		b.pressed.connect(_preset_button_pressed.bind(p_category, p_preset))
		_add_button(p_category, p_preset, b)
		
		
	func _add_text_button(p_row, p_category, p_preset, p_name):
		var b = Button.new()
		b.toggle_mode = true
		b.set("theme_override_font_sizes/font_size",12)
		b.set_custom_minimum_size(BASE_SIZE * EDSCALE)
		b.set_size(BASE_SIZE * EDSCALE)
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
	signal flexbox_preset_selected
	var state:Dictionary

	const Category = {
		Direction = "flex_direction",
		DirectionReverse = "reverse",
		Wrap = "flex_wrap",
		Align = "align_items",
		Justify = "justify_content",
		AlignContent = "align_content",
	}

	const LABEL_WIDTH = 70
	func _init(scale):
		EDSCALE = scale
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
		_add_row_button(dir_row, Category.DirectionReverse, DirectionPreset.Reverse, "Reverse");

		#wrap
		var wrap_row = HBoxContainer.new()
		wrap_row.set_alignment(HBoxContainer.ALIGNMENT_BEGIN)
		wrap_row.add_theme_constant_override("separation", grid_separation)
		main_vb.add_child(wrap_row)
		var wrap_label = Label.new()
		wrap_label.set_text("Wrap")
		wrap_label.custom_minimum_size.x = LABEL_WIDTH
		wrap_row.add_child(wrap_label)
		_add_row_button(wrap_row, Category.Wrap, WrapPreset.NoWrap, "NoWrap")
		_add_row_button(wrap_row, Category.Wrap, WrapPreset.Wrap, "Wrap")
		
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
		_add_row_button(content_row, Category.AlignContent, AlignContentPreset.SpaceBetween, "SpaceBetween");
		_add_row_button(content_row, Category.AlignContent, AlignContentPreset.SpaceAround, "SpaceAround");
		_add_row_button(content_row, Category.AlignContent, AlignContentPreset.Stretch, "Stretch");
		

	func map_state(p_state):
		state = p_state
		#reset all
		for cat in preset_buttons:
			var cat_btns = preset_buttons[cat]
			for btn in cat_btns.values():
				btn.button_pressed = false
		#
		for category in p_state:
			var value = p_state[category]
			if category == Category.Direction:
				var is_reverse = value == DirectionPreset.RowReverse or value == DirectionPreset.ColumnReverse
				if value == DirectionPreset.Row or value == DirectionPreset.RowReverse:
					preset_buttons[Category.Direction][DirectionPreset.Row].button_pressed = true
				else:
					preset_buttons[Category.Direction][DirectionPreset.Column].button_pressed = true
				preset_buttons[Category.DirectionReverse][DirectionPreset.Reverse].button_pressed = is_reverse
			else:
				preset_buttons[category][value].button_pressed = true
		update_icons()

	func is_reverse():
		var direction = state[Category.Direction]
		return direction == DirectionPreset.ColumnReverse or direction == DirectionPreset.RowReverse

	func is_row():
		var direction = state[Category.Direction]
		return direction == DirectionPreset.RowReverse or direction == DirectionPreset.Row

	func get_icon(icon):
		return IconAssets.get_icon(icon)


	func _preset_button_pressed(p_category, p_preset):
		var update_icon = false
		var select_btn = preset_buttons[p_category][p_preset]
		for b in preset_buttons[p_category].values():
			if b == select_btn:
				continue
			b.button_pressed = false
		#
		if p_category == "flex_direction":
			var row_selected = preset_buttons[Category.Direction][DirectionPreset.Row].button_pressed
			var column_selected = preset_buttons[Category.Direction][DirectionPreset.Column].button_pressed
			if !row_selected && !column_selected:
				select_btn.button_pressed = true
			update_icon = true
		elif p_category == "reverse":
			update_icon = true
		
		flexbox_preset_selected.emit(p_category, p_preset, select_btn.button_pressed)
		if update_icon:
			update_icons()


	func _notification(what):
		match what:
			NOTIFICATION_ENTER_TREE,NOTIFICATION_THEME_CHANGED:
				if state.is_empty(): return
				update_icons()


	func update_icons():
		await get_tree().process_frame
		var direction =  "Row" if is_row() else "Column"
		var reverse = is_reverse()

		preset_buttons[Category.Direction][DirectionPreset.Row].icon = get_icon("DisplayFlexRow")
		preset_buttons[Category.Direction][DirectionPreset.Column].icon = get_icon("DisplayFlexColumn")
		preset_buttons[Category.DirectionReverse][DirectionPreset.Reverse].icon = get_icon("ArrowReverseIcon")
		#
		preset_buttons[Category.Wrap][WrapPreset.NoWrap].icon = get_icon("FlexWrapNoWrap%sIcon" % direction)
		preset_buttons[Category.Wrap][WrapPreset.Wrap].icon = get_icon("FlexWrapWrap%sIcon" % direction)
		#
		preset_buttons[Category.Align][AlignItemsPreset.FlexStart].icon = get_icon("AlignItemsStart%sIcon" % direction)
		preset_buttons[Category.Align][AlignItemsPreset.Center].icon = get_icon("AlignItemsCenter%sIcon" % direction)
		preset_buttons[Category.Align][AlignItemsPreset.FlexEnd].icon = get_icon("AlignItemsEnd%sIcon" % direction)
		preset_buttons[Category.Align][AlignItemsPreset.Stretch].icon = get_icon("AlignItemsStretch%sIcon" % direction)
		preset_buttons[Category.Align][AlignItemsPreset.Baseline].icon = get_icon("AlignItemsBaseline%sIcon" % direction)
		#
		var justify_content_start_icon = get_icon("JustifyContentStart%sIcon" % direction)
		var justify_content_end_icon = get_icon("JustifyContentEnd%sIcon" % direction)
		preset_buttons[Category.Justify][JustifyPreset.Center].icon = get_icon("JustifyContentCenter%sIcon" % direction)
		preset_buttons[Category.Justify][JustifyPreset.FlexStart].icon = justify_content_end_icon if reverse else justify_content_start_icon
		preset_buttons[Category.Justify][JustifyPreset.FlexEnd].icon =justify_content_start_icon if reverse else justify_content_end_icon
		preset_buttons[Category.Justify][JustifyPreset.SpaceBetween].icon = get_icon("JustifyContentSpaceBetween%sIcon" % direction)
		preset_buttons[Category.Justify][JustifyPreset.SpaceAround].icon = get_icon("JustifyContentSpaceAround%sIcon" % direction)
		preset_buttons[Category.Justify][JustifyPreset.SpaceEvenly].icon = get_icon("JustifyContentSpaceEvenly%sIcon" % direction)
		#
		preset_buttons[Category.AlignContent][AlignContentPreset.FlexStart].icon = get_icon("AlignContentStart%sIcon" % direction)
		preset_buttons[Category.AlignContent][AlignContentPreset.Center].icon = get_icon("AlignContentCenter%sIcon" % direction)
		preset_buttons[Category.AlignContent][AlignContentPreset.FlexEnd].icon = get_icon("AlignContentEnd%sIcon" % direction)
		preset_buttons[Category.AlignContent][AlignContentPreset.SpaceAround].icon = get_icon("AlignContentAround%sIcon" % direction)
		preset_buttons[Category.AlignContent][AlignContentPreset.SpaceBetween].icon = get_icon("AlignContentBetween%sIcon" % direction)
		preset_buttons[Category.AlignContent][AlignContentPreset.Stretch].icon = get_icon("AlignContentStretch%sIcon" % direction)
