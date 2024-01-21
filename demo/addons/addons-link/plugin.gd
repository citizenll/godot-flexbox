@tool
extends EditorPlugin


func _enter_tree():
	check_link()

func check_link():
	var dir = DirAccess.open("res://addons")
	if dir.dir_exists("./godot-flexbox"):return
	if OS.get_name()=="Windows":
		OS.execute("./setup.bat",[])
	else:
		OS.execute("sh",["./setup.sh"])
	OS.alert("addons copy suceess, will restart eidtor")
	EditorInterface.restart_editor(true)
