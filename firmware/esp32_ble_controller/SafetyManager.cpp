#include "SafetyManager.h"

SafetyManager::SafetyManager(LightController& controller,
                             PatternManager& patternManager)
    : _controller(controller), _patternManager(patternManager) {}

void SafetyManager::onStartup(SystemState& state) {
  enterSafeState(state, "STARTUP_SAFE");
}

void SafetyManager::onCommandReceived(unsigned long nowMs, SystemState& state) {
  state.bleClientConnected = true;
  state.lastCommandAtMs = nowMs;
  state.safeStateActive = false;
}

void SafetyManager::onClientConnected(unsigned long nowMs, SystemState& state) {
  state.bleClientConnected = true;
  state.lastCommandAtMs = nowMs;
  state.lastStatus = "BLE_CONNECTED";
}

void SafetyManager::onClientDisconnected(SystemState& state) {
  state.bleClientConnected = false;
  enterSafeState(state, "BLE_DISCONNECTED");
}

void SafetyManager::update(unsigned long nowMs, SystemState& state) {
  if (!state.bleClientConnected) {
    return;
  }
  if (nowMs - state.lastCommandAtMs < Config::kDisconnectSafeTimeoutMs) {
    return;
  }
  enterSafeState(state, "COMMAND_TIMEOUT");
  state.bleClientConnected = false;
}

void SafetyManager::enterSafeState(SystemState& state, const String& reason) {
  _patternManager.stop(state, true);
  _controller.allOff();
  state.safeStateActive = true;
  state.lastStatus = reason;
}
