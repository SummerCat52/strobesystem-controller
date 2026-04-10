#include "CommandParser.h"

namespace {
String nextToken(String& input, char separator) {
  const int index = input.indexOf(separator);
  if (index < 0) {
    String token = input;
    input = "";
    token.trim();
    return token;
  }
  String token = input.substring(0, index);
  input.remove(0, index + 1);
  token.trim();
  return token;
}
}  // namespace

ParsedCommand CommandParser::parse(const String& rawCommand) const {
  String command = rawCommand;
  command.trim();

  if (command.isEmpty()) {
    ParsedCommand result;
    result.error = "Empty command";
    return result;
  }

  return parseTextCommand(command);
}

ParsedCommand CommandParser::parseTextCommand(String command) const {
  ParsedCommand result;
  String upper = command;
  upper.trim();

  if (upper == "PING") {
    result.type = ParsedCommandType::Ping;
    return result;
  }
  if (upper == "STATUS") {
    result.type = ParsedCommandType::Status;
    return result;
  }
  if (upper == "STOP") {
    result.type = ParsedCommandType::Stop;
    return result;
  }
  if (upper == "ALL_OFF") {
    result.type = ParsedCommandType::AllOff;
    return result;
  }
  if (upper.startsWith("MODE=")) {
    return parseModeCommand(command);
  }
  if (upper.indexOf('=') > 0) {
    return parseImmediateChannelCommand(command);
  }

  result.error = "Unknown command";
  return result;
}

ParsedCommand CommandParser::parseModeCommand(String command) const {
  ParsedCommand result;
  result.type = ParsedCommandType::Pattern;
  PatternConfig config;

  while (!command.isEmpty()) {
    const String token = nextToken(command, ';');
    const int separatorIndex = token.indexOf('=');
    if (separatorIndex < 0) {
      result.error = "Missing '=' in MODE command";
      result.type = ParsedCommandType::Invalid;
      return result;
    }

    String key = token.substring(0, separatorIndex);
    String value = token.substring(separatorIndex + 1);
    key.trim();
    value.trim();
    key.toUpperCase();

    if (!applyField(key, value, config, result)) {
      result.type = ParsedCommandType::Invalid;
      return result;
    }
  }

  if (config.mode == Mode::Idle) {
    result.error = "MODE is required";
    result.type = ParsedCommandType::Invalid;
    return result;
  }

  if ((config.mode == Mode::On || config.mode == Mode::Off) &&
      config.channelMask == 0) {
    result.error = "CH or GROUP is required for MODE=ON/OFF";
    result.type = ParsedCommandType::Invalid;
    return result;
  }

  if ((config.mode == Mode::SingleFlash || config.mode == Mode::DoubleFlash ||
       config.mode == Mode::Strobe || config.mode == Mode::Alternate ||
       config.mode == Mode::Sequence) &&
      config.channelMask == 0) {
    result.error = "No valid channels selected";
    result.type = ParsedCommandType::Invalid;
    return result;
  }

  result.pattern = config;
  result.channelMask = config.channelMask;
  return result;
}

ParsedCommand CommandParser::parseImmediateChannelCommand(String command) const {
  ParsedCommand result;
  const int separatorIndex = command.indexOf('=');
  if (separatorIndex < 0) {
    result.error = "Invalid immediate command";
    return result;
  }

  String left = command.substring(0, separatorIndex);
  String right = command.substring(separatorIndex + 1);
  left.trim();
  right.trim();

  uint32_t mask = channelMaskForGroup(left);
  if (mask == 0) {
    const ChannelId id = channelFromName(left);
    if (id != ChannelId::Invalid) {
      mask = 1UL << static_cast<uint8_t>(id);
    }
  }

  if (mask == 0) {
    result.error = "Unknown channel or group in immediate command";
    return result;
  }

  right.toUpperCase();
  if (right == "ON") {
    result.type = ParsedCommandType::ImmediateOn;
    result.channelMask = mask;
    return result;
  }
  if (right == "OFF") {
    result.type = ParsedCommandType::ImmediateOff;
    result.channelMask = mask;
    return result;
  }

  result.error = "Immediate command expects ON or OFF";
  return result;
}

bool CommandParser::applyField(String key, String value, PatternConfig& config,
                               ParsedCommand& result) const {
  if (key == "MODE") {
    value.toUpperCase();
    if (value == "ON") {
      config.mode = Mode::On;
      return true;
    }
    if (value == "OFF") {
      config.mode = Mode::Off;
      return true;
    }
    if (value == "SINGLE_FLASH") {
      config.mode = Mode::SingleFlash;
      return true;
    }
    if (value == "DOUBLE_FLASH") {
      config.mode = Mode::DoubleFlash;
      return true;
    }
    if (value == "STROBE") {
      config.mode = Mode::Strobe;
      return true;
    }
    if (value == "ALTERNATE") {
      config.mode = Mode::Alternate;
      return true;
    }
    if (value == "SEQUENCE") {
      config.mode = Mode::Sequence;
      return true;
    }
    result.error = "Unsupported MODE";
    return false;
  }

  if (key == "CH" || key == "CHANNELS") {
    uint32_t mask = 0;
    if (!parseChannelList(value, mask)) {
      result.error = "Invalid CH list";
      return false;
    }
    config.channelMask |= mask;
    return true;
  }

  if (key == "GROUP") {
    const uint32_t groupMask = channelMaskForGroup(value);
    if (groupMask == 0) {
      result.error = "Unknown GROUP";
      return false;
    }
    config.channelMask |= groupMask;
    return true;
  }

  if (key == "ON" || key == "FLASH_ON") {
    config.onMs = max(1, value.toInt());
    return true;
  }

  if (key == "OFF" || key == "FLASH_OFF") {
    config.offMs = max(1, value.toInt());
    return true;
  }

  if (key == "REP" || key == "REPEAT") {
    config.repeat = max(1, value.toInt());
    return true;
  }

  if (key == "PAUSE" || key == "SERIES_PAUSE") {
    config.seriesPauseMs = max(1, value.toInt());
    return true;
  }

  if (key == "SPEED") {
    config.speedPercent = constrain(value.toInt(), 1, 1000);
    return true;
  }

  if (key == "ORDER") {
    if (!parseOrderList(value, config)) {
      result.error = "Invalid ORDER list";
      return false;
    }
    return true;
  }

  result.error = "Unknown field: " + key;
  return false;
}

bool CommandParser::parseChannelList(const String& rawValue, uint32_t& mask) const {
  String remaining = rawValue;
  while (!remaining.isEmpty()) {
    String token = nextToken(remaining, ',');
    if (token.isEmpty()) {
      continue;
    }
    const ChannelId id = channelFromName(token);
    if (id == ChannelId::Invalid) {
      return false;
    }
    mask |= (1UL << static_cast<uint8_t>(id));
  }
  return mask != 0;
}

bool CommandParser::parseOrderList(const String& rawValue,
                                   PatternConfig& config) const {
  String remaining = rawValue;
  uint8_t index = 0;
  while (!remaining.isEmpty() && index < Config::kMaxSequenceLength) {
    String token = nextToken(remaining, ',');
    if (token.isEmpty()) {
      continue;
    }
    const ChannelId id = channelFromName(token);
    if (id == ChannelId::Invalid) {
      return false;
    }
    config.sequence[index++] = static_cast<uint8_t>(id);
    config.channelMask |= (1UL << static_cast<uint8_t>(id));
  }
  config.sequenceLength = index;
  return index > 0;
}
