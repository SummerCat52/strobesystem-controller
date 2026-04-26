#pragma once

#include <Arduino.h>

#include "Config.h"
#include "SystemState.h"

struct ChannelConfig {
  ChannelId id;
  const char* name;
  uint8_t gpio;
  bool inverted;
};

class LightController {
 public:
  void begin();
  void allOff();
  void setChannel(ChannelId id, bool on);
  void setMask(uint32_t channelMask, bool on);
  void pulseSingle(ChannelId id, bool on);
  void pulseMask(uint32_t channelMask, bool on);
  bool isOn(ChannelId id) const;
  uint32_t activeMask() const;
  String activeChannelsCsv() const;
  const ChannelConfig& channelConfig(ChannelId id) const;
  void printChannelMap() const;
  void printStateSnapshot() const;

 private:
  bool _channelStates[Config::kChannelCount] = {false};
  ChannelConfig _channels[Config::kChannelCount] = {
      {ChannelId::FrontLeft, "FrontLeft", Config::kChannelPins[0],
       Config::kChannelInverted[0]},
      {ChannelId::FrontRight, "FrontRight", Config::kChannelPins[1],
       Config::kChannelInverted[1]},
      {ChannelId::RearLeft, "RearLeft", Config::kChannelPins[2],
       Config::kChannelInverted[2]},
      {ChannelId::RearRight, "RearRight", Config::kChannelPins[3],
       Config::kChannelInverted[3]},
      {ChannelId::SideLeft, "SideLeft", Config::kChannelPins[4],
       Config::kChannelInverted[4]},
      {ChannelId::SideRight, "SideRight", Config::kChannelPins[5],
       Config::kChannelInverted[5]},
      {ChannelId::Beacon, "Beacon", Config::kChannelPins[6],
       Config::kChannelInverted[6]},
      {ChannelId::Flood, "Flood", Config::kChannelPins[7],
       Config::kChannelInverted[7]},
  };
};
