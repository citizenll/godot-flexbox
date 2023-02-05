tool
extends Container

var Flexbox = preload("bin/flexbox.gdns")
var PropertyList = preload("flex_property.gd")

var _root = Flexbox.new()
var _children_flex = {}
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
			0,
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


func _ready() -> void:
	_root.set_flex_direction(get("flex/flex_direction"))
	_root.set_flex_wrap(get("flex/flex_wrap"))
	_root.set_justify_content(get("alignment/justify_content"))
	_root.set_align_items(get("alignment/align_items"))
	_root.set_align_content(get("alignment/align_content"))

	set_process_input(false)


func _notification(what: int) -> void:
	if what == NOTIFICATION_SORT_CHILDREN:
		_resort()
		print("sort children")


func _resort() -> void:
	var rootSize = get_size()
	_root.set_width(rootSize.x)
	_root.set_height(rootSize.y)
	#
	var childCount = get_child_count()
	for i in range(childCount):
		var c = get_child(i)
		var cid = c.get_instance_id()
		var flexbox = _children_flex.get(cid)
		if not flexbox:
			var size = c.get_size()
			flexbox = Flexbox.new()
			flexbox.set_width(size.x)
			flexbox.set_height(size.y)
			_root.insert_child(flexbox, i)
			_children_flex[cid] = flexbox
	#
	_root.calculate_layout(NAN, NAN, 1)
	#
	for i in range(childCount):
		var c = get_child(i)
		var flexbox = _children_flex[c.get_instance_id()]
		c.set_position(Vector2(flexbox.get_computed_left(), flexbox.get_computed_top()))
		c.set_size(Vector2(flexbox.get_computed_width(), flexbox.get_computed_height()))


func fit_child_in_rect(child: Control, rect: Rect2) -> void:
	var cid = child.get_instance_id()
	var size = child.get_combined_minimum_size()
	child.set_position(rect.position)
	child.set_size(rect.size)
	child.set_rotation(0)
	child.set_scale(Vector2(1, 1))


func property_changed(property, value):
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


func _get(property):
	var v = _property_list.get(property)
	return v


func _set(property, value):
	var dirty = _property_list.set(property, value)
	if dirty:
		#print("set:", dirty, " ", property, " ", value)
		property_changed(property, value)
		property_list_changed_notify()


func _get_property_list():
	return _property_list.properties.values()
