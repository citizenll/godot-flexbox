/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <yoga/Yoga.h>

#include "./FlexConfig.hh"

/* static */ FlexConfig* FlexConfig::create(void) {
  return new FlexConfig();
}

/* static */ void FlexConfig::destroy(FlexConfig* node) {
  delete node;
}

FlexConfig::FlexConfig(void) : m_config(YGConfigNew()) {}

FlexConfig::~FlexConfig(void) {
  YGConfigFree(m_config);
}

void FlexConfig::setExperimentalFeatureEnabled(int feature, bool enabled) {
  YGConfigSetExperimentalFeatureEnabled(
      m_config, static_cast<YGExperimentalFeature>(feature), enabled);
}

void FlexConfig::setPointScaleFactor(float pixelsInPoint) {
  YGConfigSetPointScaleFactor(m_config, pixelsInPoint);
}

void FlexConfig::setUseLegacyStretchBehaviour(bool useLegacyStretchBehaviour) {
  YGConfigSetUseLegacyStretchBehaviour(m_config, useLegacyStretchBehaviour);
}

void FlexConfig::setUseWebDefaults(bool useWebDefaults) {
  YGConfigSetUseWebDefaults(m_config, useWebDefaults);
}

bool FlexConfig::isExperimentalFeatureEnabled(int feature) const {
  return YGConfigIsExperimentalFeatureEnabled(
      m_config, static_cast<YGExperimentalFeature>(feature));
}

bool FlexConfig::useLegacyStretchBehaviour() {
  return YGConfigGetUseLegacyStretchBehaviour(m_config);
}

bool FlexConfig::useWebDefaults() {
  return YGConfigGetUseWebDefaults(m_config);
}
