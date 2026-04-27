#include "ConfigStore.h"

void ConfigStore::begin() {
  _preferences.begin("stagepatch", false);
}

void ConfigStore::load(LightController& controller, SystemState& state) {
  state.disconnectSafeTimeoutMs = _preferences.getULong(
      "failsafe", Config::kDisconnectSafeTimeoutMs);

  for (uint8_t i = 0; i < Config::kChannelCount; ++i) {
    const auto id = static_cast<ChannelId>(i);
    const uint8_t gpio =
        _preferences.getUChar(pinKey(i).c_str(), Config::kChannelPins[i]);
    const bool inverted =
        _preferences.getBool(invertedKey(i).c_str(), Config::kChannelInverted[i]);
    controller.configureChannel(id, gpio, inverted);
  }
}

bool ConfigStore::save(const LightController& controller,
                       const SystemState& state) {
  _preferences.putULong("failsafe", state.disconnectSafeTimeoutMs);

  for (uint8_t i = 0; i < Config::kChannelCount; ++i) {
    const ChannelConfig& channel =
        controller.channelConfig(static_cast<ChannelId>(i));
    _preferences.putUChar(pinKey(i).c_str(), channel.gpio);
    _preferences.putBool(invertedKey(i).c_str(), channel.inverted);
  }
  return true;
}

bool ConfigStore::factoryReset(LightController& controller, SystemState& state) {
  _preferences.clear();
  state.disconnectSafeTimeoutMs = Config::kDisconnectSafeTimeoutMs;
  for (uint8_t i = 0; i < Config::kChannelCount; ++i) {
    controller.configureChannel(static_cast<ChannelId>(i), Config::kChannelPins[i],
                                Config::kChannelInverted[i]);
  }
  return save(controller, state);
}

String ConfigStore::pinKey(uint8_t index) const {
  return "pin" + String(index);
}

String ConfigStore::invertedKey(uint8_t index) const {
  return "inv" + String(index);
}
