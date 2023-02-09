extends EditorProperty


func _ready() -> void:
	label = "FlexProperty"
	var _hidden_menu_button = MenuButton.new()
	_hidden_menu_button.text = "Visible nodes"
	add_child(_hidden_menu_button)
