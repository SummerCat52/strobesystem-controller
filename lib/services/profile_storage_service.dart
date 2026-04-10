import 'dart:convert';

import 'package:shared_preferences/shared_preferences.dart';

import '../models/controller_profile.dart';

class ProfileStorageService {
  static const _profilesKey = 'controller_profiles';

  Future<List<ControllerProfile>> loadProfiles() async {
    final prefs = await SharedPreferences.getInstance();
    final raw = prefs.getStringList(_profilesKey) ?? const [];

    return raw
        .map((item) => ControllerProfile.fromMap(
              jsonDecode(item) as Map<String, dynamic>,
            ))
        .toList();
  }

  Future<void> saveProfiles(List<ControllerProfile> profiles) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setStringList(
      _profilesKey,
      profiles.map((item) => item.toJson()).toList(),
    );
  }
}
