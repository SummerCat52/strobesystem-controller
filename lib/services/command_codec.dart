import '../models/light_device.dart';

class ControllerCommandCodec {
  static const frontChannels = ['FrontLeft', 'FrontRight'];
  static const rearChannels = ['RearLeft', 'RearRight'];
  static const sideChannels = ['SideLeft', 'SideRight'];
  static const allChannels = [
    'FrontLeft',
    'FrontRight',
    'RearLeft',
    'RearRight',
    'SideLeft',
    'SideRight',
    'Beacon',
    'Flood',
  ];

  String ping() => 'PING';
  String getConfig() => 'STATUS';
  String saveProfile() => '';
  String loadProfile(String name) => '';
  String allOff() => 'ALL_OFF';
  String pattern(String patternId) => 'MODE=STROBE;GROUP=ALL;ON=80;OFF=80;REP=5;PAUSE=300';
  String speed(int value) => '';
  String toggle(String id) => '';
  String on(String id) => '$id=ON';
  String off(String id) => '$id=OFF';
  String deleteDevice(String id) => '';
  String setDevice(LightDevice device) => '';

  String setGroup(String groupName, bool on) =>
      'MODE=${on ? 'ON' : 'OFF'};GROUP=$groupName';

  String setChannels(List<String> channels, bool on) =>
      'MODE=${on ? 'ON' : 'OFF'};CH=${channels.join(',')}';

  String strobe({
    required List<String> channels,
    required int onMs,
    required int offMs,
    required int repeat,
    required int seriesPauseMs,
  }) {
    return 'MODE=STROBE;CH=${channels.join(',')};ON=$onMs;OFF=$offMs;REP=$repeat;PAUSE=$seriesPauseMs';
  }

  String alternate({
    required List<String> channels,
    required int onMs,
    required int offMs,
    required int seriesPauseMs,
  }) {
    return 'MODE=ALTERNATE;CH=${channels.join(',')};ON=$onMs;OFF=$offMs;PAUSE=$seriesPauseMs';
  }

  String sequence({
    required List<String> order,
    required int onMs,
    required int offMs,
    required int seriesPauseMs,
  }) {
    return 'MODE=SEQUENCE;ORDER=${order.join(',')};ON=$onMs;OFF=$offMs;PAUSE=$seriesPauseMs';
  }

  Map<String, String> parseIncoming(String raw) {
    final parts = raw.split(':');
    return {
      'command': parts.first.trim(),
      'payload': parts.length > 1 ? parts.sublist(1).join(':').trim() : '',
    };
  }
}
