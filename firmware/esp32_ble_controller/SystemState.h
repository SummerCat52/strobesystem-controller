#pragma once

#include <Arduino.h>

#include "Config.h"

enum class ChannelId : uint8_t {
  FrontLeft = 0,
  FrontRight,
  RearLeft,
  RearRight,
  SideLeft,
  SideRight,
  Beacon,
  Flood,
  Invalid = 255,
};

enum class Mode : uint8_t {
  Idle = 0,
  On,
  Off,
  SingleFlash,
  DoubleFlash,
  Strobe,
  Alternate,
  Sequence,
};

enum class RuntimePhase : uint8_t {
  Idle = 0,
  StepOn,
  StepOff,
  SeriesPause,
};

struct PatternConfig {
  Mode mode = Mode::Idle;
  uint32_t channelMask = 0;
  unsigned long onMs = Config::kDefaultPulseOnMs;
  unsigned long offMs = Config::kDefaultPulseOffMs;
  uint16_t repeat = Config::kDefaultRepeat;
  unsigned long seriesPauseMs = Config::kDefaultSeriesPauseMs;
  uint16_t speedPercent = 100;
  uint8_t sequence[Config::kMaxSequenceLength] = {0};
  uint8_t sequenceLength = 0;
};

struct SystemState {
  bool bleClientConnected = false;
  bool safeStateActive = true;
  Mode activeMode = Mode::Idle;
  unsigned long lastCommandAtMs = 0;
  String lastStatus = "BOOT";
  String lastError = "";
};

inline const char* channelName(ChannelId id) {
  switch (id) {
    case ChannelId::FrontLeft:
      return "FrontLeft";
    case ChannelId::FrontRight:
      return "FrontRight";
    case ChannelId::RearLeft:
      return "RearLeft";
    case ChannelId::RearRight:
      return "RearRight";
    case ChannelId::SideLeft:
      return "SideLeft";
    case ChannelId::SideRight:
      return "SideRight";
    case ChannelId::Beacon:
      return "Beacon";
    case ChannelId::Flood:
      return "Flood";
    default:
      return "Invalid";
  }
}

inline const char* modeName(Mode mode) {
  switch (mode) {
    case Mode::Idle:
      return "IDLE";
    case Mode::On:
      return "ON";
    case Mode::Off:
      return "OFF";
    case Mode::SingleFlash:
      return "SINGLE_FLASH";
    case Mode::DoubleFlash:
      return "DOUBLE_FLASH";
    case Mode::Strobe:
      return "STROBE";
    case Mode::Alternate:
      return "ALTERNATE";
    case Mode::Sequence:
      return "SEQUENCE";
    default:
      return "UNKNOWN";
  }
}

inline ChannelId channelFromName(String value) {
  value.trim();
  value.toUpperCase();

  if (value == "1" || value == "FRONTLEFT") return ChannelId::FrontLeft;
  if (value == "2" || value == "FRONTRIGHT") return ChannelId::FrontRight;
  if (value == "3" || value == "REARLEFT") return ChannelId::RearLeft;
  if (value == "4" || value == "REARRIGHT") return ChannelId::RearRight;
  if (value == "5" || value == "SIDELEFT") return ChannelId::SideLeft;
  if (value == "6" || value == "SIDERIGHT") return ChannelId::SideRight;
  if (value == "7" || value == "BEACON") return ChannelId::Beacon;
  if (value == "8" || value == "FLOOD") return ChannelId::Flood;
  return ChannelId::Invalid;
}

inline uint32_t channelMaskForGroup(String value) {
  value.trim();
  value.toUpperCase();

  if (value == "FRONT") {
    return (1UL << static_cast<uint8_t>(ChannelId::FrontLeft)) |
           (1UL << static_cast<uint8_t>(ChannelId::FrontRight));
  }
  if (value == "REAR") {
    return (1UL << static_cast<uint8_t>(ChannelId::RearLeft)) |
           (1UL << static_cast<uint8_t>(ChannelId::RearRight));
  }
  if (value == "SIDE" || value == "SIDES") {
    return (1UL << static_cast<uint8_t>(ChannelId::SideLeft)) |
           (1UL << static_cast<uint8_t>(ChannelId::SideRight));
  }
  if (value == "ALL") {
    return (1UL << Config::kChannelCount) - 1UL;
  }
  return 0;
}
