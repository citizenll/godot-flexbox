@tool
extends EditorPlugin


func _enter_tree():
	check_link()

func check_link():
	var dir = DirAccess.open("res://addons")
	if dir.dir_exists("./godot-flexbox"):return
	if OS.get_name()=="Window":
		OS.execute("./setup.bat",[])
		print("link on windows")
	else:
		OS.execute("sh",["./setup.sh"])
		print("link on unix")
	OS.alert("addons copy suceess, will restart eidtor")
	EditorInterface.restart_editor(true)
