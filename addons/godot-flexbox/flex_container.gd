@tool
class_name FlexContainer
extends Container

@export var debug_draw = false

var _draw_rects = []

const EDGES = [1, 2, 3, 0]

enum FlexDirection {Column,ColumnReverse,Row,RowReverse}
enum FlexWrap {NoWrap,Wrap,WrapReverse}
enum JustifyContent {FlexStart,Center,FlexEnd,SpaceBetween,SpaceAround,SpaceEvenly}
enum AlignItems {Auto,FlexStart,Center,FlexEnd,Stretch,Baseline,SpaceBetween,SpaceAround}
enum AlignContent {Auto,FlexStart,Center,FlexEnd,Stretch,Baseline,SpaceBetween,SpaceAround}

const DEFAULT_VALUE = {
	reverse = 0,
	flex_wrap = FlexWrap.NoWrap,
	flex_direction = FlexDirection.Row,
	justify_content = JustifyContent.FlexStart,
	align_items = AlignItems.FlexStart,
	align_content = AlignContent.FlexStart
}

var _root: Flexbox
var _initialized = false

var _flex_list = []
enum FlexDataType { CID = 0, FLEXBOX, CONTROL }

var direction_reverse = DEFAULT_VALUE.reverse
@export var flex_direction:FlexDirection = DEFAULT_VALUE.flex_direction
@export var flex_wrap:FlexWrap = DEFAULT_VALUE.flex_wrap
@export var justify_content:JustifyContent = DEFAULT_VALUE.justify_content
@export var align_items:AlignItems = DEFAULT_VALUE.align_items
@export var align_content:AlignContent = DEFAULT_VALUE.align_content


var state:Dictionary = {flex_direction=null, flex_wrap=null,justify_content=null,align_items=null,align_content=null}

func _init():
	_root = Flexbox.new()
	update_state()


func update_state():
	for property in state:
		var value = get(property)
		state[property] = value


func _ready() -> void:
	_root.set_flex_direction(get("flex_direction"))
	_root.set_flex_wrap(get("flex_wrap"))
	_root.set_justify_content(get("justify_content"))
	_root.set_align_items(get("align_items"))
	_root.set_align_content(get("align_content"))
	update_state()
	set_process_input(false)
	_initialized = true


func _notification(what: int) -> void:
	match what:
		NOTIFICATION_SORT_CHILDREN:
			_resort()
		[NOTIFICATION_TRANSLATION_CHANGED, NOTIFICATION_LAYOUT_DIRECTION_CHANGED]:
			queue_sort()


func _resort() -> void:
	var root_size = get_size()
	_root.set_width(root_size.x)
	_root.set_height(root_size.y)
	
	if debug_draw:
		_draw_debug_rect(Rect2(Vector2.ZERO, root_size), Color(0, 0.8, 0.5, 1))
	
	var child_count = get_child_count()
	var valid_child_index = 0
	for i in range(child_count):
		var c = get_child(i)
		if not c.is_class("Control"): continue
		if c.is_set_as_top_level(): continue
		
		var cid = c.get_instance_id()
		var target_index = _find_index_from_flex_list(_flex_list, cid)
		var flexbox: Flexbox
		
		if not c.is_visible_in_tree():
			if target_index != -1:
				# Remove invisible flexbox
				_root.remove_child_at(target_index)
				_flex_list.remove_at(target_index)
#				print("Remove: ", target_index)
			continue
		
		if target_index != -1:
			# Replace flexbox
			var old_flex_data = _flex_list[valid_child_index]
			var new_flex_data = _flex_list[target_index]
			flexbox = new_flex_data[FlexDataType.FLEXBOX]
			
			if old_flex_data[FlexDataType.CID] != cid:
				_root.swap_child(valid_child_index, target_index)
				_flex_list[target_index] = old_flex_data
				_flex_list[valid_child_index] = new_flex_data
#				print("Swap: ", target_index, " <-> ", valid_child_index)
				
		else:
			# Add flexbox
			flexbox = Flexbox.new()
			_root.insert_child(flexbox, valid_child_index)
			_flex_list.insert(valid_child_index, [cid, flexbox, c])
#			print("Add: ", valid_child_index)
			
#		print(flexbox.get_instance_id(), ", ", _root.get_child_count())
		_set_control_min_size(c, flexbox)
		valid_child_index += 1
	
	child_count = valid_child_index
	
	# Remove unused flexbox
	if child_count != _flex_list.size():
		for i in range(_flex_list.size() - 1, child_count - 1, -1):
			_root.remove_child_at(i)
#			print("Remove: ", i)
		_flex_list.resize(child_count)
		_root.mark_dirty_and_propogate()
	
#	print("Count: ", _root.get_child_count())
	
	# Calculate layout
	_root.calculate_layout(NAN, NAN, 1)
	
	# Adjust layout
	for flex_data in _flex_list:
		var flexbox = flex_data[FlexDataType.FLEXBOX]
		var c = flex_data[FlexDataType.CONTROL]
		
		var offset = Vector2(flexbox.get_computed_left(), flexbox.get_computed_top())
		var size = Vector2(flexbox.get_computed_width(), flexbox.get_computed_height())
		var rect = Rect2(offset, size)
		_fit_child_in_rect(c, rect)
		
		if debug_draw:
			_draw_debug_rect(rect, Color(1, 0, 0, 0.8))
	
	# Redraw component
	queue_redraw()


func _find_index_from_flex_list(flex_list: Array, cid: int) -> int:
	for i in range(flex_list.size()):
		if flex_list[i][FlexDataType.CID] == cid:
			return i
	return -1


func _set_control_min_size(c: Control, flexbox: Flexbox):
	var size = c.custom_minimum_size if c.custom_minimum_size else c.size
	flexbox.set_min_width(size.x)
	flexbox.set_min_height(size.y)


func _fit_child_in_rect(child: Control, rect: Rect2) -> void:
	var cid = child.get_instance_id()
	child.set_position(rect.position)
	child.set_size(rect.size)
	child.set_rotation(0)
	child.set_scale(Vector2.ONE)


func apply_flex_meta(node, metas):
	for key in metas:
		var value = metas[key]
		apply_child_property(node, key, value)


func apply_child_property(node, prop, value):
	match prop:
		"basis":
			if typeof(value) == TYPE_STRING and value == "auto":
				node.set_flex_basis_auto()
			else:
				node.set_flex_basis(value)
		"grow":
			node.set_flex_grow(value)
		"padding":
			for i in range(4):
				var edge = EDGES[i]
				node.set_padding(edge, value[i])  #value->edge, value1->padding
		"margin":
			for i in range(4):
				var edge = EDGES[i]
				var value1 = value[i]
				if typeof(value1) == TYPE_STRING and value1 == "auto":
					node.set_margin_auto(edge)
				else:
					node.set_margin(edge, value1)  #value->edge, value1->margin
		"align_self":
			node.set_align_self(value)


func flex_property_changed(property, value):
	value = process_value(property, value)
	state[property] = value
	set(property, value)
	match property:
		"flex_direction":
			_root.set_flex_direction(value)
		"flex_wrap":
			_root.set_flex_wrap(value)
		"justify_content":
			_root.set_justify_content(value)
		"align_items":
			_root.set_align_items(value)
		"align_content":
			_root.set_align_content(value)


func update_layout():
	queue_sort()


func edit_set_state(p_state):
	for property in p_state:
		var value = p_state[property]
		flex_property_changed(property, value)
	update_layout()


func edit_get_state():
	return state.duplicate()


func _draw():
	for r in _draw_rects:
		draw_rect(r.rect, r.color, false, 2)


func _draw_debug_rect(rect, color):
	_draw_rects.append({rect = rect, color = color})


func process_value(key, value):
	if DEFAULT_VALUE.has(key) && value == -1:
		return DEFAULT_VALUE[key]
	return value


func get_class():
	return "FlexContainer"
