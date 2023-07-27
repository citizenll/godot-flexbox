extends Control

const TOTAL_COUNT = 1000
# Called when the node enters the scene tree for the first time.
func _ready():
	var s = Time.get_ticks_usec()
	var root = Flexbox.new()
	root.set_width(500)
	root.set_height(300)
	var childs = {}
	for i in range(TOTAL_COUNT):
		var child = Flexbox.new()
		child.set_min_width(50)
		child.set_min_height(100)
		childs[child.get_instance_id()] = child #need have a reference here, or else error because of child freed
		root.insert_child(child, i)
		
	#child.set_measure_func(Callable(self, "measure"))
	var calc = Time.get_ticks_usec()
	print("create time>",calc-s, " avg:", (calc-s) / TOTAL_COUNT)
	root.calculate_layout(NAN,NAN,1)
	var e = Time.get_ticks_usec()
	print("layout time>",e-calc)
	print("total  time>",e-s, " avg:", (e-s) / TOTAL_COUNT)


func measure(width, widthMode,height, heightMode):
	print("mesure:", width, widthMode,height, heightMode)
	return {width=112,height=130}
