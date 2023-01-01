/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <yoga/Yoga.h>

class FConfig {

  friend class FNode;

public:
  static FConfig* create(void);

  static void destroy(FConfig* config);

private:
  FConfig(void);

public:
  ~FConfig(void);

public: // Prevent accidental copy
  FConfig(FConfig const&) = delete;

  FConfig const& operator=(FConfig const&) = delete;

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
