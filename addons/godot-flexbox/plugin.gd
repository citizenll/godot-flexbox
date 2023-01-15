# ############################################################################ #
# Copyright © 2022-2023 citizenl <hi@citizenl.me>
# MIT License.
# ############################################################################ #

tool
extends EditorPlugin

func _enter_tree():
	add_custom_type("FlexContainer", "Container", preload("res://addons/godot-flexbox/bin/gdflexbox.gdns"), preload("res://addons/godot-flexbox/icon.svg"))

func _exit_tree():
	remove_custom_type("FlexContainer")

