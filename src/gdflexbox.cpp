#include "gdflexbox.h"

using namespace godot;

void Flexbox::_register_methods()
{
    GODOT_LOG(0, "Flexbox::_register_methods");
    register_property<Flexbox, int>("position_type", &Flexbox::setPositionType, &Flexbox::getPositionType, YGPositionTypeStatic);
    register_method("create", &Flexbox::createDefault);
    register_method("createWithConfig", &Flexbox::createWithConfig);
    register_method("destroy", &Flexbox::destroy);
    register_method("copyStyle", &Flexbox::copyStyle);
    register_method("setPositionType", &Flexbox::setPositionType);
    register_method("setPosition", &Flexbox::setPosition);
    register_method("setPositionPercent", &Flexbox::setPositionPercent);
    register_method("setAlignContent", &Flexbox::setAlignContent);
    register_method("setAlignItems", &Flexbox::setAlignItems);
    register_method("setAlignSelf", &Flexbox::setAlignSelf);
    register_method("setFlexDirection", &Flexbox::setFlexDirection);
    register_method("setFlexWrap", &Flexbox::setFlexWrap);
    register_method("setJustifyContent", &Flexbox::setJustifyContent);
    register_method("setMargin", &Flexbox::setMargin);
    register_method("setMarginPercent", &Flexbox::setMarginPercent);
    register_method("setMarginAuto", &Flexbox::setMarginAuto);
    register_method("setOverflow", &Flexbox::setOverflow);
    register_method("setDisplay", &Flexbox::setDisplay);
    register_method("setFlex", &Flexbox::setFlex);
    register_method("setFlexBasis", &Flexbox::setFlexBasis);
    register_method("setFlexBasisPercent", &Flexbox::setFlexBasisPercent);
    register_method("setFlexBasisAuto", &Flexbox::setFlexBasisAuto);
    register_method("setFlexGrow", &Flexbox::setFlexGrow);
    register_method("setFlexShrink", &Flexbox::setFlexShrink);
    register_method("setWidth", &Flexbox::setWidth);
    register_method("setWidthPercent", &Flexbox::setWidthPercent);
    register_method("setWidthAuto", &Flexbox::setWidthAuto);
    register_method("setHeight", &Flexbox::setHeight);
    register_method("setHeightPercent", &Flexbox::setHeightPercent);
    register_method("setHeightAuto", &Flexbox::setHeightAuto);
    register_method("setMinWidth", &Flexbox::setMinWidth);
    register_method("setMinWidthPercent", &Flexbox::setMinWidthPercent);
    register_method("setMinHeight", &Flexbox::setMinHeight);
    register_method("setMinHeightPercent", &Flexbox::setMinHeightPercent);
    register_method("setMaxWidth", &Flexbox::setMaxWidth);
    register_method("setMaxWidthPercent", &Flexbox::setMaxWidthPercent);
    register_method("setMaxHeight", &Flexbox::setMaxHeight);
    register_method("setMaxHeightPercent", &Flexbox::setMaxHeightPercent);
    register_method("setAspectRatio", &Flexbox::setAspectRatio);
    register_method("setBorder", &Flexbox::setBorder);
    register_method("setPadding", &Flexbox::setPadding);
    register_method("setPaddingPercent", &Flexbox::setPaddingPercent);
    register_method("setIsReferenceBaseline", &Flexbox::setIsReferenceBaseline);
    register_method("setGap", &Flexbox::setGap);
    register_method("getPositionType", &Flexbox::getPositionType);
    register_method("getPosition", &Flexbox::getPosition);
    register_method("getAlignContent", &Flexbox::getAlignContent);
    register_method("getAlignItems", &Flexbox::getAlignItems);
    register_method("getAlignSelf", &Flexbox::getAlignSelf);
    register_method("getFlexDirection", &Flexbox::getFlexDirection);
    register_method("getFlexWrap", &Flexbox::getFlexWrap);
    register_method("getJustifyContent", &Flexbox::getJustifyContent);
    register_method("getMargin", &Flexbox::getMargin);
    register_method("getOverflow", &Flexbox::getOverflow);
    register_method("getDisplay", &Flexbox::getDisplay);
    register_method("getFlexBasis", &Flexbox::getFlexBasis);
    register_method("getFlexGrow", &Flexbox::getFlexGrow);
    register_method("getFlexShrink", &Flexbox::getFlexShrink);
    register_method("getWidth", &Flexbox::getWidth);
    register_method("getHeight", &Flexbox::getHeight);
    register_method("getMinWidth", &Flexbox::getMinWidth);
    register_method("getMinHeight", &Flexbox::getMinHeight);
    register_method("getMaxWidth", &Flexbox::getMaxWidth);
    register_method("getMaxHeight", &Flexbox::getMaxHeight);
    register_method("getAspectRatio", &Flexbox::getAspectRatio);
    register_method("getBorder", &Flexbox::getBorder);
    register_method("getPadding", &Flexbox::getPadding);
    register_method("getGap", &Flexbox::getGap);
    register_method("isReferenceBaseline", &Flexbox::isReferenceBaseline);
    register_method("insertChild", &Flexbox::insertChild);
    register_method("removeChild", &Flexbox::removeChild);
    register_method("getChildCount", &Flexbox::getChildCount);
    register_method("getChild", &Flexbox::getChild);
    register_method("setMeasureFunc", &Flexbox::setMeasureFunc);
    register_method("unsetMeasureFunc", &Flexbox::unsetMeasureFunc);
    register_method("setDirtiedFunc", &Flexbox::setDirtiedFunc);
    register_method("unsetDirtiedFunc", &Flexbox::unsetDirtiedFunc);
    register_method("markDirty", &Flexbox::markDirty);
    register_method("isDirty", &Flexbox::isDirty);
    register_method("calculateLayout", &Flexbox::calculateLayout);
    register_method("getComputedLeft", &Flexbox::getComputedLeft);
    register_method("getComputedRight", &Flexbox::getComputedRight);
    register_method("getComputedTop", &Flexbox::getComputedTop);
    register_method("getComputedBottom", &Flexbox::getComputedBottom);
    register_method("getComputedWidth", &Flexbox::getComputedWidth);
    register_method("getComputedHeight", &Flexbox::getComputedHeight);
    register_method("getComputedLayout", &Flexbox::getComputedLayout);

    register_method("getComputedMargin", &Flexbox::getComputedMargin);
    register_method("getComputedPadding", &Flexbox::getComputedPadding);
    register_method("getComputedBorder", &Flexbox::getComputedBorder);

}
Flexbox::~Flexbox()
{
    GODOT_LOG(0, "Flexbox destructor");
}

void Flexbox::_init()
{
    GODOT_LOG(0, "Flexbox::_init");
}

void Flexbox::dirtied()
{
    GODOT_LOG(0, "Flexbox::dirtied");
}