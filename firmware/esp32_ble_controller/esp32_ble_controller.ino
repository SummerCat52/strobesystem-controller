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
  void handleParsedCommand(const ParsedCommand& command) {
    switch (command.type) {
      case ParsedCommandType::Ping:
        publishStatus("PONG");
        return;

      case ParsedCommandType::Status:
        publishStatus(buildStatusText());
        return;

      case ParsedCommandType::Stop:
        _patternManager.stop(_state, true);
        publishStatus("STOPPED");
        return;

      case ParsedCommandType::AllOff:
        _patternManager.stop(_state, true);
        _lightController.allOff();
        publishStatus("ALL_OFF");
        return;

      case ParsedCommandType::ImmediateOn:
        _patternManager.stop(_state, false);
        _lightController.setMask(command.channelMask, true);
        _state.activeMode = Mode::On;
        publishStatus(buildStatusText());
        return;

      case ParsedCommandType::ImmediateOff:
        _patternManager.stop(_state, false);
        _lightController.setMask(command.channelMask, false);
        _state.activeMode = Mode::Off;
        publishStatus(buildStatusText());
        return;

      case ParsedCommandType::Pattern:
        if (command.pattern.mode == Mode::On) {
          _patternManager.stop(_state, false);
          _lightController.setMask(command.pattern.channelMask, true);
          _state.activeMode = Mode::On;
          publishStatus(buildStatusText());
          return;
        }
        if (command.pattern.mode == Mode::Off) {
          _patternManager.stop(_state, false);
          _lightController.setMask(command.pattern.channelMask, false);
          _state.activeMode = Mode::Off;
          publishStatus(buildStatusText());
          return;
        }

        _patternManager.start(command.pattern, _state);
        publishStatus(buildStatusText());
        return;

      default:
        publishStatus("ERROR:Unhandled command");
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
