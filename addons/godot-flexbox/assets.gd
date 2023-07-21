class_name IconAssets

static var icons_map
const flex_icons_sources = [
	"AlignContentAroundRowIcon",
	"AlignContentBetweenRowIcon",
	"AlignContentCenterRowIcon",
	"AlignContentEndRowIcon",
	"AlignContentStartRowIcon",
	"AlignContentStretchRowIcon",

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
	"JustifyContentSpaceBetweenRowReverseIcon",
	"JustifyContentSpaceEvenlyColumnIcon",
	"JustifyContentSpaceEvenlyRowIcon",
	"JustifyContentStartColumnIcon",
	"JustifyContentStartRowIcon",
]
static func get_icon(icon:String):
	return icons_map.get(icon)

#maybe bugs here, map should be initial here
static func generate_icons():
	icons_map = {}
	for icon_name in flex_icons_sources:
		icons_map[icon_name] = load("res://addons/godot-flexbox/assets/%s.svg" % icon_name)
