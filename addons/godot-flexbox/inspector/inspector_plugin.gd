extends EditorInspectorPlugin

const EditorProperty = preload("editor_property.gd")
const EditorScene = preload("res://addons/godot-flexbox/inspector/Properties.tscn")


func can_handle(object: Object) -> bool:
	var isFlexChild = object.get_meta("_flex_child", -1)
	var condition = isFlexChild != -1
	if condition:
		var instance = EditorScene.instance()
		instance.editObject = object
		instance.connect("property_changed", self, "property_changed", [object])
		add_custom_control(instance)
	return condition


func property_changed(properties, object):
	object.set_meta("_flex_metas", properties)
	var parent = object.get_parent()
	if parent.has_method("update_layout"):
		parent.call("update_layout")
