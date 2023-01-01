/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <yoga/Yoga.h>

#include "./FConfig.hh"

/* static */ FConfig* FConfig::create(void) {
  return new FConfig();
}

/* static */ void FConfig::destroy(FConfig* node) {
  delete node;
}

FConfig::FConfig(void) : m_config(YGConfigNew()) {}

FConfig::~FConfig(void) {
  YGConfigFree(m_config);
}

void FConfig::setExperimentalFeatureEnabled(int feature, bool enabled) {
  YGConfigSetExperimentalFeatureEnabled(
      m_config, static_cast<YGExperimentalFeature>(feature), enabled);
}

void FConfig::setPointScaleFactor(float pixelsInPoint) {
  YGConfigSetPointScaleFactor(m_config, pixelsInPoint);
}

void FConfig::setUseLegacyStretchBehaviour(bool useLegacyStretchBehaviour) {
  YGConfigSetUseLegacyStretchBehaviour(m_config, useLegacyStretchBehaviour);
}

void FConfig::setUseWebDefaults(bool useWebDefaults) {
  YGConfigSetUseWebDefaults(m_config, useWebDefaults);
}

bool FConfig::isExperimentalFeatureEnabled(int feature) const {
  return YGConfigIsExperimentalFeatureEnabled(
      m_config, static_cast<YGExperimentalFeature>(feature));
}

bool FConfig::useLegacyStretchBehaviour() {
  return YGConfigGetUseLegacyStretchBehaviour(m_config);
}

bool FConfig::useWebDefaults() {
  return YGConfigGetUseWebDefaults(m_config);
}
