# Strobe Controller Mobile MVP

Flutter application and ESP32 firmware for configuring and controlling a strobe-light controller over BLE.

## Core Features
- Offline-first mobile UI with optional mock mode.
- BLE service abstraction that can be replaced later with Bluetooth Serial or Wi-Fi transport.
- Device management: add, edit, delete, enable, disable, and channel/output assignment.
- Large manual control pad for real-time actions.
- Pattern configuration (speed, pause, alternating, random).
- Local profile storage with JSON import/export.
- ESP32 firmware with non-blocking command parsing and fail-safe shutdown.

## Project Structure
- `lib/models`
- `lib/services`
- `lib/providers`
- `lib/screens`
- `lib/widgets`
- `firmware/esp32_ble_controller`
- `test`
- `integration_test`

## Run App
1. Install Flutter (3.41+ recommended).
2. Run `flutter pub get`.
3. Run `flutter run`.
4. On Android BLE runs, grant Bluetooth Scan/Connect and Location permissions.

## BLE Contract
- Device name: `ESP32-StrobeCtrl`
- Service UUID: `5E7A1001-0000-4C0A-B001-112233445566`
- Command characteristic (write): `5E7A1002-0000-4C0A-B001-112233445566`
- Status characteristic (read/notify): `5E7A1003-0000-4C0A-B001-112233445566`

## Supported Command Protocol
- `HELLO`
- `HEARTBEAT`
- `PING`
- `GET_CONFIG`
- `STATUS`
- `STOP`
- `ALL_OFF`
- `SET_GPIO;CH=FrontLeft;GPIO=16`
- `SET_INVERT;CH=Beacon;VALUE=0`
- `SET_FAILSAFE;MS=5000`
- `SAVE_CONFIG`
- `FACTORY_RESET`
- `FrontLeft=ON` / `FrontLeft=OFF` and other channel direct commands
- `MODE=ON;CH=FrontLeft,FrontRight`
- `MODE=OFF;GROUP=REAR`
- `MODE=STROBE;CH=FrontLeft,RearRight;ON=80;OFF=80;REP=5;PAUSE=300`
- `MODE=ALTERNATE;CH=FrontLeft,FrontRight;ON=60;OFF=60;PAUSE=100`
- `MODE=SEQUENCE;ORDER=FrontLeft,RearLeft,Beacon;ON=50;OFF=70;PAUSE=120`

## Quality Gates
- `flutter analyze`
- `flutter test --coverage`
- `flutter test integration_test`

## GitHub Releases
- Push a tag like `v0.1.1` to build and publish a GitHub Release automatically.
- Or run **Build APK and publish release** manually from the GitHub Actions tab.
- Each release contains `StagePatch-release.apk`, ESP32 firmware, and a source package.
- Current release: https://github.com/SummerCat52/strobesystem-controller/releases/tag/v0.1.0
- Russian release guide: `docs/RELEASE_GUIDE_RU.md`

## Hardware Build Guide
- Full Russian wiring and soldering guide: `docs/BUILD_AND_WIRING_GUIDE_RU.md`
- Safe LED-only bench test guide: `docs/LED_BENCH_TEST_GUIDE_RU.md`
- Firmware quick setup guide: `firmware/esp32_ble_controller/SETUP_GUIDE_RU.md`
- Example LED test profile: `examples/led_bench_profile.json`

Detailed test strategy, load scenarios, and mutation notes are documented in:
- `docs/TESTING.md`

Release-level change history is documented in:
- `CHANGELOG.md`
