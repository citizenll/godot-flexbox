extends Control


# Called when the node enters the scene tree for the first time.
func _ready():
	var root = Flexbox.new()
	root.set_width(500)
	root.set_height(300)
	
	var child = Flexbox.new()
	child.set_flex_shrink(1)
	child.set_flex_grow(0)
	child.set_measure_func(Callable(self, "measure"))
	
	root.insert_child(child, 0)
	
	root.calculate_layout(NAN,NAN,1)
	
	
	print("root:",root.get_computed_width()," ", root.get_computed_height())
	print("child:",child.get_computed_width()," ", child.get_computed_height())
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass


func measure(width, widthMode,height, heightMode):
	print("mesure:", width, widthMode,height, heightMode)
	return {width=112,height=130}
