var properties = {}


func add(name, type, default_value, hint, hint_string = ""):
	properties[name] = {
		"hint": hint,
		"hint_string": hint_string,
		"usage": PROPERTY_USAGE_DEFAULT,
		"name": name,
		"type": type,
		"value": default_value
	}


func get(name):
	if properties.has(name):
		return properties[name].value
	return null


func set(name, value):
	if properties.has(name):
		properties[name].value = value
		return true
	return false


func _init(list):
	for prop in list:
		add(prop[0], prop[1], prop[2], prop[3], prop[4])
