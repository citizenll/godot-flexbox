extends Control

onready var controlContainer := $Task/HBoxContainer
onready var control := $Task/HBoxContainer/Control
onready var flexbox := $Panel/FlexContainer

var controls = [
	{
		label = "FlexDirection",
		property = "flex_direction",
		selected = 2,
		value = "Column,ColumnReverse,Row,RowReverse".split(",")
	},
	{
		label = "JustifyContent",
		property = "justify_content",
		selected = 0,
		value = "FlexStart,Center,FlexEnd,SpaceBetween,SpaceAround,SpaceEvenly".split(",")
	},
	{
		label = "AlignItems",
		property = "align_items",
		selected = 0,
		value = "Auto,FlexStart,Center,FlexEnd,Stretch,Baseline,SpaceBetween,SpaceAround".split(",")
	},
	{
		label = "AlignContent",
		property = "align_content",
		selected = 0,
		value = "Auto,FlexStart,Center,FlexEnd,Stretch,Baseline,SpaceBetween,SpaceAround".split(",")
	},
	{
		label = "FlexWrap",
		property = "flex_wrap",
		selected = 0,
		value = "NoWrap,Wrap,WrapReverse".split(","),
	},
]


# Called when the node enters the scene tree for the first time.
func _ready():
	for i in range(controls.size()):
		var item = controls[i]
		var inst = control.duplicate()
		controlContainer.add_child(inst)
		inst.get_child(0).text = item.label
		var selectNode = inst.get_child(1)
		for j in range(item.value.size()):
			selectNode.add_item(item.value[j], j)
		selectNode.selected = item.selected
		selectNode.connect("item_selected", self, "_on_item_selected", [i])
	control.queue_free()


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_item_selected(index, cindex):
	flexbox.set(controls[cindex].property, index)
