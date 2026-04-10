import 'dart:convert';

import 'package:file_picker/file_picker.dart';
import 'package:share_plus/share_plus.dart';

import '../models/controller_profile.dart';

class ProfileExchangeService {
  Future<ControllerProfile?> importProfile() async {
    final result = await FilePicker.platform.pickFiles(
      type: FileType.custom,
      allowedExtensions: const ['json'],
      withData: true,
    );

    final bytes = result?.files.single.bytes;
    if (bytes == null) {
      return null;
    }

    final decoded = jsonDecode(utf8.decode(bytes)) as Map<String, dynamic>;
    return ControllerProfile.fromMap(decoded);
  }

  Future<void> exportProfile(ControllerProfile profile) {
    final json = const JsonEncoder.withIndent('  ').convert(profile.toMap());
    return Share.shareXFiles(
      [
        XFile.fromData(
          utf8.encode(json),
          mimeType: 'application/json',
          name: '${profile.name}.json',
        ),
      ],
      text: 'Controller profile export',
    );
  }
}
