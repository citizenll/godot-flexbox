extends EditorInspectorPlugin

var properties = {}
var has_script = false
var script_index = -1

func _can_handle(object: Object) -> bool:
	script_index = -1
	if not object is Control:
		return false
	var parent = object.get_parent()
	var passed = parent is FlexContainer
	if passed:
		var property_list = object.get_property_list().map(func(item):return item.name)
		var scripts = property_list.filter(func(name):return name.match("*.gd"))
		has_script = scripts.size() > 0
	return passed

func _parse_category(object, category):
	## make flex property below the scrip
	if has_script: script_index += 1
	else: script_index+=2
	if script_index != 1 : return
	properties = object.get_meta("flex_metas", {})

	var align_self = EditorPropertyEnum.new()
	align_self.setup(properties.get("align_self", 0))
	var grow = EditorPropertyNumber.new()
	grow.setup("Flex Grow", properties.get("grow", 0))
	var shrink = EditorPropertyNumber.new()
	shrink.setup("Flex Shrink", properties.get("shrink", 1))
	var margin = EditorPropertySpacing.new("Margin")
	margin.setup(properties.get("margin", [0,0,0,0]))
	# var padding = EditorPropertySpacing.new("Padding")
	# padding.setup(properties.get("padding", [0,0,0,0]))
	#
	var _on_change_handle = _property_value_changed.bind(object)
	align_self.property_changed.connect(_on_change_handle)
	grow.property_changed.connect(_on_change_handle)
	shrink.property_changed.connect(_on_change_handle)
	margin.property_changed.connect(_on_change_handle)
	#padding.property_changed.connect(_on_change_handle)
	#
	add_property_editor("align_self", align_self)
	add_property_editor("grow", grow)
	add_property_editor("shrink", shrink)
	add_property_editor("spacing/margin", margin)
	#add_property_editor("spacing/padding", padding)


func _property_value_changed(property, value, field, changing, object):
	if property == "spacing/margin":
		var margin = properties.get("margin", [0,0,0,0])
		margin[field.to_int()] = value
		value = margin
		property = 'margin'
	elif property == "spacing/padding":
		var padding = properties.get("padding", [0,0,0,0])
		padding[field.to_int()] = value
		value = padding
		property = 'padding'
	
	properties[property] = value
	object.set_meta("flex_metas", properties)
	var parent = object.get_parent()
	if parent.has_method("update_layout"):
		parent.call("update_layout")


class EditorPropertySpacing extends EditorProperty:
	var spin := []
	var linked:TextureButton

	var top = 0
	var right = 0
	var bottom = 0
	var left = 0

	func _init(spacing):
		label = spacing
		
		spin.resize(4)
		var bc = VBoxContainer.new()
		bc.set_h_size_flags(SIZE_EXPAND_FILL)
		add_child(bc)
		
		var desc = ["T","R", "B", "L"]
		for i in 4:
			spin[i] = EditorSpinSlider.new()
			spin[i].set_allow_greater(true)
			spin[i].set_flat(true)
			spin[i].set_label(desc[i])
			bc.add_child(spin[i])
			add_focusable(spin[i])
			spin[i].connect("value_changed", _value_changed.bind(str(i)))
		

	func setup(initial:Array):
		for i in 4:
			spin[i].value = initial[i]


	func _value_changed(val, p_name):
		emit_changed(get_edited_property(), val, p_name)


class EditorPropertyEnum extends EditorProperty:
	var p_options = "Auto,FlexStart,Center,FlexEnd,Stretch,Baseline,SpaceBetween,SpaceAround".split(",")

	var options:OptionButton

	func _init():
		label = "Align Self"
		options = OptionButton.new()
		options.auto_translate = false
		options.set_clip_text(true)
		options.set_flat(true)
		add_child(options)
		add_focusable(options)
		options.connect("item_selected", _option_selected)
	
	func setup(selected = 0):
		options.clear()
		var current_val = 0
		for i in p_options.size():
			options.add_item(p_options[i])
			options.set_item_metadata(i, current_val)
			current_val += 1;
		options.selected = selected
	
	func _option_selected(p_which):
		emit_changed(get_edited_property(), p_which)


class EditorPropertyNumber extends EditorProperty:
	var spin:EditorSpinSlider
	func _init():
		spin = EditorSpinSlider.new()
		spin.set_flat(true)
		spin.set_label("")
		add_child(spin)
		add_focusable(spin)
		spin.connect("value_changed", _value_changed)
		spin.set_min(0)
	
	func setup(desc,val):
		label = desc
		spin.value = val
		spin.set_step(0.001)

	func _value_changed(val):
		emit_changed(get_edited_property(), val)
