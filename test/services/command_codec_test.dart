import 'package:flutter_test/flutter_test.dart';
import 'package:strobe_controller_mobile/services/command_codec.dart';

void main() {
  group('ControllerCommandCodec', () {
    final codec = ControllerCommandCodec();

    test('builds strobe command with required parameters', () {
      final command = codec.strobe(
        channels: const ['FrontLeft', 'RearRight'],
        onMs: 80,
        offMs: 90,
        repeat: 4,
        seriesPauseMs: 250,
      );

      expect(
        command,
        'MODE=STROBE;CH=FrontLeft,RearRight;ON=80;OFF=90;REP=4;PAUSE=250',
      );
    });

    test('builds alternate and sequence commands', () {
      expect(
        codec.alternate(
          channels: const ['FrontLeft', 'FrontRight'],
          onMs: 60,
          offMs: 60,
          seriesPauseMs: 100,
        ),
        'MODE=ALTERNATE;CH=FrontLeft,FrontRight;ON=60;OFF=60;PAUSE=100',
      );

      expect(
        codec.sequence(
          order: const ['FrontLeft', 'RearLeft', 'Beacon'],
          onMs: 50,
          offMs: 70,
          seriesPauseMs: 120,
        ),
        'MODE=SEQUENCE;ORDER=FrontLeft,RearLeft,Beacon;ON=50;OFF=70;PAUSE=120',
      );
    });

    test('parses incoming message into command and payload', () {
      expect(
        codec.parseIncoming('STATUS:CONNECTED'),
        {'command': 'STATUS', 'payload': 'CONNECTED'},
      );
      expect(
        codec.parseIncoming('PING'),
        {'command': 'PING', 'payload': ''},
      );
    });

    test('empty placeholders are explicit for unsupported firmware commands', () {
      expect(codec.saveProfile(), isEmpty);
      expect(codec.loadProfile('night-run'), isEmpty);
      expect(codec.speed(8), isEmpty);
      expect(codec.toggle('FrontLeft'), isEmpty);
      expect(codec.deleteDevice('id-1'), isEmpty);
    });
  });
}
