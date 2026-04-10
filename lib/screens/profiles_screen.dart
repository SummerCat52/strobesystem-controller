import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../providers/app_state.dart';
import '../widgets/section_card.dart';

class ProfilesScreen extends StatelessWidget {
  const ProfilesScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final state = context.watch<AppState>();

    return Scaffold(
      appBar: AppBar(title: const Text('Profiles')),
      body: ListView(
        padding: const EdgeInsets.all(16),
        children: [
          Row(
            children: [
              Expanded(
                child: FilledButton(
                  onPressed: () => _showSaveDialog(context),
                  child: const Text('Save Current Profile'),
                ),
              ),
              const SizedBox(width: 12),
              Expanded(
                child: OutlinedButton(
                  onPressed: state.importProfile,
                  child: const Text('Import JSON'),
                ),
              ),
            ],
          ),
          const SizedBox(height: 16),
          ...state.profiles.map(
            (profile) => Padding(
              padding: const EdgeInsets.only(bottom: 12),
              child: SectionCard(
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Text(
                      profile.name,
                      style: Theme.of(context).textTheme.titleMedium,
                    ),
                    const SizedBox(height: 4),
                    Text(
                      'Devices ${profile.devices.length} | Updated ${profile.lastUpdated}',
                    ),
                    const SizedBox(height: 12),
                    Row(
                      children: [
                        Expanded(
                          child: OutlinedButton(
                            onPressed: () => state.loadProfile(profile.name),
                            child: const Text('Load'),
                          ),
                        ),
                        const SizedBox(width: 8),
                        Expanded(
                          child: FilledButton.tonal(
                            onPressed: () => state.deleteProfile(profile.name),
                            child: const Text('Delete'),
                          ),
                        ),
                        IconButton(
                          onPressed: () => state.exportProfile(profile),
                          icon: const Icon(Icons.ios_share_outlined),
                        ),
                        IconButton(
                          onPressed: () {
                            final json = const JsonEncoder.withIndent('  ')
                                .convert(profile.toMap());
                            showDialog<void>(
                              context: context,
                              builder: (_) => AlertDialog(
                                title: const Text('Profile JSON'),
                                content: SingleChildScrollView(
                                  child: SelectableText(json),
                                ),
                              ),
                            );
                          },
                          icon: const Icon(Icons.code_outlined),
                        ),
                      ],
                    ),
                  ],
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }

  Future<void> _showSaveDialog(BuildContext context) async {
    final controller = TextEditingController();
    final state = context.read<AppState>();

    await showDialog<void>(
      context: context,
      builder: (_) => AlertDialog(
        title: const Text('Save Profile'),
        content: TextField(
          controller: controller,
          decoration: const InputDecoration(labelText: 'Profile name'),
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.of(context).pop(),
            child: const Text('Cancel'),
          ),
          FilledButton(
            onPressed: () async {
              if (controller.text.trim().isEmpty) {
                return;
              }
              await state.saveCurrentProfile(controller.text.trim());
              if (context.mounted) {
                Navigator.of(context).pop();
              }
            },
            child: const Text('Save'),
          ),
        ],
      ),
    );
  }
}
