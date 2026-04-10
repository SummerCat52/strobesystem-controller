import 'dart:convert';

import 'light_device.dart';
import 'light_group.dart';
import 'pattern_config.dart';

class ControllerProfile {
  const ControllerProfile({
    required this.name,
    required this.devices,
    required this.groups,
    required this.patterns,
    required this.lastUpdated,
  });

  final String name;
  final List<LightDevice> devices;
  final List<LightGroup> groups;
  final List<PatternConfig> patterns;
  final DateTime lastUpdated;

  Map<String, dynamic> toMap() {
    return {
      'name': name,
      'devices': devices.map((item) => item.toMap()).toList(),
      'groups': groups.map((item) => item.toMap()).toList(),
      'patterns': patterns.map((item) => item.toMap()).toList(),
      'lastUpdated': lastUpdated.toIso8601String(),
    };
  }

  factory ControllerProfile.fromMap(Map<String, dynamic> map) {
    return ControllerProfile(
      name: map['name'] as String,
      devices: (map['devices'] as List? ?? const [])
          .map((item) => LightDevice.fromMap(item as Map<String, dynamic>))
          .toList(),
      groups: (map['groups'] as List? ?? const [])
          .map((item) => LightGroup.fromMap(item as Map<String, dynamic>))
          .toList(),
      patterns: (map['patterns'] as List? ?? const [])
          .map((item) => PatternConfig(
                id: item['id'] as String,
                name: item['name'] as String,
                speed: (item['speed'] as num?)?.toDouble() ?? 1,
                pauseMs: (item['pauseMs'] as num?)?.toInt() ?? 100,
                syncEnabled: item['syncEnabled'] as bool? ?? true,
                alternating: item['alternating'] as bool? ?? false,
                randomMode: item['randomMode'] as bool? ?? false,
              ))
          .toList(),
      lastUpdated: DateTime.tryParse(map['lastUpdated'] as String? ?? '') ??
          DateTime.now(),
    );
  }

  String toJson() => jsonEncode(toMap());
}
