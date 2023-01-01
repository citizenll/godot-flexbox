/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <algorithm>

#include <yoga/Yoga.h>

#include "./YGNode.hh"
#include "./Layout.hh"
#include "./Size.hh"
#include "./YGConfig.hh"

static YGSize globalMeasureFunc(
    YGNodeRef nodeRef,
    float width,
    YGMeasureMode widthMode,
    float height,
    YGMeasureMode heightMode) {
  YGNode const& node = *reinterpret_cast<YGNode const*>(YGNodeGetContext(nodeRef));

  Size size = node.callMeasureFunc(width, widthMode, height, heightMode);
  YGSize ygSize = {
      static_cast<float>(size.width), static_cast<float>(size.height)};

  return ygSize;
}

static void globalDirtiedFunc(YGNodeRef nodeRef) {
  YGNode const& node = *reinterpret_cast<YGNode const*>(YGNodeGetContext(nodeRef));

  node.callDirtiedFunc();
}

/* static */ YGNode* YGNode::createDefault(void) {
  return new YGNode(nullptr);
}

/* static */ YGNode* YGNode::createWithConfig(YGConfig* config) {
  return new YGNode(config);
}

/* static */ void YGNode::destroy(YGNode* node) {
  delete node;
}

/* static */ YGNode* YGNode::fromYGNode(YGNodeRef nodeRef) {
  return reinterpret_cast<YGNode*>(YGNodeGetContext(nodeRef));
}

YGNode::YGNode(YGConfig* config)
    : m_node(
          config != nullptr ? YGNodeNewWithConfig(config->m_config)
                            : YGNodeNew()),
      m_measureFunc(nullptr),
      m_dirtiedFunc(nullptr) {
  YGNodeSetContext(m_node, reinterpret_cast<void*>(this));
}

YGNode::~YGNode(void) {
  YGNodeFree(m_node);
}

void YGNode::reset(void) {
  m_measureFunc.reset(nullptr);
  m_dirtiedFunc.reset(nullptr);

  YGNodeReset(m_node);
}

void YGNode::copyStyle(YGNode const& other) {
  YGNodeCopyStyle(m_node, other.m_node);
}

void YGNode::setPositionType(int positionType) {
  YGNodeStyleSetPositionType(m_node, static_cast<YGPositionType>(positionType));
}

void YGNode::setPosition(int edge, double position) {
  YGNodeStyleSetPosition(m_node, static_cast<YGEdge>(edge), position);
}

void YGNode::setPositionPercent(int edge, double position) {
  YGNodeStyleSetPositionPercent(m_node, static_cast<YGEdge>(edge), position);
}

void YGNode::setAlignContent(int alignContent) {
  YGNodeStyleSetAlignContent(m_node, static_cast<YGAlign>(alignContent));
}

void YGNode::setAlignItems(int alignItems) {
  YGNodeStyleSetAlignItems(m_node, static_cast<YGAlign>(alignItems));
}

void YGNode::setAlignSelf(int alignSelf) {
  YGNodeStyleSetAlignSelf(m_node, static_cast<YGAlign>(alignSelf));
}

void YGNode::setFlexDirection(int flexDirection) {
  YGNodeStyleSetFlexDirection(
      m_node, static_cast<YGFlexDirection>(flexDirection));
}

void YGNode::setFlexWrap(int flexWrap) {
  YGNodeStyleSetFlexWrap(m_node, static_cast<YGWrap>(flexWrap));
}

void YGNode::setJustifyContent(int justifyContent) {
  YGNodeStyleSetJustifyContent(m_node, static_cast<YGJustify>(justifyContent));
}

void YGNode::setMargin(int edge, double margin) {
  YGNodeStyleSetMargin(m_node, static_cast<YGEdge>(edge), margin);
}

void YGNode::setMarginPercent(int edge, double margin) {
  YGNodeStyleSetMarginPercent(m_node, static_cast<YGEdge>(edge), margin);
}

void YGNode::setMarginAuto(int edge) {
  YGNodeStyleSetMarginAuto(m_node, static_cast<YGEdge>(edge));
}

void YGNode::setOverflow(int overflow) {
  YGNodeStyleSetOverflow(m_node, static_cast<YGOverflow>(overflow));
}

void YGNode::setDisplay(int display) {
  YGNodeStyleSetDisplay(m_node, static_cast<YGDisplay>(display));
}

void YGNode::setFlex(double flex) {
  YGNodeStyleSetFlex(m_node, flex);
}

void YGNode::setFlexBasis(double flexBasis) {
  YGNodeStyleSetFlexBasis(m_node, flexBasis);
}

void YGNode::setFlexBasisPercent(double flexBasis) {
  YGNodeStyleSetFlexBasisPercent(m_node, flexBasis);
}

void YGNode::setFlexBasisAuto() {
  YGNodeStyleSetFlexBasisAuto(m_node);
}

void YGNode::setFlexGrow(double flexGrow) {
  YGNodeStyleSetFlexGrow(m_node, flexGrow);
}

void YGNode::setFlexShrink(double flexShrink) {
  YGNodeStyleSetFlexShrink(m_node, flexShrink);
}

void YGNode::setWidth(double width) {
  YGNodeStyleSetWidth(m_node, width);
}

void YGNode::setWidthPercent(double width) {
  YGNodeStyleSetWidthPercent(m_node, width);
}

void YGNode::setWidthAuto() {
  YGNodeStyleSetWidthAuto(m_node);
}

void YGNode::setHeight(double height) {
  YGNodeStyleSetHeight(m_node, height);
}

void YGNode::setHeightPercent(double height) {
  YGNodeStyleSetHeightPercent(m_node, height);
}

void YGNode::setHeightAuto() {
  YGNodeStyleSetHeightAuto(m_node);
}

void YGNode::setMinWidth(double minWidth) {
  YGNodeStyleSetMinWidth(m_node, minWidth);
}

void YGNode::setMinWidthPercent(double minWidth) {
  YGNodeStyleSetMinWidthPercent(m_node, minWidth);
}

void YGNode::setMinHeight(double minHeight) {
  YGNodeStyleSetMinHeight(m_node, minHeight);
}

void YGNode::setMinHeightPercent(double minHeight) {
  YGNodeStyleSetMinHeightPercent(m_node, minHeight);
}

void YGNode::setMaxWidth(double maxWidth) {
  YGNodeStyleSetMaxWidth(m_node, maxWidth);
}

void YGNode::setMaxWidthPercent(double maxWidth) {
  YGNodeStyleSetMaxWidthPercent(m_node, maxWidth);
}

void YGNode::setMaxHeight(double maxHeight) {
  YGNodeStyleSetMaxHeight(m_node, maxHeight);
}

void YGNode::setMaxHeightPercent(double maxHeight) {
  YGNodeStyleSetMaxHeightPercent(m_node, maxHeight);
}

void YGNode::setAspectRatio(double aspectRatio) {
  YGNodeStyleSetAspectRatio(m_node, aspectRatio);
}

void YGNode::setBorder(int edge, double border) {
  YGNodeStyleSetBorder(m_node, static_cast<YGEdge>(edge), border);
}

void YGNode::setPadding(int edge, double padding) {
  YGNodeStyleSetPadding(m_node, static_cast<YGEdge>(edge), padding);
}

void YGNode::setPaddingPercent(int edge, double padding) {
  YGNodeStyleSetPaddingPercent(m_node, static_cast<YGEdge>(edge), padding);
}

void YGNode::setIsReferenceBaseline(bool isReferenceBaseline) {
  YGNodeSetIsReferenceBaseline(m_node, isReferenceBaseline);
}

void YGNode::setGap(int gutter, double gapLength) {
  YGNodeStyleSetGap(m_node, static_cast<YGGutter>(gutter), gapLength);
}

int YGNode::getPositionType(void) const {
  return YGNodeStyleGetPositionType(m_node);
}

Value YGNode::getPosition(int edge) const {
  return Value::fromYGValue(
      YGNodeStyleGetPosition(m_node, static_cast<YGEdge>(edge)));
}

int YGNode::getAlignContent(void) const {
  return YGNodeStyleGetAlignContent(m_node);
}

int YGNode::getAlignItems(void) const {
  return YGNodeStyleGetAlignItems(m_node);
}

int YGNode::getAlignSelf(void) const {
  return YGNodeStyleGetAlignSelf(m_node);
}

int YGNode::getFlexDirection(void) const {
  return YGNodeStyleGetFlexDirection(m_node);
}

int YGNode::getFlexWrap(void) const {
  return YGNodeStyleGetFlexWrap(m_node);
}

int YGNode::getJustifyContent(void) const {
  return YGNodeStyleGetJustifyContent(m_node);
}

Value YGNode::getMargin(int edge) const {
  return Value::fromYGValue(
      YGNodeStyleGetMargin(m_node, static_cast<YGEdge>(edge)));
}

int YGNode::getOverflow(void) const {
  return YGNodeStyleGetOverflow(m_node);
}

int YGNode::getDisplay(void) const {
  return YGNodeStyleGetDisplay(m_node);
}

Value YGNode::getFlexBasis(void) const {
  return Value::fromYGValue(YGNodeStyleGetFlexBasis(m_node));
}

double YGNode::getFlexGrow(void) const {
  return YGNodeStyleGetFlexGrow(m_node);
}

double YGNode::getFlexShrink(void) const {
  return YGNodeStyleGetFlexShrink(m_node);
}

Value YGNode::getWidth(void) const {
  return Value::fromYGValue(YGNodeStyleGetWidth(m_node));
}

Value YGNode::getHeight(void) const {
  return Value::fromYGValue(YGNodeStyleGetHeight(m_node));
}

Value YGNode::getMinWidth(void) const {
  return Value::fromYGValue(YGNodeStyleGetMinWidth(m_node));
}

Value YGNode::getMinHeight(void) const {
  return Value::fromYGValue(YGNodeStyleGetMinHeight(m_node));
}

Value YGNode::getMaxWidth(void) const {
  return Value::fromYGValue(YGNodeStyleGetMaxWidth(m_node));
}

Value YGNode::getMaxHeight(void) const {
  return Value::fromYGValue(YGNodeStyleGetMaxHeight(m_node));
}

double YGNode::getAspectRatio(void) const {
  return YGNodeStyleGetAspectRatio(m_node);
}

double YGNode::getBorder(int edge) const {
  return YGNodeStyleGetBorder(m_node, static_cast<YGEdge>(edge));
}

Value YGNode::getPadding(int edge) const {
  return Value::fromYGValue(
      YGNodeStyleGetPadding(m_node, static_cast<YGEdge>(edge)));
}

float YGNode::getGap(int gutter) {
  return YGNodeStyleGetGap(m_node, static_cast<YGGutter>(gutter));
}

bool YGNode::isReferenceBaseline() {
  return YGNodeIsReferenceBaseline(m_node);
}

void YGNode::insertChild(YGNode* child, unsigned index) {
  YGNodeInsertChild(m_node, child->m_node, index);
}

void YGNode::removeChild(YGNode* child) {
  YGNodeRemoveChild(m_node, child->m_node);
}

unsigned YGNode::getChildCount(void) const {
  return YGNodeGetChildCount(m_node);
}

YGNode* YGNode::getParent(void) {
  auto nodePtr = YGNodeGetParent(m_node);

  if (nodePtr == nullptr)
    return nullptr;

  return YGNode::fromYGNode(nodePtr);
}

YGNode* YGNode::getChild(unsigned index) {
  auto nodePtr = YGNodeGetChild(m_node, index);

  if (nodePtr == nullptr)
    return nullptr;

  return YGNode::fromYGNode(nodePtr);
}

void YGNode::setMeasureFunc(MeasureCallback* measureFunc) {
  m_measureFunc.reset(measureFunc);

  YGNodeSetMeasureFunc(m_node, &globalMeasureFunc);
}

void YGNode::unsetMeasureFunc(void) {
  m_measureFunc.reset(nullptr);

  YGNodeSetMeasureFunc(m_node, nullptr);
}

Size YGNode::callMeasureFunc(
    double width,
    int widthMode,
    double height,
    int heightMode) const {
  return m_measureFunc->measure(width, widthMode, height, heightMode);
}

void YGNode::setDirtiedFunc(DirtiedCallback* dirtiedFunc) {
  m_dirtiedFunc.reset(dirtiedFunc);

  YGNodeSetDirtiedFunc(m_node, &globalDirtiedFunc);
}

void YGNode::unsetDirtiedFunc(void) {
  m_dirtiedFunc.reset(nullptr);

  YGNodeSetDirtiedFunc(m_node, nullptr);
}

void YGNode::callDirtiedFunc(void) const {
  m_dirtiedFunc->dirtied();
}

void YGNode::markDirty(void) {
  YGNodeMarkDirty(m_node);
}

bool YGNode::isDirty(void) const {
  return YGNodeIsDirty(m_node);
}

void YGNode::calculateLayout(double width, double height, int direction) {
  YGNodeCalculateLayout(
      m_node, width, height, static_cast<YGDirection>(direction));
}

double YGNode::getComputedLeft(void) const {
  return YGNodeLayoutGetLeft(m_node);
}

double YGNode::getComputedRight(void) const {
  return YGNodeLayoutGetRight(m_node);
}

double YGNode::getComputedTop(void) const {
  return YGNodeLayoutGetTop(m_node);
}

double YGNode::getComputedBottom(void) const {
  return YGNodeLayoutGetBottom(m_node);
}

double YGNode::getComputedWidth(void) const {
  return YGNodeLayoutGetWidth(m_node);
}

double YGNode::getComputedHeight(void) const {
  return YGNodeLayoutGetHeight(m_node);
}

Layout YGNode::getComputedLayout(void) const {
  Layout layout;

  layout.left = YGNodeLayoutGetLeft(m_node);
  layout.right = YGNodeLayoutGetRight(m_node);

  layout.top = YGNodeLayoutGetTop(m_node);
  layout.bottom = YGNodeLayoutGetBottom(m_node);

  layout.width = YGNodeLayoutGetWidth(m_node);
  layout.height = YGNodeLayoutGetHeight(m_node);

  return layout;
}

double YGNode::getComputedMargin(int edge) const {
  return YGNodeLayoutGetMargin(m_node, static_cast<YGEdge>(edge));
}

double YGNode::getComputedBorder(int edge) const {
  return YGNodeLayoutGetBorder(m_node, static_cast<YGEdge>(edge));
}

double YGNode::getComputedPadding(int edge) const {
  return YGNodeLayoutGetPadding(m_node, static_cast<YGEdge>(edge));
}
