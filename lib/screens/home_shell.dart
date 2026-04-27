import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../providers/app_state.dart';
import 'connection_screen.dart';
import 'devices_screen.dart';
import 'diagnostics_screen.dart';
import 'manual_control_screen.dart';
import 'patterns_screen.dart';
import 'profiles_screen.dart';
import 'settings_screen.dart';

class HomeShell extends StatelessWidget {
  const HomeShell({super.key});

  static const _screens = <Widget>[
    ConnectionScreen(),
    DevicesScreen(),
    ManualControlScreen(),
    PatternsScreen(),
    ProfilesScreen(),
    DiagnosticsScreen(),
    SettingsScreen(),
  ];

  @override
  Widget build(BuildContext context) {
    final state = context.watch<AppState>();

    return Scaffold(
      body: SafeArea(
        child: IndexedStack(
          index: state.currentIndex,
          children: _screens,
        ),
      ),
      bottomNavigationBar: NavigationBar(
        selectedIndex: state.currentIndex,
        onDestinationSelected: state.setTab,
        destinations: const [
          NavigationDestination(
            icon: Icon(Icons.bluetooth_searching),
            label: 'Connect',
          ),
          NavigationDestination(
            icon: Icon(Icons.lightbulb_outline),
            label: 'Devices',
          ),
          NavigationDestination(
            icon: Icon(Icons.gamepad_outlined),
            label: 'Control',
          ),
          NavigationDestination(
            icon: Icon(Icons.auto_awesome_motion_outlined),
            label: 'Patterns',
          ),
          NavigationDestination(
            icon: Icon(Icons.folder_outlined),
            label: 'Profiles',
          ),
          NavigationDestination(
            icon: Icon(Icons.monitor_heart_outlined),
            label: 'Diag',
          ),
          NavigationDestination(
            icon: Icon(Icons.settings_outlined),
            label: 'Settings',
          ),
        ],
      ),
    );
  }
}
