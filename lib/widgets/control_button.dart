import 'package:flutter/material.dart';

class ControlButton extends StatelessWidget {
  const ControlButton({
    super.key,
    required this.label,
    required this.onPressed,
    required this.active,
  });

  final String label;
  final VoidCallback onPressed;
  final bool active;

  @override
  Widget build(BuildContext context) {
    final colorScheme = Theme.of(context).colorScheme;

    return FilledButton(
      style: FilledButton.styleFrom(
        backgroundColor: active
            ? colorScheme.primary
            : colorScheme.surfaceContainerHighest,
        foregroundColor:
            active ? colorScheme.onPrimary : colorScheme.onSurface,
        minimumSize: const Size.fromHeight(88),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(18),
        ),
      ),
      onPressed: onPressed,
      child: Text(
        label,
        textAlign: TextAlign.center,
      ),
    );
  }
}
