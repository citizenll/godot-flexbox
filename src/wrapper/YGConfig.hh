/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <yoga/Yoga.h>

class YGConfig {

  friend class YGNode;

public:
  static YGConfig* create(void);

  static void destroy(YGConfig* config);

private:
  YGConfig(void);

public:
  ~YGConfig(void);

public: // Prevent accidental copy
  YGConfig(YGConfig const&) = delete;

  YGConfig const& operator=(YGConfig const&) = delete;

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
