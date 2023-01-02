/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <yoga/Yoga.h>

class FlexConfig {

  friend class FlexNode;

public:
  static FlexConfig* create(void);

  static void destroy(FlexConfig* config);

public:
  FlexConfig(void);
  ~FlexConfig(void);

public: // Prevent accidental copy
  FlexConfig(FlexConfig const&) = delete;

  FlexConfig const& operator=(FlexConfig const&) = delete;

public: // Setters
  void setExperimentalFeatureEnabled(int feature, bool enabled);
  void setPointScaleFactor(float pixelsInPoint);
  void setUseLegacyStretchBehaviour(bool useLegacyStretchBehaviour);
  void setUseWebDefaults(bool useWebDefaults);

public: // Getters
  bool isExperimentalFeatureEnabled(int feature) const;
  bool useLegacyStretchBehaviour();
  bool useWebDefaults();

private:
  YGConfigRef m_config;
};
