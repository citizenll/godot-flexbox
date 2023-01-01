/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <algorithm>
#include "./FNode.hh"

static YGSize globalMeasureFunc(
    YGNodeRef nodeRef,
    float width,
    YGMeasureMode widthMode,
    float height,
    YGMeasureMode heightMode) {
  FNode const& node = *reinterpret_cast<FNode const*>(YGNodeGetContext(nodeRef));

  Size size = node.callMeasureFunc(width, widthMode, height, heightMode);
  YGSize ygSize = {
      static_cast<float>(size.width), static_cast<float>(size.height)};

  return ygSize;
}

static void globalDirtiedFunc(YGNodeRef nodeRef) {
  FNode const& node = *reinterpret_cast<FNode const*>(YGNodeGetContext(nodeRef));

  node.callDirtiedFunc();
}

/* static */ FNode* FNode::createDefault(void) {
  return new FNode(nullptr);
}

/* static */ FNode* FNode::createWithConfig(FConfig* config) {
  return new FNode(config);
}

/* static */ void FNode::destroy(FNode* node) {
  delete node;
}

/* static */ FNode* FNode::fromYGNode(YGNodeRef nodeRef) {
  return reinterpret_cast<FNode*>(YGNodeGetContext(nodeRef));
}

FNode::FNode(FConfig* config)
    : m_node(
          config != nullptr ? YGNodeNewWithConfig(config->m_config)
                            : YGNodeNew()),
      m_measureFunc(nullptr),
      m_dirtiedFunc(nullptr) {
  YGNodeSetContext(m_node, reinterpret_cast<void*>(this));
}

FNode::~FNode(void) {
  YGNodeFree(m_node);
}

void FNode::reset(void) {
  m_measureFunc.reset(nullptr);
  m_dirtiedFunc.reset(nullptr);

  YGNodeReset(m_node);
}

void FNode::copyStyle(FNode const& other) {
  YGNodeCopyStyle(m_node, other.m_node);
}

void FNode::setPositionType(int positionType) {
  YGNodeStyleSetPositionType(m_node, static_cast<YGPositionType>(positionType));
}

void FNode::setPosition(int edge, double position) {
  YGNodeStyleSetPosition(m_node, static_cast<YGEdge>(edge), position);
}

void FNode::setPositionPercent(int edge, double position) {
  YGNodeStyleSetPositionPercent(m_node, static_cast<YGEdge>(edge), position);
}

void FNode::setAlignContent(int alignContent) {
  YGNodeStyleSetAlignContent(m_node, static_cast<YGAlign>(alignContent));
}

void FNode::setAlignItems(int alignItems) {
  YGNodeStyleSetAlignItems(m_node, static_cast<YGAlign>(alignItems));
}

void FNode::setAlignSelf(int alignSelf) {
  YGNodeStyleSetAlignSelf(m_node, static_cast<YGAlign>(alignSelf));
}

void FNode::setFlexDirection(int flexDirection) {
  YGNodeStyleSetFlexDirection(
      m_node, static_cast<YGFlexDirection>(flexDirection));
}

void FNode::setFlexWrap(int flexWrap) {
  YGNodeStyleSetFlexWrap(m_node, static_cast<YGWrap>(flexWrap));
}

void FNode::setJustifyContent(int justifyContent) {
  YGNodeStyleSetJustifyContent(m_node, static_cast<YGJustify>(justifyContent));
}

void FNode::setMargin(int edge, double margin) {
  YGNodeStyleSetMargin(m_node, static_cast<YGEdge>(edge), margin);
}

void FNode::setMarginPercent(int edge, double margin) {
  YGNodeStyleSetMarginPercent(m_node, static_cast<YGEdge>(edge), margin);
}

void FNode::setMarginAuto(int edge) {
  YGNodeStyleSetMarginAuto(m_node, static_cast<YGEdge>(edge));
}

void FNode::setOverflow(int overflow) {
  YGNodeStyleSetOverflow(m_node, static_cast<YGOverflow>(overflow));
}

void FNode::setDisplay(int display) {
  YGNodeStyleSetDisplay(m_node, static_cast<YGDisplay>(display));
}

void FNode::setFlex(double flex) {
  YGNodeStyleSetFlex(m_node, flex);
}

void FNode::setFlexBasis(double flexBasis) {
  YGNodeStyleSetFlexBasis(m_node, flexBasis);
}

void FNode::setFlexBasisPercent(double flexBasis) {
  YGNodeStyleSetFlexBasisPercent(m_node, flexBasis);
}

void FNode::setFlexBasisAuto() {
  YGNodeStyleSetFlexBasisAuto(m_node);
}

void FNode::setFlexGrow(double flexGrow) {
  YGNodeStyleSetFlexGrow(m_node, flexGrow);
}

void FNode::setFlexShrink(double flexShrink) {
  YGNodeStyleSetFlexShrink(m_node, flexShrink);
}

void FNode::setWidth(double width) {
  YGNodeStyleSetWidth(m_node, width);
}

void FNode::setWidthPercent(double width) {
  YGNodeStyleSetWidthPercent(m_node, width);
}

void FNode::setWidthAuto() {
  YGNodeStyleSetWidthAuto(m_node);
}

void FNode::setHeight(double height) {
  YGNodeStyleSetHeight(m_node, height);
}

void FNode::setHeightPercent(double height) {
  YGNodeStyleSetHeightPercent(m_node, height);
}

void FNode::setHeightAuto() {
  YGNodeStyleSetHeightAuto(m_node);
}

void FNode::setMinWidth(double minWidth) {
  YGNodeStyleSetMinWidth(m_node, minWidth);
}

void FNode::setMinWidthPercent(double minWidth) {
  YGNodeStyleSetMinWidthPercent(m_node, minWidth);
}

void FNode::setMinHeight(double minHeight) {
  YGNodeStyleSetMinHeight(m_node, minHeight);
}

void FNode::setMinHeightPercent(double minHeight) {
  YGNodeStyleSetMinHeightPercent(m_node, minHeight);
}

void FNode::setMaxWidth(double maxWidth) {
  YGNodeStyleSetMaxWidth(m_node, maxWidth);
}

void FNode::setMaxWidthPercent(double maxWidth) {
  YGNodeStyleSetMaxWidthPercent(m_node, maxWidth);
}

void FNode::setMaxHeight(double maxHeight) {
  YGNodeStyleSetMaxHeight(m_node, maxHeight);
}

void FNode::setMaxHeightPercent(double maxHeight) {
  YGNodeStyleSetMaxHeightPercent(m_node, maxHeight);
}

void FNode::setAspectRatio(double aspectRatio) {
  YGNodeStyleSetAspectRatio(m_node, aspectRatio);
}

void FNode::setBorder(int edge, double border) {
  YGNodeStyleSetBorder(m_node, static_cast<YGEdge>(edge), border);
}

void FNode::setPadding(int edge, double padding) {
  YGNodeStyleSetPadding(m_node, static_cast<YGEdge>(edge), padding);
}

void FNode::setPaddingPercent(int edge, double padding) {
  YGNodeStyleSetPaddingPercent(m_node, static_cast<YGEdge>(edge), padding);
}

void FNode::setIsReferenceBaseline(bool isReferenceBaseline) {
  YGNodeSetIsReferenceBaseline(m_node, isReferenceBaseline);
}

void FNode::setGap(int gutter, double gapLength) {
  YGNodeStyleSetGap(m_node, static_cast<YGGutter>(gutter), gapLength);
}

int FNode::getPositionType(void) const {
  return YGNodeStyleGetPositionType(m_node);
}

Value FNode::getPosition(int edge) const {
  return Value::fromYGValue(
      YGNodeStyleGetPosition(m_node, static_cast<YGEdge>(edge)));
}

int FNode::getAlignContent(void) const {
  return YGNodeStyleGetAlignContent(m_node);
}

int FNode::getAlignItems(void) const {
  return YGNodeStyleGetAlignItems(m_node);
}

int FNode::getAlignSelf(void) const {
  return YGNodeStyleGetAlignSelf(m_node);
}

int FNode::getFlexDirection(void) const {
  return YGNodeStyleGetFlexDirection(m_node);
}

int FNode::getFlexWrap(void) const {
  return YGNodeStyleGetFlexWrap(m_node);
}

int FNode::getJustifyContent(void) const {
  return YGNodeStyleGetJustifyContent(m_node);
}

Value FNode::getMargin(int edge) const {
  return Value::fromYGValue(
      YGNodeStyleGetMargin(m_node, static_cast<YGEdge>(edge)));
}

int FNode::getOverflow(void) const {
  return YGNodeStyleGetOverflow(m_node);
}

int FNode::getDisplay(void) const {
  return YGNodeStyleGetDisplay(m_node);
}

Value FNode::getFlexBasis(void) const {
  return Value::fromYGValue(YGNodeStyleGetFlexBasis(m_node));
}

double FNode::getFlexGrow(void) const {
  return YGNodeStyleGetFlexGrow(m_node);
}

double FNode::getFlexShrink(void) const {
  return YGNodeStyleGetFlexShrink(m_node);
}

Value FNode::getWidth(void) const {
  return Value::fromYGValue(YGNodeStyleGetWidth(m_node));
}

Value FNode::getHeight(void) const {
  return Value::fromYGValue(YGNodeStyleGetHeight(m_node));
}

Value FNode::getMinWidth(void) const {
  return Value::fromYGValue(YGNodeStyleGetMinWidth(m_node));
}

Value FNode::getMinHeight(void) const {
  return Value::fromYGValue(YGNodeStyleGetMinHeight(m_node));
}

Value FNode::getMaxWidth(void) const {
  return Value::fromYGValue(YGNodeStyleGetMaxWidth(m_node));
}

Value FNode::getMaxHeight(void) const {
  return Value::fromYGValue(YGNodeStyleGetMaxHeight(m_node));
}

double FNode::getAspectRatio(void) const {
  return YGNodeStyleGetAspectRatio(m_node);
}

double FNode::getBorder(int edge) const {
  return YGNodeStyleGetBorder(m_node, static_cast<YGEdge>(edge));
}

Value FNode::getPadding(int edge) const {
  return Value::fromYGValue(
      YGNodeStyleGetPadding(m_node, static_cast<YGEdge>(edge)));
}

float FNode::getGap(int gutter) {
  return YGNodeStyleGetGap(m_node, static_cast<YGGutter>(gutter));
}

bool FNode::isReferenceBaseline() {
  return YGNodeIsReferenceBaseline(m_node);
}

void FNode::insertChild(FNode* child, unsigned index) {
  YGNodeInsertChild(m_node, child->m_node, index);
}

void FNode::removeChild(FNode* child) {
  YGNodeRemoveChild(m_node, child->m_node);
}

unsigned FNode::getChildCount(void) const {
  return YGNodeGetChildCount(m_node);
}

FNode* FNode::getParent(void) {
  auto nodePtr = YGNodeGetParent(m_node);

  if (nodePtr == nullptr)
    return nullptr;

  return FNode::fromYGNode(nodePtr);
}

FNode* FNode::getChild(unsigned index) {
  auto nodePtr = YGNodeGetChild(m_node, index);

  if (nodePtr == nullptr)
    return nullptr;

  return FNode::fromYGNode(nodePtr);
}

void FNode::setMeasureFunc(MeasureCallback* measureFunc) {
  m_measureFunc.reset(measureFunc);

  YGNodeSetMeasureFunc(m_node, &globalMeasureFunc);
}

void FNode::unsetMeasureFunc(void) {
  m_measureFunc.reset(nullptr);

  YGNodeSetMeasureFunc(m_node, nullptr);
}

Size FNode::callMeasureFunc(
    double width,
    int widthMode,
    double height,
    int heightMode) const {
  return m_measureFunc->measure(width, widthMode, height, heightMode);
}

void FNode::setDirtiedFunc(DirtiedCallback* dirtiedFunc) {
  m_dirtiedFunc.reset(dirtiedFunc);

  YGNodeSetDirtiedFunc(m_node, &globalDirtiedFunc);
}

void FNode::unsetDirtiedFunc(void) {
  m_dirtiedFunc.reset(nullptr);

  YGNodeSetDirtiedFunc(m_node, nullptr);
}

void FNode::callDirtiedFunc(void) const {
  m_dirtiedFunc->dirtied();
}

void FNode::markDirty(void) {
  YGNodeMarkDirty(m_node);
}

bool FNode::isDirty(void) const {
  return YGNodeIsDirty(m_node);
}

void FNode::calculateLayout(double width, double height, int direction) {
  YGNodeCalculateLayout(
      m_node, width, height, static_cast<YGDirection>(direction));
}

double FNode::getComputedLeft(void) const {
  return YGNodeLayoutGetLeft(m_node);
}

double FNode::getComputedRight(void) const {
  return YGNodeLayoutGetRight(m_node);
}

double FNode::getComputedTop(void) const {
  return YGNodeLayoutGetTop(m_node);
}

double FNode::getComputedBottom(void) const {
  return YGNodeLayoutGetBottom(m_node);
}

double FNode::getComputedWidth(void) const {
  return YGNodeLayoutGetWidth(m_node);
}

double FNode::getComputedHeight(void) const {
  return YGNodeLayoutGetHeight(m_node);
}

Layout FNode::getComputedLayout(void) const {
  Layout layout;

  layout.left = YGNodeLayoutGetLeft(m_node);
  layout.right = YGNodeLayoutGetRight(m_node);

  layout.top = YGNodeLayoutGetTop(m_node);
  layout.bottom = YGNodeLayoutGetBottom(m_node);

  layout.width = YGNodeLayoutGetWidth(m_node);
  layout.height = YGNodeLayoutGetHeight(m_node);

  return layout;
}

double FNode::getComputedMargin(int edge) const {
  return YGNodeLayoutGetMargin(m_node, static_cast<YGEdge>(edge));
}

double FNode::getComputedBorder(int edge) const {
  return YGNodeLayoutGetBorder(m_node, static_cast<YGEdge>(edge));
}

double FNode::getComputedPadding(int edge) const {
  return YGNodeLayoutGetPadding(m_node, static_cast<YGEdge>(edge));
}
