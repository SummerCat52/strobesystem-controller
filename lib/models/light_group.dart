import 'dart:convert';

class LightGroup {
  const LightGroup({
    required this.id,
    required this.name,
    required this.colorHex,
    required this.deviceIds,
  });

  final String id;
  final String name;
  final String colorHex;
  final List<String> deviceIds;

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'name': name,
      'colorHex': colorHex,
      'deviceIds': deviceIds,
    };
  }

  factory LightGroup.fromMap(Map<String, dynamic> map) {
    return LightGroup(
      id: map['id'] as String,
      name: map['name'] as String,
      colorHex: map['colorHex'] as String? ?? '#00AEEF',
      deviceIds: List<String>.from(map['deviceIds'] as List? ?? const []),
    );
  }

  String toJson() => jsonEncode(toMap());
}
