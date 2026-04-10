# Пошаговый гайд (релиз 2026-04-10)

Этот гайд для запуска системы `StagePatch + ESP32-StrobeCtrl` с нуля.

## 1. Что отправлять другу
- Полный пакет: `dist/StagePatch_friend_package.zip`
- Только приложение (Android): `dist/StagePatch-release.apk`
- Только прошивка: `dist/ESP32_firmware_only.zip`

## 2. Что уже зашито в прошивке
- BLE имя: `ESP32-StrobeCtrl`
- Service UUID: `5E7A1001-0000-4C0A-B001-112233445566`
- Command UUID (write): `5E7A1002-0000-4C0A-B001-112233445566`
- Status UUID (notify/read): `5E7A1003-0000-4C0A-B001-112233445566`

Каналы:
- `FrontLeft` -> GPIO16
- `FrontRight` -> GPIO17
- `RearLeft` -> GPIO18
- `RearRight` -> GPIO19
- `SideLeft` -> GPIO21
- `SideRight` -> GPIO22
- `Beacon` -> GPIO23
- `Flood` -> GPIO25

Настройка пинов/инверсии: `firmware/esp32_ble_controller/Config.h`.

## 3. Прошивка ESP32
1. Установи Arduino IDE и пакет `ESP32 by Espressif Systems`.
2. Открой файл `firmware/esp32_ble_controller/esp32_ble_controller.ino`.
3. Выбери плату `ESP32 Dev Module` и нужный COM-порт.
4. Нажми `Upload`.
5. В `Serial Monitor` выставь `115200` и проверь, что есть `READY`.

## 4. Установка APK
1. Скопируй `dist/StagePatch-release.apk` на Android.
2. Установи APK (разреши установку из неизвестного источника, если спросит).
3. Включи Bluetooth.
4. Дай приложению разрешения Bluetooth и геолокации.

## 5. Подключение к контроллеру
1. Открой вкладку `Connect`.
2. Нажми `Scan Controllers`.
3. Выбери `ESP32-StrobeCtrl`.
4. Нажми `Connect`.

Если видишь TV/наушники, не подключай их. Нужен именно `ESP32-StrobeCtrl`.

## 6. Как добавлять устройства во вкладке Devices
`Devices` -> `Add Device` -> заполни поля из своей реальной схемы:
- `Name`: удобное имя (`Front Left`, `Beacon Bar`)
- `Type`: назначение (`front`, `rear`, `beacon`, `flood`, и т.д.)
- `Channel`: логический канал
- `Primary Output`: GPIO/выход ESP32, куда подключен драйвер
- `Group`: группа (`FRONT`, `REAR`, `SIDE`)
- `Mode`, `Inverted`, `Brightness`: по твоей схеме

Важно: данные для `Devices` берутся из твоей проводки, а не “из приложения”.

## 7. Быстрая проверка
1. Перейди в `Control`.
2. Нажми `Front Left` -> должен включиться соответствующий выход.
3. Нажми `Front Left` ещё раз -> должен выключиться.
4. Нажми `All Off` -> всё должно выключиться.

## 8. Безопасность (обязательно)
- ESP32 управляет только логикой, не силовой нагрузкой напрямую.
- Используй MOSFET/драйвер/реле.
- Обязательно общий `GND` между ESP32 и силовой частью.
- Питание через DC-DC 12V -> 5V/3.3V.
- Нужны предохранитель и защита от выбросов.

## 9. Если не работает
Проверь по порядку:
1. Прошивка загрузилась без ошибок.
2. В Serial есть `READY`.
3. Телефон видит `ESP32-StrobeCtrl`.
4. Разрешения Bluetooth/Location выданы.
5. Есть общий GND.
6. Нагрузка подключена через драйвер, не напрямую к GPIO.
