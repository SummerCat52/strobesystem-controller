import 'dart:convert';

class PatternConfig {
  const PatternConfig({
    required this.id,
    required this.name,
    required this.speed,
    required this.pauseMs,
    required this.syncEnabled,
    required this.alternating,
    required this.randomMode,
  });

  final String id;
  final String name;
  final double speed;
  final int pauseMs;
  final bool syncEnabled;
  final bool alternating;
  final bool randomMode;

  PatternConfig copyWith({
    String? id,
    String? name,
    double? speed,
    int? pauseMs,
    bool? syncEnabled,
    bool? alternating,
    bool? randomMode,
  }) {
    return PatternConfig(
      id: id ?? this.id,
      name: name ?? this.name,
      speed: speed ?? this.speed,
      pauseMs: pauseMs ?? this.pauseMs,
      syncEnabled: syncEnabled ?? this.syncEnabled,
      alternating: alternating ?? this.alternating,
      randomMode: randomMode ?? this.randomMode,
    );
  }

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'name': name,
      'speed': speed,
      'pauseMs': pauseMs,
      'syncEnabled': syncEnabled,
      'alternating': alternating,
      'randomMode': randomMode,
    };
  }

  String toJson() => jsonEncode(toMap());
}
