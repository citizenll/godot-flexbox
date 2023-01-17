extends Node2D

# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	OS.exit_code = 1
	var Flexbox = load("res://flex.gdns")
	print("Native Script ", Flexbox)
	if not Flexbox || !is_instance_valid(Flexbox):
		return
	print("Library ", Flexbox.library)
	if not Flexbox.library || !is_instance_valid(Flexbox.library):
		return
	var root = Flexbox.new()
	root.set_width(340)
	print("root width:",root.get_computed_width())
	root.set_max_height(170)
	root.set_min_height(0)

	var measureFunc = funcref(self, "measure")
	var child = Flexbox.new()
	child.set_flex_grow(0)
	child.set_flex_shrink(1)
	child.set_measure_func(measureFunc)

	root.insert_child(child, 0)
	
	print("child count:", root.get_child_count())
	
	root.calculate_layout(NAN, NAN, 2)
	print(root.get_computed_width(),"-", child.get_computed_width())

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func measure(width, widthMode, height, heightMode):
	print("measure ", width, widthMode, height, heightMode)
	return {width=100, height=200}
