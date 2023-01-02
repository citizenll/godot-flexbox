/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <algorithm>
#include "./FlexNode.hh"

static YGSize globalMeasureFunc(
    YGNodeRef nodeRef,
    float width,
    YGMeasureMode widthMode,
    float height,
    YGMeasureMode heightMode) {
  FlexNode const& node = *reinterpret_cast<FlexNode const*>(YGNodeGetContext(nodeRef));

  Size size = node.callMeasureFunc(width, widthMode, height, heightMode);
  YGSize ygSize = {
      static_cast<float>(size.width), static_cast<float>(size.height)};

  return ygSize;
}

static void globalDirtiedFunc(YGNodeRef nodeRef) {
  FlexNode const& node = *reinterpret_cast<FlexNode const*>(YGNodeGetContext(nodeRef));

  node.callDirtiedFunc();
}

/* static */ FlexNode* FlexNode::createDefault(void) {
  return new FlexNode(nullptr);
}

/* static */ FlexNode* FlexNode::createWithConfig(FlexConfig* config) {
  return new FlexNode(config);
}

/* static */ void FlexNode::destroy(FlexNode* node) {
  delete node;
}

/* static */ FlexNode* FlexNode::fromYGNode(YGNodeRef nodeRef) {
  return reinterpret_cast<FlexNode*>(YGNodeGetContext(nodeRef));
}

FlexNode::FlexNode(FlexConfig* config)
    : m_node(
          config != nullptr ? YGNodeNewWithConfig(config->m_config)
                            : YGNodeNew()),
      m_measureFunc(nullptr),
      m_dirtiedFunc(nullptr) {
  YGNodeSetContext(m_node, reinterpret_cast<void*>(this));
}

FlexNode::~FlexNode(void) {
  YGNodeFree(m_node);
}

void FlexNode::reset(void) {
  m_measureFunc.reset(nullptr);
  m_dirtiedFunc.reset(nullptr);

  YGNodeReset(m_node);
}

void FlexNode::copyStyle(FlexNode const& other) {
  YGNodeCopyStyle(m_node, other.m_node);
}

void FlexNode::setPositionType(int positionType) {
  YGNodeStyleSetPositionType(m_node, static_cast<YGPositionType>(positionType));
}

void FlexNode::setPosition(int edge, double position) {
  YGNodeStyleSetPosition(m_node, static_cast<YGEdge>(edge), position);
}

void FlexNode::setPositionPercent(int edge, double position) {
  YGNodeStyleSetPositionPercent(m_node, static_cast<YGEdge>(edge), position);
}

void FlexNode::setAlignContent(int alignContent) {
  YGNodeStyleSetAlignContent(m_node, static_cast<YGAlign>(alignContent));
}

void FlexNode::setAlignItems(int alignItems) {
  YGNodeStyleSetAlignItems(m_node, static_cast<YGAlign>(alignItems));
}

void FlexNode::setAlignSelf(int alignSelf) {
  YGNodeStyleSetAlignSelf(m_node, static_cast<YGAlign>(alignSelf));
}

void FlexNode::setFlexDirection(int flexDirection) {
  YGNodeStyleSetFlexDirection(
      m_node, static_cast<YGFlexDirection>(flexDirection));
}

void FlexNode::setFlexWrap(int flexWrap) {
  YGNodeStyleSetFlexWrap(m_node, static_cast<YGWrap>(flexWrap));
}

void FlexNode::setJustifyContent(int justifyContent) {
  YGNodeStyleSetJustifyContent(m_node, static_cast<YGJustify>(justifyContent));
}

void FlexNode::setMargin(int edge, double margin) {
  YGNodeStyleSetMargin(m_node, static_cast<YGEdge>(edge), margin);
}

void FlexNode::setMarginPercent(int edge, double margin) {
  YGNodeStyleSetMarginPercent(m_node, static_cast<YGEdge>(edge), margin);
}

void FlexNode::setMarginAuto(int edge) {
  YGNodeStyleSetMarginAuto(m_node, static_cast<YGEdge>(edge));
}

void FlexNode::setOverflow(int overflow) {
  YGNodeStyleSetOverflow(m_node, static_cast<YGOverflow>(overflow));
}

void FlexNode::setDisplay(int display) {
  YGNodeStyleSetDisplay(m_node, static_cast<YGDisplay>(display));
}

void FlexNode::setFlex(double flex) {
  YGNodeStyleSetFlex(m_node, flex);
}

void FlexNode::setFlexBasis(double flexBasis) {
  YGNodeStyleSetFlexBasis(m_node, flexBasis);
}

void FlexNode::setFlexBasisPercent(double flexBasis) {
  YGNodeStyleSetFlexBasisPercent(m_node, flexBasis);
}

void FlexNode::setFlexBasisAuto() {
  YGNodeStyleSetFlexBasisAuto(m_node);
}

void FlexNode::setFlexGrow(double flexGrow) {
  YGNodeStyleSetFlexGrow(m_node, flexGrow);
}

void FlexNode::setFlexShrink(double flexShrink) {
  YGNodeStyleSetFlexShrink(m_node, flexShrink);
}

void FlexNode::setWidth(double width) {
  YGNodeStyleSetWidth(m_node, width);
}

void FlexNode::setWidthPercent(double width) {
  YGNodeStyleSetWidthPercent(m_node, width);
}

void FlexNode::setWidthAuto() {
  YGNodeStyleSetWidthAuto(m_node);
}

void FlexNode::setHeight(double height) {
  YGNodeStyleSetHeight(m_node, height);
}

void FlexNode::setHeightPercent(double height) {
  YGNodeStyleSetHeightPercent(m_node, height);
}

void FlexNode::setHeightAuto() {
  YGNodeStyleSetHeightAuto(m_node);
}

void FlexNode::setMinWidth(double minWidth) {
  YGNodeStyleSetMinWidth(m_node, minWidth);
}

void FlexNode::setMinWidthPercent(double minWidth) {
  YGNodeStyleSetMinWidthPercent(m_node, minWidth);
}

void FlexNode::setMinHeight(double minHeight) {
  YGNodeStyleSetMinHeight(m_node, minHeight);
}

void FlexNode::setMinHeightPercent(double minHeight) {
  YGNodeStyleSetMinHeightPercent(m_node, minHeight);
}

void FlexNode::setMaxWidth(double maxWidth) {
  YGNodeStyleSetMaxWidth(m_node, maxWidth);
}

void FlexNode::setMaxWidthPercent(double maxWidth) {
  YGNodeStyleSetMaxWidthPercent(m_node, maxWidth);
}

void FlexNode::setMaxHeight(double maxHeight) {
  YGNodeStyleSetMaxHeight(m_node, maxHeight);
}

void FlexNode::setMaxHeightPercent(double maxHeight) {
  YGNodeStyleSetMaxHeightPercent(m_node, maxHeight);
}

void FlexNode::setAspectRatio(double aspectRatio) {
  YGNodeStyleSetAspectRatio(m_node, aspectRatio);
}

void FlexNode::setBorder(int edge, double border) {
  YGNodeStyleSetBorder(m_node, static_cast<YGEdge>(edge), border);
}

void FlexNode::setPadding(int edge, double padding) {
  YGNodeStyleSetPadding(m_node, static_cast<YGEdge>(edge), padding);
}

void FlexNode::setPaddingPercent(int edge, double padding) {
  YGNodeStyleSetPaddingPercent(m_node, static_cast<YGEdge>(edge), padding);
}

void FlexNode::setIsReferenceBaseline(bool isReferenceBaseline) {
  YGNodeSetIsReferenceBaseline(m_node, isReferenceBaseline);
}

void FlexNode::setGap(int gutter, double gapLength) {
  YGNodeStyleSetGap(m_node, static_cast<YGGutter>(gutter), gapLength);
}

int FlexNode::getPositionType(void) const {
  return YGNodeStyleGetPositionType(m_node);
}

Value FlexNode::getPosition(int edge) const {
  return Value::fromYGValue(
      YGNodeStyleGetPosition(m_node, static_cast<YGEdge>(edge)));
}

int FlexNode::getAlignContent(void) const {
  return YGNodeStyleGetAlignContent(m_node);
}

int FlexNode::getAlignItems(void) const {
  return YGNodeStyleGetAlignItems(m_node);
}

int FlexNode::getAlignSelf(void) const {
  return YGNodeStyleGetAlignSelf(m_node);
}

int FlexNode::getFlexDirection(void) const {
  return YGNodeStyleGetFlexDirection(m_node);
}

int FlexNode::getFlexWrap(void) const {
  return YGNodeStyleGetFlexWrap(m_node);
}

int FlexNode::getJustifyContent(void) const {
  return YGNodeStyleGetJustifyContent(m_node);
}

Value FlexNode::getMargin(int edge) const {
  return Value::fromYGValue(
      YGNodeStyleGetMargin(m_node, static_cast<YGEdge>(edge)));
}

int FlexNode::getOverflow(void) const {
  return YGNodeStyleGetOverflow(m_node);
}

int FlexNode::getDisplay(void) const {
  return YGNodeStyleGetDisplay(m_node);
}

Value FlexNode::getFlexBasis(void) const {
  return Value::fromYGValue(YGNodeStyleGetFlexBasis(m_node));
}

double FlexNode::getFlexGrow(void) const {
  return YGNodeStyleGetFlexGrow(m_node);
}

double FlexNode::getFlexShrink(void) const {
  return YGNodeStyleGetFlexShrink(m_node);
}

Value FlexNode::getWidth(void) const {
  return Value::fromYGValue(YGNodeStyleGetWidth(m_node));
}

Value FlexNode::getHeight(void) const {
  return Value::fromYGValue(YGNodeStyleGetHeight(m_node));
}

Value FlexNode::getMinWidth(void) const {
  return Value::fromYGValue(YGNodeStyleGetMinWidth(m_node));
}

Value FlexNode::getMinHeight(void) const {
  return Value::fromYGValue(YGNodeStyleGetMinHeight(m_node));
}

Value FlexNode::getMaxWidth(void) const {
  return Value::fromYGValue(YGNodeStyleGetMaxWidth(m_node));
}

Value FlexNode::getMaxHeight(void) const {
  return Value::fromYGValue(YGNodeStyleGetMaxHeight(m_node));
}

double FlexNode::getAspectRatio(void) const {
  return YGNodeStyleGetAspectRatio(m_node);
}

double FlexNode::getBorder(int edge) const {
  return YGNodeStyleGetBorder(m_node, static_cast<YGEdge>(edge));
}

Value FlexNode::getPadding(int edge) const {
  return Value::fromYGValue(
      YGNodeStyleGetPadding(m_node, static_cast<YGEdge>(edge)));
}

float FlexNode::getGap(int gutter) {
  return YGNodeStyleGetGap(m_node, static_cast<YGGutter>(gutter));
}

bool FlexNode::isReferenceBaseline() {
  return YGNodeIsReferenceBaseline(m_node);
}

void FlexNode::insertChild(FlexNode* child, unsigned index) {
  YGNodeInsertChild(m_node, child->m_node, index);
}

void FlexNode::removeChild(FlexNode* child) {
  YGNodeRemoveChild(m_node, child->m_node);
}

unsigned FlexNode::getChildCount(void) const {
  return YGNodeGetChildCount(m_node);
}

FlexNode* FlexNode::getParent(void) {
  auto nodePtr = YGNodeGetParent(m_node);

  if (nodePtr == nullptr)
    return nullptr;

  return FlexNode::fromYGNode(nodePtr);
}

FlexNode* FlexNode::getChild(unsigned index) {
  auto nodePtr = YGNodeGetChild(m_node, index);

  if (nodePtr == nullptr)
    return nullptr;

  return FlexNode::fromYGNode(nodePtr);
}

void FlexNode::setMeasureFunc(MeasureCallback* measureFunc) {
  m_measureFunc.reset(measureFunc);

  YGNodeSetMeasureFunc(m_node, &globalMeasureFunc);
}

void FlexNode::unsetMeasureFunc(void) {
  m_measureFunc.reset(nullptr);

  YGNodeSetMeasureFunc(m_node, nullptr);
}

Size FlexNode::callMeasureFunc(
    double width,
    int widthMode,
    double height,
    int heightMode) const {
  return m_measureFunc->measure(width, widthMode, height, heightMode);
}

void FlexNode::setDirtiedFunc(DirtiedCallback* dirtiedFunc) {
  m_dirtiedFunc.reset(dirtiedFunc);

  YGNodeSetDirtiedFunc(m_node, &globalDirtiedFunc);
}

void FlexNode::unsetDirtiedFunc(void) {
  m_dirtiedFunc.reset(nullptr);

  YGNodeSetDirtiedFunc(m_node, nullptr);
}

void FlexNode::callDirtiedFunc(void) const {
  m_dirtiedFunc->dirtied();
}

void FlexNode::markDirty(void) {
  YGNodeMarkDirty(m_node);
}

bool FlexNode::isDirty(void) const {
  return YGNodeIsDirty(m_node);
}

void FlexNode::calculateLayout(double width, double height, int direction) {
  YGNodeCalculateLayout(
      m_node, width, height, static_cast<YGDirection>(direction));
}

double FlexNode::getComputedLeft(void) const {
  return YGNodeLayoutGetLeft(m_node);
}

double FlexNode::getComputedRight(void) const {
  return YGNodeLayoutGetRight(m_node);
}

double FlexNode::getComputedTop(void) const {
  return YGNodeLayoutGetTop(m_node);
}

double FlexNode::getComputedBottom(void) const {
  return YGNodeLayoutGetBottom(m_node);
}

double FlexNode::getComputedWidth(void) const {
  return YGNodeLayoutGetWidth(m_node);
}

double FlexNode::getComputedHeight(void) const {
  return YGNodeLayoutGetHeight(m_node);
}

Layout FlexNode::getComputedLayout(void) const {
  Layout layout;

  layout.left = YGNodeLayoutGetLeft(m_node);
  layout.right = YGNodeLayoutGetRight(m_node);

  layout.top = YGNodeLayoutGetTop(m_node);
  layout.bottom = YGNodeLayoutGetBottom(m_node);

  layout.width = YGNodeLayoutGetWidth(m_node);
  layout.height = YGNodeLayoutGetHeight(m_node);

  return layout;
}

double FlexNode::getComputedMargin(int edge) const {
  return YGNodeLayoutGetMargin(m_node, static_cast<YGEdge>(edge));
}

double FlexNode::getComputedBorder(int edge) const {
  return YGNodeLayoutGetBorder(m_node, static_cast<YGEdge>(edge));
}

double FlexNode::getComputedPadding(int edge) const {
  return YGNodeLayoutGetPadding(m_node, static_cast<YGEdge>(edge));
}
