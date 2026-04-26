#include <Arduino.h>

#include "BLEHandler.h"
#include "CommandParser.h"
#include "LightController.h"
#include "PatternManager.h"
#include "SafetyManager.h"
#include "SystemState.h"

class StrobeControllerApp : public BLEHandlerListener {
 public:
  StrobeControllerApp()
      : _patternManager(_lightController),
        _safetyManager(_lightController, _patternManager),
        _bleHandler(*this) {}

  void begin() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("ESP32 BLE strobe controller booting...");

    _lightController.begin();
    _lightController.printChannelMap();
    _safetyManager.onStartup(_state);
    _bleHandler.begin();
    publishStatus("READY");
  }

  void loop() {
    const unsigned long nowMs = millis();
    _patternManager.update(nowMs, _state);
    _safetyManager.update(nowMs, _state);
  }

  void onBleCommand(const String& command) override {
    const unsigned long nowMs = millis();

    Serial.print("RX: ");
    Serial.println(command);

    const ParsedCommand parsed = _commandParser.parse(command);
    if (parsed.type == ParsedCommandType::Invalid) {
      _state.lastError = parsed.error;
      publishStatus("ERROR:" + parsed.error);
      Serial.print("ERR: ");
      Serial.println(parsed.error);
      return;
    }

    _safetyManager.onCommandReceived(nowMs, _state);
    handleParsedCommand(parsed);
  }

  void onBleConnected() override {
    _safetyManager.onClientConnected(millis(), _state);
    publishStatus("CONNECTED");
    Serial.println("BLE client connected");
  }

  void onBleDisconnected() override {
    _safetyManager.onClientDisconnected(_state);
    publishStatus("SAFE:BLE_DISCONNECTED");
    Serial.println("BLE client disconnected");
  }

 private:
  const char* parsedTypeName(ParsedCommandType type) const {
    switch (type) {
      case ParsedCommandType::Ping:
        return "Ping";
      case ParsedCommandType::Status:
        return "Status";
      case ParsedCommandType::Stop:
        return "Stop";
      case ParsedCommandType::AllOff:
        return "AllOff";
      case ParsedCommandType::ImmediateOn:
        return "ImmediateOn";
      case ParsedCommandType::ImmediateOff:
        return "ImmediateOff";
      case ParsedCommandType::Pattern:
        return "Pattern";
      case ParsedCommandType::Invalid:
      default:
        return "Invalid";
    }
  }

  void printMaskDetails(uint32_t channelMask) const {
    Serial.print("CHANNEL_MASK: 0b");
    for (int8_t i = Config::kChannelCount - 1; i >= 0; --i) {
      Serial.print((channelMask & (1UL << i)) != 0 ? '1' : '0');
    }
    Serial.print(" channels=");
    bool printedAny = false;
    for (uint8_t i = 0; i < Config::kChannelCount; ++i) {
      if ((channelMask & (1UL << i)) == 0) {
        continue;
      }
      if (printedAny) {
        Serial.print(",");
      }
      Serial.print(channelName(static_cast<ChannelId>(i)));
      printedAny = true;
    }
    Serial.println(printedAny ? "" : "NONE");
  }

  void printParsedCommand(const ParsedCommand& command) const {
    Serial.print("PARSED: type=");
    Serial.print(parsedTypeName(command.type));
    Serial.print(" mode=");
    Serial.print(modeName(command.pattern.mode));
    Serial.print(" onMs=");
    Serial.print(command.pattern.onMs);
    Serial.print(" offMs=");
    Serial.print(command.pattern.offMs);
    Serial.print(" repeat=");
    Serial.print(command.pattern.repeat);
    Serial.print(" pauseMs=");
    Serial.print(command.pattern.seriesPauseMs);
    Serial.print(" speedPercent=");
    Serial.println(command.pattern.speedPercent);
    printMaskDetails(command.channelMask != 0 ? command.channelMask
                                              : command.pattern.channelMask);
  }

  void handleParsedCommand(const ParsedCommand& command) {
    printParsedCommand(command);
    switch (command.type) {
      case ParsedCommandType::Ping:
        publishStatus("PONG");
        _lightController.printStateSnapshot();
        return;

      case ParsedCommandType::Status:
        publishStatus(buildStatusText());
        _lightController.printStateSnapshot();
        return;

      case ParsedCommandType::Stop:
        _patternManager.stop(_state, true);
        publishStatus("STOPPED");
        _lightController.printStateSnapshot();
        return;

      case ParsedCommandType::AllOff:
        _patternManager.stop(_state, true);
        _lightController.allOff();
        publishStatus("ALL_OFF");
        _lightController.printStateSnapshot();
        return;

      case ParsedCommandType::ImmediateOn:
        _patternManager.stop(_state, false);
        _lightController.setMask(command.channelMask, true);
        _state.activeMode = Mode::On;
        publishStatus(buildStatusText());
        _lightController.printStateSnapshot();
        return;

      case ParsedCommandType::ImmediateOff:
        _patternManager.stop(_state, false);
        _lightController.setMask(command.channelMask, false);
        _state.activeMode = Mode::Off;
        publishStatus(buildStatusText());
        _lightController.printStateSnapshot();
        return;

      case ParsedCommandType::Pattern:
        if (command.pattern.mode == Mode::On) {
          _patternManager.stop(_state, false);
          _lightController.setMask(command.pattern.channelMask, true);
          _state.activeMode = Mode::On;
          publishStatus(buildStatusText());
          _lightController.printStateSnapshot();
          return;
        }
        if (command.pattern.mode == Mode::Off) {
          _patternManager.stop(_state, false);
          _lightController.setMask(command.pattern.channelMask, false);
          _state.activeMode = Mode::Off;
          publishStatus(buildStatusText());
          _lightController.printStateSnapshot();
          return;
        }

        _patternManager.start(command.pattern, _state);
        publishStatus(buildStatusText());
        _lightController.printStateSnapshot();
        return;

      default:
        publishStatus("ERROR:Unhandled command");
        _lightController.printStateSnapshot();
        return;
    }
  }

  String buildStatusText() const {
    String status = "STATE=";
    status += _state.bleClientConnected ? "CONNECTED" : "DISCONNECTED";
    status += ";MODE=";
    status += modeName(_state.activeMode);
    status += ";ACTIVE=";
    status += _lightController.activeChannelsCsv();
    status += ";SAFE=";
    status += _state.safeStateActive ? "1" : "0";
    return status;
  }

  void publishStatus(const String& status) {
    _state.lastStatus = status;
    _bleHandler.updateStatus(status);
    Serial.print("STATUS: ");
    Serial.println(status);
  }

  SystemState _state;
  LightController _lightController;
  PatternManager _patternManager;
  SafetyManager _safetyManager;
  CommandParser _commandParser;
  BLEHandler _bleHandler;
};

StrobeControllerApp app;

void setup() {
  app.begin();
}

void loop() {
  app.loop();
}
