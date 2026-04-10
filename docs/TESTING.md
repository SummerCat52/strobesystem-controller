# Testing Guide

## Test Pyramid (Current)
- Unit tests: protocol codec, model serialization, app state logic.
- Integration test: app connect screen flow (`integration_test/app_flow_test.dart`).
- Load-style scenario: burst command throttling in `test/providers/app_state_test.dart`.
- Static analysis: `flutter analyze`.

## Commands
```powershell
flutter analyze
flutter test --coverage
flutter test integration_test
```

Note: `flutter test integration_test` requires a supported target (Android device/emulator configured for this project).

If Flutter is installed through Puro in this workspace:
```powershell
.\.puro-home\.puro\envs\stable\flutter\bin\flutter.bat analyze
.\.puro-home\.puro\envs\stable\flutter\bin\flutter.bat test --coverage
.\.puro-home\.puro\envs\stable\flutter\bin\flutter.bat test integration_test
```

## Coverage
- Coverage is generated to `coverage/lcov.info`.
- Quick summary formula:
  - lines covered = number of `DA:<line>,<hits>` with `hits > 0`
  - lines total = number of all `DA:` entries
  - coverage = `covered / total * 100`

Latest local run (2026-04-10):
- before expansion: `24.37%` (`251/1030`)
- after expansion: `50.78%` (`523/1030`)

## Mutation Analysis (Pragmatic MVP Review)
Automated mutation tooling is not yet wired in CI for this Flutter workspace.  
For MVP hardening, a manual mutation checklist was applied against critical paths:

- Killed mutants:
  - `MODE=STROBE` builder changed to wrong delimiter.
  - `parseIncoming` payload parsing removed.
  - `AppState.triggerControl` guard for disconnected live mode removed.
  - `sendRawCommand` throttle condition inverted.
  - profile save/load unexpectedly sending BLE placeholder commands.

- Surviving/risky mutants (next step):
  - some UI-only text/label changes are not asserted by tests.
  - BLE RSSI normalization boundaries are not yet tested.
  - firmware-side parser mutation tests are not automated.

## Remaining Gaps To Reach Strong Production Confidence
- Hardware-in-loop tests with real ESP32 and automotive power noise.
- Firmware unit tests (native or host-side C++).
- Automated mutation testing pipeline for Flutter and firmware.
- End-to-end regression suite on Android physical device farm.
