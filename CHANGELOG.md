# Changelog

## Unreleased
- Added a dedicated Russian LED-only bench test guide for safe ESP32 validation before connecting MOSFETs, relays, 12V loads, or vehicle wiring.
- Added an example LED bench profile JSON matching the default 8-channel firmware GPIO map.
- Added a regression test that validates the LED bench profile against the default firmware GPIO map.

## 0.2.0 - 2026-04-10

### Added
- Extended unit tests for:
  - command protocol codec,
  - model serialization/deserialization fallbacks,
  - app state behavior and profile persistence.
- Integration test scaffold under `integration_test/app_flow_test.dart`.
- Test support fakes under `test/support/fakes.dart`.
- Project testing documentation in `docs/TESTING.md`.

### Changed
- BLE app-side service now uses separate command and status characteristics:
  - command write UUID `...1002`,
  - status notify UUID `...1003`.
- App state now:
  - processes disconnect events to reset live connection status,
  - avoids mock devices in non-mock mode bootstrap,
  - blocks manual controls when disconnected in live mode,
  - avoids sending empty/placeholder commands.
- Firmware safety keepalive refresh moved after successful command parsing.

### Fixed
- Resolved UI text encoding artifact in device card metadata row.
- Removed placeholder protocol side effects from profile save/load path.

### Verification
- `flutter analyze` passed.
- `flutter test --coverage` passed.
- Coverage baseline improved with added tests (see `docs/TESTING.md`).
