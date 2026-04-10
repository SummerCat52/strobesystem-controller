#pragma once

#include <Arduino.h>

#include "Config.h"
#include "LightController.h"
#include "PatternManager.h"
#include "SystemState.h"

class SafetyManager {
 public:
  SafetyManager(LightController& controller, PatternManager& patternManager);

  void onStartup(SystemState& state);
  void onCommandReceived(unsigned long nowMs, SystemState& state);
  void onClientConnected(unsigned long nowMs, SystemState& state);
  void onClientDisconnected(SystemState& state);
  void update(unsigned long nowMs, SystemState& state);

 private:
  void enterSafeState(SystemState& state, const String& reason);

  LightController& _controller;
  PatternManager& _patternManager;
};
