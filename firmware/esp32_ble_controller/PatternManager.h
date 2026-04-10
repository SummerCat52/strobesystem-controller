#pragma once

#include <Arduino.h>

#include "LightController.h"
#include "SystemState.h"

class PatternManager {
 public:
  explicit PatternManager(LightController& controller);

  void start(const PatternConfig& config, SystemState& state);
  void stop(SystemState& state, bool allOff = true);
  void update(unsigned long nowMs, SystemState& state);
  bool isRunning() const;

 private:
  void applyCurrentStep(bool on);
  void advanceSequenceIndex();
  uint16_t effectiveOnMs() const;
  uint16_t effectiveOffMs() const;
  uint16_t effectiveSeriesPauseMs() const;
  uint8_t pulsesPerSeries() const;
  uint8_t currentSequenceChannel() const;
  uint8_t buildDefaultSequence(uint32_t channelMask);

  LightController& _controller;
  PatternConfig _config;
  RuntimePhase _phase = RuntimePhase::Idle;
  unsigned long _phaseStartedAtMs = 0;
  uint16_t _pulseInSeries = 0;
  uint8_t _sequenceIndex = 0;
  bool _running = false;
};
