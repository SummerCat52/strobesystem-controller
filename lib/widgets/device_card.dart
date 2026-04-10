import 'package:flutter/material.dart';

import '../models/light_device.dart';
import 'status_badge.dart';

class DeviceCard extends StatelessWidget {
  const DeviceCard({
    super.key,
    required this.device,
    required this.onEdit,
    required this.onDelete,
    required this.onToggle,
  });

  final LightDevice device;
  final VoidCallback onEdit;
  final VoidCallback onDelete;
  final VoidCallback onToggle;

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              children: [
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        device.name,
                        style: Theme.of(context).textTheme.titleMedium,
                      ),
                      const SizedBox(height: 4),
                      Text(
                        '${device.type.name} - Ch ${device.channel} - Out ${device.primaryOutput}',
                      ),
                    ],
                  ),
                ),
                StatusBadge(
                  label: device.enabled ? 'Active' : 'Muted',
                  color: device.enabled ? Colors.greenAccent : Colors.orange,
                ),
              ],
            ),
            const SizedBox(height: 12),
            Wrap(
              spacing: 8,
              runSpacing: 8,
              children: [
                Chip(label: Text('Group ${device.group}')),
                Chip(label: Text('Mode ${device.mode}')),
                Chip(label: Text('Bright ${device.brightness}')),
                if (device.inverted) const Chip(label: Text('Inverted')),
              ],
            ),
            const SizedBox(height: 12),
            Row(
              children: [
                Expanded(
                  child: OutlinedButton(
                    onPressed: onEdit,
                    child: const Text('Edit'),
                  ),
                ),
                const SizedBox(width: 12),
                Expanded(
                  child: FilledButton.tonal(
                    onPressed: onToggle,
                    child: Text(device.enabled ? 'Disable' : 'Enable'),
                  ),
                ),
                IconButton(
                  onPressed: onDelete,
                  icon: const Icon(Icons.delete_outline),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
