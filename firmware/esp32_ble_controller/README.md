# ESP32 BLE Strobe Controller Firmware

## Libraries
- ESP32 Arduino core
- `BLEDevice.h`
- `BLEServer.h`
- `BLEUtils.h`
- `BLE2902.h`

## BLE UUIDs
- Service: `5E7A1001-0000-4C0A-B001-112233445566`
- Command: `5E7A1002-0000-4C0A-B001-112233445566`
- Status: `5E7A1003-0000-4C0A-B001-112233445566`

## BLE Name
- `ESP32-StrobeCtrl`

## Commands
- `PING`
- `STATUS`
- `STOP`
- `ALL_OFF`
- `FrontLeft=ON`
- `RearRight=OFF`
- `MODE=ON;CH=FrontLeft,RearRight`
- `MODE=OFF;GROUP=REAR`
- `MODE=SINGLE_FLASH;CH=FrontLeft;ON=120;OFF=180;REP=3`
- `MODE=DOUBLE_FLASH;CH=Beacon;ON=70;OFF=70;PAUSE=250`
- `MODE=STROBE;CH=FrontLeft,RearRight;ON=80;OFF=80;REP=5;PAUSE=300`
- `MODE=ALTERNATE;CH=FrontLeft,FrontRight,RearLeft,RearRight;ON=90;OFF=90`
- `MODE=SEQUENCE;ORDER=FrontLeft,FrontRight,RearRight,RearLeft;ON=120;OFF=80;PAUSE=300`

## Wiring
ESP32 GPIO must drive only logic-level inputs of MOSFETs, transistor stages,
relay drivers, or optocouplers.

- use a shared GND between ESP32 and the driver stage
- do not connect lamps or inductive loads directly to ESP32 GPIO
- add flyback diodes for relays and other inductive loads
- use fused automotive 12V input and a DC-DC supply for ESP32
