# ############################################################################ #
# Copyright © 2022-2023 citizenl <hi@citizenl.me>
# MIT License.
# ############################################################################ #

tool
extends EditorPlugin

var Icon = preload("icon.svg")
var FlexContainer = preload("flex_container.gd")

func _enter_tree():
	add_custom_type("FlexContainer", "Container", FlexContainer, Icon)

func _exit_tree():
	remove_custom_type("FlexContainer")

