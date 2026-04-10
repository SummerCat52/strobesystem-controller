import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../providers/app_state.dart';
import '../widgets/section_card.dart';

class DiagnosticsScreen extends StatelessWidget {
  const DiagnosticsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final state = context.watch<AppState>();

    return Scaffold(
      appBar: AppBar(
        title: const Text('Diagnostics'),
        actions: [
          IconButton(
            onPressed: state.clearLogs,
            icon: const Icon(Icons.delete_sweep_outlined),
          ),
        ],
      ),
      body: ListView(
        padding: const EdgeInsets.all(16),
        children: [
          SectionCard(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  'Connection Snapshot',
                  style: Theme.of(context).textTheme.titleMedium,
                ),
                const SizedBox(height: 12),
                Text('Status: ${state.connection.status.name}'),
                Text('Controller: ${state.connection.controllerName}'),
                Text('Signal: ${state.connection.signalStrength}%'),
                Text('Mock mode: ${state.useMockMode ? 'on' : 'off'}'),
                Text('Devices: ${state.devices.length}'),
                Text('Profiles: ${state.profiles.length}'),
              ],
            ),
          ),
          const SizedBox(height: 16),
          SectionCard(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  'Recent Logs',
                  style: Theme.of(context).textTheme.titleMedium,
                ),
                const SizedBox(height: 12),
                if (state.logs.isEmpty)
                  const Text('No logs yet.')
                else
                  ...state.logs.map(
                    (entry) => Padding(
                      padding: const EdgeInsets.only(bottom: 8),
                      child: SelectableText(entry),
                    ),
                  ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
