class_name IconAssets

static var icons_map
static var current_theme
static var color_conversion_map = {}

const flex_icons_sources = [
	"AlignContentAroundRowIcon",
	"AlignContentAroundColumnIcon",
	"AlignContentBetweenRowIcon",
	"AlignContentBetweenColumnIcon",
	"AlignContentCenterRowIcon",
	"AlignContentCenterColumnIcon",
	"AlignContentEndRowIcon",
	"AlignContentEndColumnIcon",
	"AlignContentStartRowIcon",
	"AlignContentStartColumnIcon",
	"AlignContentStretchRowIcon",
	"AlignContentStretchColumnIcon",

	"AlignItemsBaselineColumnIcon",
	"AlignItemsBaselineRowIcon",
	"AlignItemsCenterColumnIcon",
	"AlignItemsCenterRowIcon",
	"AlignItemsEndColumnIcon",
	"AlignItemsEndRowIcon",
	"AlignItemsStartColumnIcon",
	"AlignItemsStartRowIcon",
	"AlignItemsStretchColumnIcon",
	"AlignItemsStretchRowIcon",

	"DisplayFlexRow",
	"DisplayFlexColumn",
	"ArrowReverseIcon",

	"FlexWrapNoWrapColumnIcon",
	"FlexWrapNoWrapRowIcon",
	"FlexWrapWrapColumnIcon",
	"FlexWrapWrapRowIcon",

	"JustifyContentCenterColumnIcon",
	"JustifyContentCenterRowIcon",
	"JustifyContentEndColumnIcon",
	"JustifyContentEndRowIcon",
	"JustifyContentSpaceAroundColumnIcon",
	"JustifyContentSpaceAroundRowIcon",
	"JustifyContentSpaceBetweenColumnIcon",
	"JustifyContentSpaceBetweenRowIcon",
	"JustifyContentSpaceEvenlyColumnIcon",
	"JustifyContentSpaceEvenlyRowIcon",
	"JustifyContentStartColumnIcon",
	"JustifyContentStartRowIcon",
]

static func get_icon(icon:String):
	return icons_map[current_theme].get(icon)


#maybe bugs here, map should be initial here
static func generate_icons(dark_theme):
	if icons_map == null:
		icons_map = {}
	if icons_map.has(dark_theme):
		current_theme = dark_theme
		return
	if current_theme != dark_theme:
		current_theme = dark_theme
		icons_map[current_theme] = {}
		var icon_theme = "dark" if dark_theme else "light"
		for icon_name in flex_icons_sources:
			icons_map[current_theme][icon_name] = load("res://addons/godot-flexbox/assets/%s/%s.svg" % [icon_theme, icon_name])


static func add_conversion_color_pair(p_from_color:String, p_to_color:String):
	color_conversion_map[Color.html(p_from_color)] = Color.html(p_to_color)


static func conversion_color_pairs():
	color_conversion_map = {}
	add_conversion_color_pair("#d6d6d6", "#474747"); # Highlighted part
	add_conversion_color_pair("#474747", "#d6d6d6"); # Background part
