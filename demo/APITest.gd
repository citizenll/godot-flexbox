extends Control


# Called when the node enters the scene tree for the first time.
func _ready():
	var s = Time.get_ticks_usec()
	var root = Flexbox.new()
	root.set_min_width(500)
	root.set_min_height(300)
	for i in range(10):
		var child = Flexbox.new()
		child.set_min_width(50)
		child.set_min_height(100)
		root.insert_child(child, i)
		
	#child.set_measure_func(Callable(self, "measure"))
	var calc = Time.get_ticks_usec()
	print("--create--",calc-s)
	

	
	root.calculate_layout(NAN,NAN,1)
	
	var child = root.get_child(5)
	print(child.is_dirty())
	child.set_flex_grow(1)
	print("---grow", child.get_flex_grow())
	
	var e = Time.get_ticks_usec()
	print("--calc--",e-calc)
	
	print("--total--",e-s)
	
	print("root:",root.get_computed_width()," ", root.get_computed_height())


func measure(width, widthMode,height, heightMode):
	print("mesure:", width, widthMode,height, heightMode)
	return {width=112,height=130}
