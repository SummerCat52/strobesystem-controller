#pragma once

#include <Arduino.h>
#include <Preferences.h>

#include "Config.h"
#include "LightController.h"
#include "SystemState.h"

class ConfigStore {
 public:
  void begin();
  void load(LightController& controller, SystemState& state);
  bool save(const LightController& controller, const SystemState& state);
  bool factoryReset(LightController& controller, SystemState& state);

 private:
  String pinKey(uint8_t index) const;
  String invertedKey(uint8_t index) const;

  Preferences _preferences;
};
