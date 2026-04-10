import 'dart:io';

import 'package:permission_handler/permission_handler.dart';

class BlePermissionService {
  Future<void> ensurePermissions() async {
    if (!Platform.isAndroid && !Platform.isIOS) {
      return;
    }

    final permissions = <Permission>[
      Permission.bluetoothScan,
      Permission.bluetoothConnect,
    ];

    if (Platform.isAndroid) {
      permissions.add(Permission.locationWhenInUse);
    }

    final statuses = await permissions.request();
    final denied = statuses.entries
        .where((entry) => !entry.value.isGranted)
        .map((entry) => entry.key.toString())
        .toList();

    if (denied.isNotEmpty) {
      throw StateError(
        'Bluetooth permissions are required: ${denied.join(', ')}',
      );
    }
  }
}
