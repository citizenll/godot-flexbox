/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <yoga/Yoga.h>

#include "./YGConfig.hh"

/* static */ YGConfig* YGConfig::create(void) {
  return new YGConfig();
}

/* static */ void YGConfig::destroy(YGConfig* node) {
  delete node;
}

YGConfig::YGConfig(void) : m_config(YGConfigNew()) {}

YGConfig::~YGConfig(void) {
  YGConfigFree(m_config);
}

void YGConfig::setExperimentalFeatureEnabled(int feature, bool enabled) {
  YGConfigSetExperimentalFeatureEnabled(
      m_config, static_cast<YGExperimentalFeature>(feature), enabled);
}

void YGConfig::setPointScaleFactor(float pixelsInPoint) {
  YGConfigSetPointScaleFactor(m_config, pixelsInPoint);
}

void YGConfig::setUseLegacyStretchBehaviour(bool useLegacyStretchBehaviour) {
  YGConfigSetUseLegacyStretchBehaviour(m_config, useLegacyStretchBehaviour);
}

void YGConfig::setUseWebDefaults(bool useWebDefaults) {
  YGConfigSetUseWebDefaults(m_config, useWebDefaults);
}

bool YGConfig::isExperimentalFeatureEnabled(int feature) const {
  return YGConfigIsExperimentalFeatureEnabled(
      m_config, static_cast<YGExperimentalFeature>(feature));
}

bool YGConfig::useLegacyStretchBehaviour() {
  return YGConfigGetUseLegacyStretchBehaviour(m_config);
}

bool YGConfig::useWebDefaults() {
  return YGConfigGetUseWebDefaults(m_config);
}
