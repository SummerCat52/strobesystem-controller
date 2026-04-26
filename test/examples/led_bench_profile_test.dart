import 'dart:convert';
import 'dart:io';

import 'package:flutter_test/flutter_test.dart';
import 'package:strobe_controller_mobile/models/controller_profile.dart';
import 'package:strobe_controller_mobile/models/light_device.dart';

void main() {
  test('LED bench profile matches firmware GPIO map', () {
    final file = File('examples/led_bench_profile.json');
    final profile = ControllerProfile.fromMap(
      jsonDecode(file.readAsStringSync()) as Map<String, dynamic>,
    );

    expect(profile.name, 'LED Bench Test');
    expect(profile.devices, hasLength(8));

    final outputsByName = {
      for (final device in profile.devices) device.name: device.primaryOutput,
    };
    expect(outputsByName['Front Left LED'], 16);
    expect(outputsByName['Front Right LED'], 17);
    expect(outputsByName['Rear Left LED'], 18);
    expect(outputsByName['Rear Right LED'], 19);
    expect(outputsByName['Side Left LED'], 21);
    expect(outputsByName['Side Right LED'], 22);
    expect(outputsByName['Beacon LED'], 23);
    expect(outputsByName['Flood LED'], 25);

    expect(
      profile.devices.map((device) => device.type),
      containsAll([
        LightDeviceType.front,
        LightDeviceType.rear,
        LightDeviceType.leftSide,
        LightDeviceType.rightSide,
        LightDeviceType.beacon,
        LightDeviceType.flood,
      ]),
    );
    expect(profile.devices.every((device) => device.channelCount == 1), isTrue);
    expect(profile.devices.every((device) => device.inverted == false), isTrue);
  });
}
