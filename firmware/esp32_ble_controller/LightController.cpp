#include "LightController.h"

namespace {
bool logicalStateToPinState(bool on, bool inverted) {
  return inverted ? !on : on;
}
}  // namespace

void LightController::begin() {
  for (uint8_t i = 0; i < Config::kChannelCount; ++i) {
    pinMode(_channels[i].gpio, OUTPUT);
  }
  allOff();
}

void LightController::allOff() {
  setMask((1UL << Config::kChannelCount) - 1UL, false);
}

void LightController::setChannel(ChannelId id, bool on) {
  const uint8_t index = static_cast<uint8_t>(id);
  if (index >= Config::kChannelCount) {
    return;
  }

  const bool pinState = logicalStateToPinState(on, _channels[index].inverted);
  digitalWrite(_channels[index].gpio, pinState ? HIGH : LOW);
  _channelStates[index] = on;
}

void LightController::setMask(uint32_t channelMask, bool on) {
  for (uint8_t i = 0; i < Config::kChannelCount; ++i) {
    if ((channelMask & (1UL << i)) != 0) {
      setChannel(static_cast<ChannelId>(i), on);
    }
  }
}

void LightController::pulseSingle(ChannelId id, bool on) {
  setChannel(id, on);
}

void LightController::pulseMask(uint32_t channelMask, bool on) {
  setMask(channelMask, on);
}

bool LightController::isOn(ChannelId id) const {
  const uint8_t index = static_cast<uint8_t>(id);
  return index < Config::kChannelCount ? _channelStates[index] : false;
}

uint32_t LightController::activeMask() const {
  uint32_t mask = 0;
  for (uint8_t i = 0; i < Config::kChannelCount; ++i) {
    if (_channelStates[i]) {
      mask |= (1UL << i);
    }
  }
  return mask;
}

String LightController::activeChannelsCsv() const {
  String result;
  for (uint8_t i = 0; i < Config::kChannelCount; ++i) {
    if (!_channelStates[i]) {
      continue;
    }
    if (!result.isEmpty()) {
      result += ",";
    }
    result += _channels[i].name;
  }
  return result.isEmpty() ? "NONE" : result;
}

const ChannelConfig& LightController::channelConfig(ChannelId id) const {
  return _channels[static_cast<uint8_t>(id)];
}
