import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../providers/app_state.dart';
import '../widgets/section_card.dart';

class DiagnosticsScreen extends StatelessWidget {
  const DiagnosticsScreen({super.key});

  static const _pinScanCommand =
      'MODE=SEQUENCE;ORDER=FrontLeft,FrontRight,RearLeft,RearRight,SideLeft,SideRight,Beacon,Flood;ON=3000;OFF=500;PAUSE=1000';

  static const _gpioRows = <_GpioRow>[
    _GpioRow('Front Left', 'FrontLeft', 'GPIO16 / D16'),
    _GpioRow('Front Right', 'FrontRight', 'GPIO17 / D17'),
    _GpioRow('Rear Left', 'RearLeft', 'GPIO18 / D18'),
    _GpioRow('Rear Right', 'RearRight', 'GPIO19 / D19'),
    _GpioRow('Side Left', 'SideLeft', 'GPIO21 / D21'),
    _GpioRow('Side Right', 'SideRight', 'GPIO22 / D22'),
    _GpioRow('Beacon', 'Beacon', 'GPIO23 / D23'),
    _GpioRow('Flood', 'Flood', 'GPIO25 / D25'),
  ];

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
                  'Hardware Test',
                  style: Theme.of(context).textTheme.titleMedium,
                ),
                const SizedBox(height: 12),
                Wrap(
                  spacing: 8,
                  runSpacing: 8,
                  children: [
                    FilledButton.tonal(
                      onPressed: () =>
                          state.sendRawCommand('PING', critical: true),
                      child: const Text('PING'),
                    ),
                    FilledButton.tonal(
                      onPressed: () =>
                          state.sendRawCommand('STATUS', critical: true),
                      child: const Text('STATUS'),
                    ),
                    FilledButton(
                      onPressed: () => state.sendRawCommand(
                        _pinScanCommand,
                        critical: true,
                      ),
                      child: const Text('Pin Scan'),
                    ),
                    FilledButton.tonalIcon(
                      onPressed: () =>
                          state.triggerControl('all_off', 'ALL_OFF'),
                      icon: const Icon(Icons.power_settings_new),
                      label: const Text('All Off'),
                    ),
                  ],
                ),
                const SizedBox(height: 12),
                const Text(
                  'Pin Scan turns each output on for 3 seconds. Keep black probe on GND and move red probe across the listed pins.',
                ),
              ],
            ),
          ),
          const SizedBox(height: 16),
          SectionCard(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  'GPIO Map',
                  style: Theme.of(context).textTheme.titleMedium,
                ),
                const SizedBox(height: 12),
                ..._gpioRows.map(
                  (row) => Padding(
                    padding: const EdgeInsets.only(bottom: 8),
                    child: Row(
                      children: [
                        Expanded(child: Text(row.label)),
                        Text(row.command),
                        const SizedBox(width: 12),
                        Text(
                          row.pin,
                          style: const TextStyle(fontWeight: FontWeight.w700),
                        ),
                      ],
                    ),
                  ),
                ),
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

class _GpioRow {
  const _GpioRow(this.label, this.command, this.pin);

  final String label;
  final String command;
  final String pin;
}
