#pragma once

#include <Arduino.h>

#include "Config.h"
#include "SystemState.h"

enum class ParsedCommandType : uint8_t {
  Invalid = 0,
  Ping,
  Status,
  Stop,
  AllOff,
  ImmediateOn,
  ImmediateOff,
  Pattern,
};

struct ParsedCommand {
  ParsedCommandType type = ParsedCommandType::Invalid;
  PatternConfig pattern;
  uint32_t channelMask = 0;
  String error;
};

class CommandParser {
 public:
  ParsedCommand parse(const String& rawCommand) const;

 private:
  ParsedCommand parseTextCommand(String command) const;
  ParsedCommand parseModeCommand(String command) const;
  ParsedCommand parseImmediateChannelCommand(String command) const;
  bool applyField(String key, String value, PatternConfig& config,
                  ParsedCommand& result) const;
  bool parseChannelList(const String& rawValue, uint32_t& mask) const;
  bool parseOrderList(const String& rawValue, PatternConfig& config) const;
};
