#include "PatternManager.h"

PatternManager::PatternManager(LightController& controller)
    : _controller(controller) {}

void PatternManager::start(const PatternConfig& config, SystemState& state) {
  _config = config;
  if (_config.sequenceLength == 0) {
    _config.sequenceLength = buildDefaultSequence(_config.channelMask);
  }
  _phase = RuntimePhase::StepOn;
  _phaseStartedAtMs = millis();
  _pulseInSeries = 0;
  _sequenceIndex = 0;
  _running = true;
  state.activeMode = _config.mode;
  state.safeStateActive = false;
  applyCurrentStep(true);
}

void PatternManager::stop(SystemState& state, bool allOff) {
  _running = false;
  _phase = RuntimePhase::Idle;
  _pulseInSeries = 0;
  _sequenceIndex = 0;
  if (allOff) {
    _controller.allOff();
  }
  state.activeMode = Mode::Idle;
  state.safeStateActive = true;
}

void PatternManager::update(unsigned long nowMs, SystemState& state) {
  if (!_running) {
    return;
  }

  const unsigned long elapsed = nowMs - _phaseStartedAtMs;

  switch (_phase) {
    case RuntimePhase::StepOn:
      if (elapsed < effectiveOnMs()) {
        return;
      }
      applyCurrentStep(false);
      _phase = RuntimePhase::StepOff;
      _phaseStartedAtMs = nowMs;
      return;

    case RuntimePhase::StepOff:
      if (elapsed < effectiveOffMs()) {
        return;
      }
      ++_pulseInSeries;
      if (_pulseInSeries >= pulsesPerSeries()) {
        _pulseInSeries = 0;
        _phase = RuntimePhase::SeriesPause;
        _phaseStartedAtMs = nowMs;
        return;
      }
      advanceSequenceIndex();
      applyCurrentStep(true);
      _phase = RuntimePhase::StepOn;
      _phaseStartedAtMs = nowMs;
      return;

    case RuntimePhase::SeriesPause:
      if (elapsed < effectiveSeriesPauseMs()) {
        return;
      }
      if (_config.mode == Mode::SingleFlash ||
          _config.mode == Mode::DoubleFlash) {
        stop(state, true);
        return;
      }
      _sequenceIndex = 0;
      applyCurrentStep(true);
      _phase = RuntimePhase::StepOn;
      _phaseStartedAtMs = nowMs;
      return;

    case RuntimePhase::Idle:
    default:
      return;
  }
}

bool PatternManager::isRunning() const {
  return _running;
}

void PatternManager::applyCurrentStep(bool on) {
  switch (_config.mode) {
    case Mode::SingleFlash:
    case Mode::DoubleFlash:
    case Mode::Strobe:
      _controller.pulseMask(_config.channelMask, on);
      return;

    case Mode::Alternate:
    case Mode::Sequence:
      _controller.allOff();
      if (on && _config.sequenceLength > 0) {
        _controller.pulseSingle(
            static_cast<ChannelId>(currentSequenceChannel()), true);
      }
      return;

    default:
      return;
  }
}

void PatternManager::advanceSequenceIndex() {
  if (_config.sequenceLength == 0) {
    return;
  }
  _sequenceIndex = (_sequenceIndex + 1) % _config.sequenceLength;
}

uint16_t PatternManager::effectiveOnMs() const {
  return max<unsigned long>(
      10, (_config.onMs * 100UL) / max<uint16_t>(1, _config.speedPercent));
}

uint16_t PatternManager::effectiveOffMs() const {
  return max<unsigned long>(
      10, (_config.offMs * 100UL) / max<uint16_t>(1, _config.speedPercent));
}

uint16_t PatternManager::effectiveSeriesPauseMs() const {
  return max<unsigned long>(
      10,
      (_config.seriesPauseMs * 100UL) / max<uint16_t>(1, _config.speedPercent));
}

uint8_t PatternManager::pulsesPerSeries() const {
  switch (_config.mode) {
    case Mode::SingleFlash:
      return max<uint16_t>(1, _config.repeat);
    case Mode::DoubleFlash:
      return 2;
    case Mode::Strobe:
      return max<uint16_t>(1, _config.repeat);
    case Mode::Alternate:
    case Mode::Sequence:
      return max<uint8_t>(1, _config.sequenceLength);
    default:
      return 1;
  }
}

uint8_t PatternManager::currentSequenceChannel() const {
  return _config.sequence[_sequenceIndex];
}

uint8_t PatternManager::buildDefaultSequence(uint32_t channelMask) {
  uint8_t count = 0;
  for (uint8_t i = 0; i < Config::kChannelCount; ++i) {
    if ((channelMask & (1UL << i)) == 0) {
      continue;
    }
    _config.sequence[count++] = i;
  }
  return count;
}
