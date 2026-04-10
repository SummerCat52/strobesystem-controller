import 'dart:convert';

import 'package:flutter_test/flutter_test.dart';
import 'package:strobe_controller_mobile/models/controller_profile.dart';
import 'package:strobe_controller_mobile/models/light_device.dart';
import 'package:strobe_controller_mobile/models/light_group.dart';
import 'package:strobe_controller_mobile/models/pattern_config.dart';

void main() {
  group('LightDevice', () {
    test('serializes and deserializes with fallbacks', () {
      const device = LightDevice(
        id: 'd1',
        name: 'Front Left',
        type: LightDeviceType.front,
        channel: 1,
        group: 'Front',
        enabled: true,
        inverted: false,
        brightness: 200,
        mode: 'burst',
        channelCount: 1,
        primaryOutput: 12,
      );

      final decoded = LightDevice.fromMap(device.toMap());
      expect(decoded.name, 'Front Left');
      expect(decoded.type, LightDeviceType.front);

      final fallback = LightDevice.fromMap(<String, dynamic>{
        'id': 'x',
        'name': 'Unknown',
        'type': 'does-not-exist',
      });
      expect(fallback.type, LightDeviceType.custom);
      expect(fallback.channel, 1);
      expect(fallback.brightness, 255);
    });
  });

  group('LightGroup', () {
    test('json map round-trip keeps values', () {
      const group = LightGroup(
        id: 'g1',
        name: 'Rear',
        colorHex: '#112233',
        deviceIds: ['d2', 'd3'],
      );
      final parsed = LightGroup.fromMap(jsonDecode(group.toJson()) as Map<String, dynamic>);
      expect(parsed.name, group.name);
      expect(parsed.deviceIds, group.deviceIds);
    });
  });

  group('PatternConfig', () {
    test('copyWith overrides selected fields', () {
      const pattern = PatternConfig(
        id: 'p1',
        name: 'Fast',
        speed: 1.0,
        pauseMs: 120,
        syncEnabled: true,
        alternating: false,
        randomMode: false,
      );

      final updated = pattern.copyWith(speed: 2.0, alternating: true);
      expect(updated.speed, 2.0);
      expect(updated.alternating, isTrue);
      expect(updated.name, 'Fast');
    });
  });

  group('ControllerProfile', () {
    test('fromMap restores nested entities and defaults', () {
      final map = <String, dynamic>{
        'name': 'Night profile',
        'devices': [
          {
            'id': 'd1',
            'name': 'Beacon',
            'type': 'beacon',
            'channel': 2,
            'group': 'Top',
            'enabled': true,
            'inverted': false,
            'brightness': 255,
            'mode': 'steady',
            'channelCount': 1,
            'primaryOutput': 13,
          },
        ],
        'groups': [
          {
            'id': 'top',
            'name': 'Top',
            'colorHex': '#ffaa00',
            'deviceIds': ['d1'],
          },
        ],
        'patterns': [
          {
            'id': 'pat1',
            'name': 'Pulse',
            'speed': 1.5,
            'pauseMs': 90,
            'syncEnabled': true,
            'alternating': true,
            'randomMode': false,
          },
        ],
        'lastUpdated': '2026-04-10T12:00:00.000Z',
      };

      final profile = ControllerProfile.fromMap(map);
      expect(profile.name, 'Night profile');
      expect(profile.devices.single.type, LightDeviceType.beacon);
      expect(profile.groups.single.name, 'Top');
      expect(profile.patterns.single.alternating, isTrue);
    });

    test('invalid date falls back to now', () {
      final before = DateTime.now();
      final profile = ControllerProfile.fromMap(<String, dynamic>{
        'name': 'Fallback',
        'lastUpdated': 'bad-date',
      });
      final after = DateTime.now();
      expect(profile.lastUpdated.isBefore(before), isFalse);
      expect(profile.lastUpdated.isAfter(after), isFalse);
    });
  });
}
