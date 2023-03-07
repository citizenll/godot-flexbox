@tool
extends Control
signal property_changed

@onready var basisNode := $VC/Basis/LineEdit
@onready var growNode := $VC/Grow/LineEdit
@onready var shrinkNode := $VC/Shrink/LineEdit
@onready var optionButton := $VC/FlexProperty/AlignSelfOpts
#padding
@onready var paddingTopNode := $VC/Layout/Padding/LETop
@onready var paddingRightNode := $VC/Layout/Padding/LERight
@onready var paddingBottomNode := $VC/Layout/Padding/LEBottom
@onready var paddingLeftNode := $VC/Layout/Padding/LELeft
@onready var paddingNodes = [paddingTopNode, paddingRightNode, paddingBottomNode, paddingLeftNode]
#margin
@onready var marginTopNode := $VC/Layout/Margin/LETop
@onready var marginRightNode := $VC/Layout/Margin/LERight
@onready var marginBottomNode := $VC/Layout/Margin/LEBottom
@onready var marginLeftNode := $VC/Layout/Margin/LELeft
@onready var marginNodes = [marginTopNode, marginRightNode, marginBottomNode, marginLeftNode]

var options = "Auto,FlexStart,Center,FlexEnd,Stretch,Baseline,SpaceBetween,SpaceAround".split(",")

var exportProp = {}
var editObject: Node
var paddings := [0, 0, 0, 0]  #["top","right","bottom","left"]
var margins := [0, 0, 0, 0]  #["top","right","bottom","left"]


func _ready():
	var props = editObject.get_meta("_flex_metas")
	optionButton.clear()  #why already has options?
	for i in options:
		optionButton.add_item(i)
	if not props:
		return
	#
	for key in props:
		exportProp[key] = props[key]
		if key == "align_self":
			optionButton.select(props[key])
		elif key == "basis":
			basisNode.text = str(props[key])
		elif key == "grow":
			growNode.text = str(props[key])
		elif key == "shrink":
			shrinkNode.text = str(props[key])
		elif key == "padding":
			paddings = props[key]
			for i in range(4):
				paddingNodes[i].text = str(paddings[i])
		elif key == "margin":
			margins = props[key]
			for i in range(4):
				marginNodes[i].text = str(margins[i])
			pass


func _on_LineEdit_text_changed(originValue, args1, args2 = null):
	var value
	var intVal = originValue.to_int()
	if args1 == "basis":
		value = _parse_value(originValue, "auto")
	else:
		value = intVal
	#
	if args1 == "padding":
		paddings[args2] = _parse_value(originValue, 0)
		exportProp[args1] = paddings
	elif args1 == "margin":
		margins[args2] = _parse_value(originValue, 0)
		exportProp[args1] = margins
	else:
		exportProp[args1] = value
	emit_signal("property_changed", exportProp)


func _parse_value(value, default = null):
	var intVal = value.to_int()
	if intVal == 0 and value != "auto":
		return default
	elif value == "auto":
		return "auto"
	else:
		return intVal


func _on_AlignSelfOpts_item_selected(index):
	exportProp["align_self"] = index
	emit_signal("property_changed", exportProp)
