import '../models/light_device.dart';
import '../models/light_group.dart';
import '../models/pattern_config.dart';

class MockData {
  static List<LightDevice> devices() {
    return const [
      LightDevice(
        id: 'front-left',
        name: 'Front Left Strobe',
        type: LightDeviceType.front,
        channel: 1,
        group: 'Front',
        enabled: true,
        inverted: false,
        brightness: 220,
        mode: 'burst',
        channelCount: 1,
        primaryOutput: 12,
      ),
      LightDevice(
        id: 'beacon',
        name: 'Beacon Bar',
        type: LightDeviceType.beacon,
        channel: 2,
        group: 'Top',
        enabled: true,
        inverted: false,
        brightness: 255,
        mode: 'rotate',
        channelCount: 1,
        primaryOutput: 13,
      ),
      LightDevice(
        id: 'rear-right',
        name: 'Rear Right Flood',
        type: LightDeviceType.rear,
        channel: 3,
        group: 'Rear',
        enabled: false,
        inverted: true,
        brightness: 180,
        mode: 'steady',
        channelCount: 1,
        primaryOutput: 14,
      ),
    ];
  }

  static List<LightGroup> groups() {
    return const [
      LightGroup(
        id: 'front',
        name: 'Front',
        colorHex: '#00AEEF',
        deviceIds: ['front-left'],
      ),
      LightGroup(
        id: 'rear',
        name: 'Rear',
        colorHex: '#FF7043',
        deviceIds: ['rear-right'],
      ),
    ];
  }

  static List<PatternConfig> patterns() {
    return const [
      PatternConfig(
        id: 'pattern-1',
        name: 'Rapid Alternating',
        speed: 1.0,
        pauseMs: 120,
        syncEnabled: true,
        alternating: true,
        randomMode: false,
      ),
      PatternConfig(
        id: 'pattern-2',
        name: 'Random Burst',
        speed: 1.4,
        pauseMs: 90,
        syncEnabled: false,
        alternating: true,
        randomMode: true,
      ),
    ];
  }
}
