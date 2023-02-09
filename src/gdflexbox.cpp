#include "gdflexbox.h"

using namespace godot;
//
static Variant fromYGValue(YGValue const &ygValue)
{
    Dictionary value;
    value["unit"] = static_cast<int>(ygValue.unit);
    value["value"] = ygValue.value;
    return value;
}

static YGSize globalMeasureFunc(
    YGNodeRef nodeRef,
    float width,
    YGMeasureMode widthMode,
    float height,
    YGMeasureMode heightMode)
{
    Flexbox &node = *reinterpret_cast<Flexbox *>(YGNodeGetContext(nodeRef));

    const Dictionary &size = node.call_measure_func(width, widthMode, height, heightMode);
    Variant w = size["width"];
    Variant h = size["height"];
    YGSize ygSize = {static_cast<float>(w), static_cast<float>(h)};
    // String info = "[measure] width: " + String::num_real(w) + " height: " + String::num_real(h);
    // GODOT_LOG(0, info);
    return ygSize;
}

static void globalDirtiedFunc(YGNodeRef nodeRef)
{
    Flexbox &node = *reinterpret_cast<Flexbox *>(YGNodeGetContext(nodeRef));

    node.call_dirtied_func();
}
//
Flexbox::Flexbox()
{
}
Flexbox::~Flexbox()
{
}

void Flexbox::_init()
{
    // m_node = YGNodeNewWithConfig(YGConfigGetDefault());
    m_node = YGNodeNew();
    YGNodeSetContext(m_node, reinterpret_cast<void *>(this));
}

void Flexbox::dirtied()
{
    GODOT_LOG(0, "Flexbox::dirtied");
}
//====================================================
/* static */ Flexbox *Flexbox::fromYGNode(YGNodeRef nodeRef)
{
    return reinterpret_cast<Flexbox *>(YGNodeGetContext(nodeRef));
}
/* static */ void Flexbox::destroy(Flexbox *node)
{
    delete node;
}
void Flexbox::copy_style(Flexbox *other)
{
    YGNodeCopyStyle(m_node, other->m_node);
}

void Flexbox::set_position_type(int positionType)
{
    YGNodeStyleSetPositionType(m_node, static_cast<YGPositionType>(positionType));
}
void Flexbox::set_position(int edge, double position)
{
    YGNodeStyleSetPosition(m_node, static_cast<YGEdge>(edge), position);
}
void Flexbox::set_position_percent(int edge, double position)
{
    YGNodeStyleSetPositionPercent(m_node, static_cast<YGEdge>(edge), position);
}

void Flexbox::set_align_content(int alignContent)
{
    YGNodeStyleSetAlignContent(m_node, static_cast<YGAlign>(alignContent));
}

void Flexbox::set_align_items(int alignItems)
{
    YGNodeStyleSetAlignItems(m_node, static_cast<YGAlign>(alignItems));
}

void Flexbox::set_align_self(int alignSelf)
{
    YGNodeStyleSetAlignSelf(m_node, static_cast<YGAlign>(alignSelf));
}

void Flexbox::set_flex_direction(int flexDirection)
{
    YGNodeStyleSetFlexDirection(
        m_node, static_cast<YGFlexDirection>(flexDirection));
}

void Flexbox::set_flex_wrap(int flexWrap)
{
    YGNodeStyleSetFlexWrap(m_node, static_cast<YGWrap>(flexWrap));
}

void Flexbox::set_justify_content(int justifyContent)
{
    YGNodeStyleSetJustifyContent(m_node, static_cast<YGJustify>(justifyContent));
}

void Flexbox::set_margin(int edge, double margin)
{
    YGNodeStyleSetMargin(m_node, static_cast<YGEdge>(edge), margin);
}

void Flexbox::set_margin_percent(int edge, double margin)
{
    YGNodeStyleSetMarginPercent(m_node, static_cast<YGEdge>(edge), margin);
}

void Flexbox::set_margin_auto(int edge)
{
    YGNodeStyleSetMarginAuto(m_node, static_cast<YGEdge>(edge));
}

void Flexbox::set_overflow(int overflow)
{
    YGNodeStyleSetOverflow(m_node, static_cast<YGOverflow>(overflow));
}

void Flexbox::set_display(int display)
{
    YGNodeStyleSetDisplay(m_node, static_cast<YGDisplay>(display));
}

void Flexbox::set_flex(double flex)
{
    YGNodeStyleSetFlex(m_node, flex);
}

void Flexbox::set_flex_basis(double flexBasis)
{
    YGNodeStyleSetFlexBasis(m_node, flexBasis);
}

void Flexbox::set_flex_basis_percent(double flexBasis)
{
    YGNodeStyleSetFlexBasisPercent(m_node, flexBasis);
}

void Flexbox::set_flex_basis_auto()
{
    YGNodeStyleSetFlexBasisAuto(m_node);
}

void Flexbox::set_flex_grow(double flexGrow)
{
    YGNodeStyleSetFlexGrow(m_node, flexGrow);
}

void Flexbox::set_flex_shrink(double flexShrink)
{
    YGNodeStyleSetFlexShrink(m_node, flexShrink);
}
//
void Flexbox::set_width(double width)
{
    // GODOT_LOG(0, "Flexbox::set_width " + String::num_real(width));
    YGNodeStyleSetWidth(m_node, width);
}

void Flexbox::set_width_percent(double width)
{
    YGNodeStyleSetWidthPercent(m_node, width);
}

void Flexbox::set_width_auto()
{
    YGNodeStyleSetWidthAuto(m_node);
}

void Flexbox::set_height(double height)
{
    // GODOT_LOG(0, "Flexbox::set_height " + String::num_real(height));
    YGNodeStyleSetHeight(m_node, height);
}

void Flexbox::set_height_percent(double height)
{
    YGNodeStyleSetHeightPercent(m_node, height);
}

void Flexbox::set_height_auto()
{
    YGNodeStyleSetHeightAuto(m_node);
}
//

void Flexbox::set_min_width(double minWidth)
{
    YGNodeStyleSetMinWidth(m_node, minWidth);
}

void Flexbox::set_min_width_percent(double minWidth)
{
    YGNodeStyleSetMinWidthPercent(m_node, minWidth);
}

void Flexbox::set_min_height(double minHeight)
{
    YGNodeStyleSetMinHeight(m_node, minHeight);
}

void Flexbox::set_min_height_percent(double minHeight)
{
    YGNodeStyleSetMinHeightPercent(m_node, minHeight);
}
//

void Flexbox::set_max_width(double maxWidth)
{
    YGNodeStyleSetMaxWidth(m_node, maxWidth);
}

void Flexbox::set_max_width_percent(double maxWidth)
{
    YGNodeStyleSetMaxWidthPercent(m_node, maxWidth);
}

void Flexbox::set_max_height(double maxHeight)
{
    YGNodeStyleSetMaxHeight(m_node, maxHeight);
}

void Flexbox::set_max_height_percent(double maxHeight)
{
    YGNodeStyleSetMaxHeightPercent(m_node, maxHeight);
}
//
void Flexbox::set_aspect_ratio(double aspectRatio)
{
    YGNodeStyleSetAspectRatio(m_node, aspectRatio);
}

void Flexbox::set_border(int edge, double border)
{
    YGNodeStyleSetBorder(m_node, static_cast<YGEdge>(edge), border);
}

void Flexbox::set_padding(int edge, double padding)
{
    YGNodeStyleSetPadding(m_node, static_cast<YGEdge>(edge), padding);
}

void Flexbox::set_padding_percent(int edge, double padding)
{
    YGNodeStyleSetPaddingPercent(m_node, static_cast<YGEdge>(edge), padding);
}

void Flexbox::set_gap(int gutter, double gapLength)
{
    YGNodeStyleSetGap(m_node, static_cast<YGGutter>(gutter), gapLength);
}

//====================getter============================
int Flexbox::get_position_type(void) const
{
    return YGNodeStyleGetPositionType(m_node);
}

Variant Flexbox::get_position(int edge) const
{
    return fromYGValue(
        YGNodeStyleGetPosition(m_node, static_cast<YGEdge>(edge)));
}
//
int Flexbox::get_align_content(void) const
{
    return YGNodeStyleGetAlignContent(m_node);
}

int Flexbox::get_align_items(void) const
{
    return YGNodeStyleGetAlignItems(m_node);
}

int Flexbox::get_align_self(void) const
{
    return YGNodeStyleGetAlignSelf(m_node);
}

int Flexbox::get_flex_direction(void) const
{
    return YGNodeStyleGetFlexDirection(m_node);
}

int Flexbox::get_flex_wrap(void) const
{
    return YGNodeStyleGetFlexWrap(m_node);
}

int Flexbox::get_justify_content(void) const
{
    return YGNodeStyleGetJustifyContent(m_node);
}
//

Variant Flexbox::get_margin(int edge) const
{
    return fromYGValue(
        YGNodeStyleGetMargin(m_node, static_cast<YGEdge>(edge)));
}

int Flexbox::get_overflow(void) const
{
    return YGNodeStyleGetOverflow(m_node);
}

int Flexbox::get_display(void) const
{
    return YGNodeStyleGetDisplay(m_node);
}

Variant Flexbox::get_flex_basis(void) const
{
    return fromYGValue(YGNodeStyleGetFlexBasis(m_node));
}

double Flexbox::get_flex_grow(void) const
{
    return YGNodeStyleGetFlexGrow(m_node);
}

double Flexbox::get_flex_shrink(void) const
{
    return YGNodeStyleGetFlexShrink(m_node);
}
//

Variant Flexbox::get_width(void) const
{
    return fromYGValue(YGNodeStyleGetWidth(m_node));
}

Variant Flexbox::get_height(void) const
{
    return fromYGValue(YGNodeStyleGetHeight(m_node));
}

Variant Flexbox::get_min_width(void) const
{
    return fromYGValue(YGNodeStyleGetMinWidth(m_node));
}

Variant Flexbox::get_min_height(void) const
{
    return fromYGValue(YGNodeStyleGetMinHeight(m_node));
}

Variant Flexbox::get_max_width(void) const
{
    return fromYGValue(YGNodeStyleGetMaxWidth(m_node));
}

Variant Flexbox::get_max_height(void) const
{
    return fromYGValue(YGNodeStyleGetMaxHeight(m_node));
}
//

double Flexbox::get_aspect_ratio(void) const
{
    return YGNodeStyleGetAspectRatio(m_node);
}

double Flexbox::get_border(int edge) const
{
    return YGNodeStyleGetBorder(m_node, static_cast<YGEdge>(edge));
}

Variant Flexbox::get_padding(int edge) const
{
    return fromYGValue(
        YGNodeStyleGetPadding(m_node, static_cast<YGEdge>(edge)));
}

float Flexbox::get_gap(int gutter)
{
    return YGNodeStyleGetGap(m_node, static_cast<YGGutter>(gutter));
}
//

void Flexbox::insert_child(Flexbox *child, unsigned index)
{
    YGNodeInsertChild(m_node, child->m_node, index);
}

void Flexbox::remove_child(Flexbox *child)
{
    YGNodeRemoveChild(m_node, child->m_node);
}

unsigned Flexbox::get_child_count(void) const
{
    return YGNodeGetChildCount(m_node);
}

Flexbox *Flexbox::get_parent(void)
{
    auto nodePtr = YGNodeGetParent(m_node);

    if (nodePtr == nullptr)
        return nullptr;

    return Flexbox::fromYGNode(nodePtr);
}

Flexbox *Flexbox::get_child(unsigned index)
{
    auto nodePtr = YGNodeGetChild(m_node, index);

    if (nodePtr == nullptr)
        return nullptr;

    return Flexbox::fromYGNode(nodePtr);
}
//

void Flexbox::calculate_layout(double width, double height, int direction)
{
    YGNodeCalculateLayout(
        m_node, width, height, static_cast<YGDirection>(direction));
}

// void Flexbox::calculate_layout()
// {
//     YGNodeCalculateLayout(
//         m_node, YGUndefined, YGUndefined, YGDirectionLTR);
// }

double Flexbox::get_computed_left(void) const
{
    return YGNodeLayoutGetLeft(m_node);
}

double Flexbox::get_computed_right(void) const
{
    return YGNodeLayoutGetRight(m_node);
}

double Flexbox::get_computed_top(void) const
{
    return YGNodeLayoutGetTop(m_node);
}

double Flexbox::get_computed_bottom(void) const
{
    return YGNodeLayoutGetBottom(m_node);
}

double Flexbox::get_computed_width(void) const
{
    return YGNodeLayoutGetWidth(m_node);
}

double Flexbox::get_computed_height(void) const
{
    return YGNodeLayoutGetHeight(m_node);
}

Variant Flexbox::get_computed_layout(void) const
{
    Dictionary layout;

    layout["left"] = YGNodeLayoutGetLeft(m_node);
    layout["right"] = YGNodeLayoutGetRight(m_node);

    layout["top"] = YGNodeLayoutGetTop(m_node);
    layout["bottom"] = YGNodeLayoutGetBottom(m_node);

    layout["width"] = YGNodeLayoutGetWidth(m_node);
    layout["height"] = YGNodeLayoutGetHeight(m_node);

    return layout;
}

double Flexbox::get_computed_margin(int edge) const
{
    return YGNodeLayoutGetMargin(m_node, static_cast<YGEdge>(edge));
}

double Flexbox::get_computed_border(int edge) const
{
    return YGNodeLayoutGetBorder(m_node, static_cast<YGEdge>(edge));
}

double Flexbox::get_computed_padding(int edge) const
{
    return YGNodeLayoutGetPadding(m_node, static_cast<YGEdge>(edge));
}
//====================================================

void Flexbox::set_is_reference_baseline(bool isReferenceBaseline)
{
    YGNodeSetIsReferenceBaseline(m_node, isReferenceBaseline);
}
bool Flexbox::is_reference_baseline()
{
    return YGNodeIsReferenceBaseline(m_node);
}

void Flexbox::set_measure_func(Ref<FuncRef> funcRef)
{
    m_measureFunc = funcRef;

    YGNodeSetMeasureFunc(m_node, &globalMeasureFunc);
}

void Flexbox::unset_measure_func(void)
{
    m_measureFunc.unref();

    YGNodeSetMeasureFunc(m_node, nullptr);
}

Dictionary Flexbox::call_measure_func(
    double width,
    int widthMode,
    double height,
    int heightMode)
{
    if (!m_measureFunc->is_valid())
    {
        GODOT_LOG(2, "Measure call Error: Supplied function reference is invalid! Aborting callback...")
        return Variant();
    }
    Array argument_array = Array();
    argument_array.append(width);
    argument_array.append(widthMode);
    argument_array.append(height);
    argument_array.append(heightMode);
    Dictionary size = m_measureFunc->call_funcv(argument_array); // cast to Dictionary
    return size;
}
//
void Flexbox::call_dirtied_func(void)
{
    if (!m_dirtiedFunc->is_valid())
    {
        GODOT_LOG(2, "Dirtied Error: Supplied function reference is invalid! Aborting callback...")
        return;
    }
    Array argument_array = Array();
    m_dirtiedFunc->call_func(argument_array);
}

void Flexbox::set_dirtied_func(Ref<FuncRef> funcRef)
{
    m_dirtiedFunc = funcRef;

    YGNodeSetDirtiedFunc(m_node, &globalDirtiedFunc);
}

void Flexbox::unset_dirtied_func(void)
{
    m_dirtiedFunc.unref();

    YGNodeSetDirtiedFunc(m_node, nullptr);
}

void Flexbox::mark_dirty(void)
{
    YGNodeMarkDirty(m_node);
}

bool Flexbox::is_dirty(void) const
{
    return YGNodeIsDirty(m_node);
}

void Flexbox::_register_methods()
{
    // register_method("create", &Flexbox::createDefault);
    // register_method("createWithConfig", &Flexbox::createWithConfig);
    // GODOT_LOG(0, "Flexbox::_register_methods");
    // register_property<Flexbox, int>("position_type", &Flexbox::set_position_type, &Flexbox::get_position_type, YGPositionTypeStatic);
    // register_method("destroy", &Flexbox::destroy);
    register_method("copy_style", &Flexbox::copy_style);
    register_method("set_position_type", &Flexbox::set_position_type);
    register_method("set_position", &Flexbox::set_position);
    register_method("set_position_percent", &Flexbox::set_position_percent);
    register_method("set_align_content", &Flexbox::set_align_content);
    register_method("set_align_items", &Flexbox::set_align_items);
    register_method("set_align_self", &Flexbox::set_align_self);
    register_method("set_flex_direction", &Flexbox::set_flex_direction);
    register_method("set_flex_wrap", &Flexbox::set_flex_wrap);
    register_method("set_justify_content", &Flexbox::set_justify_content);

    register_method("set_margin", &Flexbox::set_margin);
    register_method("set_margin_percent", &Flexbox::set_margin_percent);
    register_method("set_margin_auto", &Flexbox::set_margin_auto);
    register_method("set_overflow", &Flexbox::set_overflow);
    register_method("set_display", &Flexbox::set_display);

    register_method("set_flex", &Flexbox::set_flex);
    register_method("set_flex_basis", &Flexbox::set_flex_basis);
    register_method("set_flex_basis_percent", &Flexbox::set_flex_basis_percent);
    register_method("set_flex_basis_auto", &Flexbox::set_flex_basis_auto);
    register_method("set_flex_grow", &Flexbox::set_flex_grow);
    register_method("set_flex_shrink", &Flexbox::set_flex_shrink);

    register_method("set_width", &Flexbox::set_width);
    register_method("set_width_percent", &Flexbox::set_width_percent);
    register_method("set_width_auto", &Flexbox::set_width_auto);
    register_method("set_height", &Flexbox::set_height);
    register_method("set_height_percent", &Flexbox::set_height_percent);
    register_method("set_height_auto", &Flexbox::set_height_auto);

    register_method("set_min_width", &Flexbox::set_min_width);
    register_method("set_min_width_percent", &Flexbox::set_min_width_percent);
    register_method("set_min_height", &Flexbox::set_min_height);
    register_method("set_min_height_percent", &Flexbox::set_min_height_percent);

    register_method("set_max_width", &Flexbox::set_max_width);
    register_method("set_max_width_percent", &Flexbox::set_max_width_percent);
    register_method("set_max_height", &Flexbox::set_max_height);
    register_method("set_max_height_percent", &Flexbox::set_max_height_percent);

    register_method("set_aspect_ratio", &Flexbox::set_aspect_ratio);
    register_method("set_border", &Flexbox::set_border);
    register_method("set_padding", &Flexbox::set_padding);
    register_method("set_padding_percent", &Flexbox::set_padding_percent);
    register_method("set_gap", &Flexbox::set_gap);

    register_method("set_is_reference_baseline", &Flexbox::set_is_reference_baseline);

    register_method("get_position_type", &Flexbox::get_position_type);
    register_method("get_position", &Flexbox::get_position);
    register_method("get_align_content", &Flexbox::get_align_content);
    register_method("get_align_items", &Flexbox::get_align_items);
    register_method("get_align_self", &Flexbox::get_align_self);
    register_method("get_flex_direction", &Flexbox::get_flex_direction);
    register_method("get_flex_wrap", &Flexbox::get_flex_wrap);
    register_method("get_justify_content", &Flexbox::get_justify_content);

    register_method("get_margin", &Flexbox::get_margin);
    register_method("get_overflow", &Flexbox::get_overflow);
    register_method("get_display", &Flexbox::get_display);
    register_method("get_flex_basis", &Flexbox::get_flex_basis);
    register_method("get_flex_grow", &Flexbox::get_flex_grow);
    register_method("get_flex_shrink", &Flexbox::get_flex_shrink);
    register_method("get_width", &Flexbox::get_width);
    register_method("get_height", &Flexbox::get_height);
    register_method("get_min_width", &Flexbox::get_min_width);
    register_method("get_min_height", &Flexbox::get_min_height);
    register_method("get_max_width", &Flexbox::get_max_width);
    register_method("get_max_height", &Flexbox::get_max_height);
    register_method("get_aspect_ratio", &Flexbox::get_aspect_ratio);
    register_method("get_border", &Flexbox::get_border);
    register_method("get_padding", &Flexbox::get_padding);
    register_method("get_gap", &Flexbox::get_gap);
    register_method("is_reference_baseline", &Flexbox::is_reference_baseline);

    register_method("insert_child", &Flexbox::insert_child);
    register_method("remove_child", &Flexbox::remove_child);
    register_method("get_child_count", &Flexbox::get_child_count);
    register_method("get_child", &Flexbox::get_child);

    register_method("set_measure_func", &Flexbox::set_measure_func);
    register_method("unset_measure_func", &Flexbox::unset_measure_func);
    register_method("set_dirtied_func", &Flexbox::set_dirtied_func);
    register_method("unset_dirtied_func", &Flexbox::unset_dirtied_func);
    register_method("mark_dirty", &Flexbox::mark_dirty);
    register_method("is_dirty", &Flexbox::is_dirty);

    register_method("calculate_layout", &Flexbox::calculate_layout);
    register_method("get_computed_left", &Flexbox::get_computed_left);
    register_method("get_computed_right", &Flexbox::get_computed_right);
    register_method("get_computed_top", &Flexbox::get_computed_top);
    register_method("get_computed_bottom", &Flexbox::get_computed_bottom);
    register_method("get_computed_width", &Flexbox::get_computed_width);
    register_method("get_computed_height", &Flexbox::get_computed_height);
    register_method("get_computed_layout", &Flexbox::get_computed_layout);

    register_method("get_computed_margin", &Flexbox::get_computed_margin);
    register_method("get_computed_padding", &Flexbox::get_computed_padding);
    register_method("get_computed_border", &Flexbox::get_computed_border);
}
