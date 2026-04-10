import 'dart:convert';

enum LightDeviceType {
  strobe,
  beacon,
  leftSide,
  rightSide,
  front,
  rear,
  flood,
  auxiliary,
  custom,
}

class LightDevice {
  const LightDevice({
    required this.id,
    required this.name,
    required this.type,
    required this.channel,
    required this.group,
    required this.enabled,
    required this.inverted,
    required this.brightness,
    required this.mode,
    required this.channelCount,
    required this.primaryOutput,
  });

  final String id;
  final String name;
  final LightDeviceType type;
  final int channel;
  final String group;
  final bool enabled;
  final bool inverted;
  final int brightness;
  final String mode;
  final int channelCount;
  final int primaryOutput;

  LightDevice copyWith({
    String? id,
    String? name,
    LightDeviceType? type,
    int? channel,
    String? group,
    bool? enabled,
    bool? inverted,
    int? brightness,
    String? mode,
    int? channelCount,
    int? primaryOutput,
  }) {
    return LightDevice(
      id: id ?? this.id,
      name: name ?? this.name,
      type: type ?? this.type,
      channel: channel ?? this.channel,
      group: group ?? this.group,
      enabled: enabled ?? this.enabled,
      inverted: inverted ?? this.inverted,
      brightness: brightness ?? this.brightness,
      mode: mode ?? this.mode,
      channelCount: channelCount ?? this.channelCount,
      primaryOutput: primaryOutput ?? this.primaryOutput,
    );
  }

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'name': name,
      'type': type.name,
      'channel': channel,
      'group': group,
      'enabled': enabled,
      'inverted': inverted,
      'brightness': brightness,
      'mode': mode,
      'channelCount': channelCount,
      'primaryOutput': primaryOutput,
    };
  }

  factory LightDevice.fromMap(Map<String, dynamic> map) {
    return LightDevice(
      id: map['id'] as String,
      name: map['name'] as String,
      type: LightDeviceType.values.firstWhere(
        (value) => value.name == map['type'],
        orElse: () => LightDeviceType.custom,
      ),
      channel: (map['channel'] as num?)?.toInt() ?? 1,
      group: map['group'] as String? ?? 'General',
      enabled: map['enabled'] as bool? ?? true,
      inverted: map['inverted'] as bool? ?? false,
      brightness: (map['brightness'] as num?)?.toInt() ?? 255,
      mode: map['mode'] as String? ?? 'steady',
      channelCount: (map['channelCount'] as num?)?.toInt() ?? 1,
      primaryOutput: (map['primaryOutput'] as num?)?.toInt() ?? 1,
    );
  }

  String toJson() => jsonEncode(toMap());
}
