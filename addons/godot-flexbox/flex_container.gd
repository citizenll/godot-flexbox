@tool
extends Container

@export var debug_draw = false

var _draw_rects = []

var PropertyList = preload("flex_property.gd")

const EDGES = [1, 2, 3, 0]

var _root
var _initialized = false
var _flex_cache = {}
var _property_list = PropertyList.new(
	[
		[
			"flex/flex_direction",
			TYPE_INT,
			2,
			PROPERTY_HINT_ENUM,
			"Column,ColumnReverse,Row,RowReverse"
		],
		["flex/flex_wrap", TYPE_INT, 0, PROPERTY_HINT_ENUM, "NoWrap,Wrap,WrapReverse"],
		[
			"alignment/justify_content",
			TYPE_INT,
			0,
			PROPERTY_HINT_ENUM,
			"FlexStart,Center,FlexEnd,SpaceBetween,SpaceAround,SpaceEvenly"
		],
		[
			"alignment/align_items",
			TYPE_INT,
			1,
			PROPERTY_HINT_ENUM,
			"Auto,FlexStart,Center,FlexEnd,Stretch,Baseline,SpaceBetween,SpaceAround"
		],
		[
			"alignment/align_content",
			TYPE_INT,
			0,
			PROPERTY_HINT_ENUM,
			"Auto,FlexStart,Center,FlexEnd,Stretch,Baseline,SpaceBetween,SpaceAround"
		],
	]
)


func _init():
	_root = Flexbox.new()


func _ready() -> void:
	_root.set_flex_direction(get("flex/flex_direction"))
	_root.set_flex_wrap(get("flex/flex_wrap"))
	_root.set_justify_content(get("alignment/justify_content"))
	_root.set_align_items(get("alignment/align_items"))
	_root.set_align_content(get("alignment/align_content"))

	set_process_input(false)
	_initialized = true


func _notification(what: int) -> void:
	match what:
		NOTIFICATION_SORT_CHILDREN:
			_resort()
		[NOTIFICATION_TRANSLATION_CHANGED, NOTIFICATION_LAYOUT_DIRECTION_CHANGED]:
			queue_sort()
		


func _resort() -> void:
	_draw_rects.clear()
	var s = Time.get_ticks_usec()
	var rootSize = get_size()
	_root.set_width(rootSize.x)
	_root.set_height(rootSize.y)
	if debug_draw:
		_draw_debug_rect(Rect2(Vector2.ZERO, rootSize), Color(0, 0.8, 0.5, 1))
	#
	var childCount = get_child_count()
	for i in range(childCount):
		var c = get_child(i)
		if c.is_set_as_top_level():
			continue
		if not c or not c.is_visible_in_tree():
			continue
		c.set_meta("_flex_child", 1)

		var cid = c.get_instance_id()
		var flexbox = _flex_cache.get(cid)
		var size = c.custom_minimum_size
		if not flexbox:
			flexbox = Flexbox.new()
			_root.insert_child(flexbox, i)
			_flex_cache[cid] = flexbox
		#
		flexbox.set_min_width(size.x)
		flexbox.set_min_height(size.y)
		#
		var flexMetas = c.get_meta("_flex_metas", -1)
		if typeof(flexMetas) == TYPE_DICTIONARY and flexMetas.size():
			apply_flex_meta(flexbox, flexMetas)
	#
	var calc = Time.get_ticks_usec()
	#print("create time:", calc - s)
	_root.calculate_layout(NAN, NAN, 1)
	var calced = Time.get_ticks_usec()
	#print("calc time:", calced - calc)
	#
	for i in range(childCount):
		var c = get_child(i)
		if c.is_set_as_top_level():
			continue
		if not c or not c.is_visible_in_tree():
			continue
		#
		var cid = c.get_instance_id()
		var flexbox = _flex_cache[cid]
		var offset = Vector2(flexbox.get_computed_left(), flexbox.get_computed_top())
		var size = Vector2(flexbox.get_computed_width(), flexbox.get_computed_height())
		
		var rect = Rect2(offset, size)
		if debug_draw:
			_draw_debug_rect(rect, Color(1, 0, 0, 0.8))
		fit_child_in_rect(c, rect)
	var end = Time.get_ticks_usec()
	#print("sort time:", end-s, " ", end - calced)
	queue_redraw()


func fit_child_in_rect(child: Control, rect: Rect2) -> void:
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
	match property:
		"flex/flex_direction":
			_root.set_flex_direction(value)
		"flex/flex_wrap":
			_root.set_flex_wrap(value)
		"alignment/justify_content":
			_root.set_justify_content(value)
		"alignment/align_items":
			_root.set_align_items(value)
		"alignment/align_content":
			_root.set_align_content(value)
	queue_sort()


func update_layout():
	queue_sort()


func _draw():
	for r in _draw_rects:
		draw_rect(r.rect, r.color, false, 2)


func _draw_debug_rect(rect, color):
	_draw_rects.append({rect = rect, color = color})


func _get(property):
	var v = _property_list.get(property)
	return v


func _set(property, value):
	var dirty = _property_list.set(property, value)
	if dirty and _initialized:
		flex_property_changed(property, value)
		notify_property_list_changed()


func _get_property_list():
	return _property_list.properties.values()
