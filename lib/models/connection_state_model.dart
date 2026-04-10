enum ControllerConnectionStatus {
  disconnected,
  scanning,
  connecting,
  connected,
  error,
}

class ConnectionStateModel {
  const ConnectionStateModel({
    required this.status,
    required this.controllerName,
    required this.signalStrength,
    required this.message,
    required this.mockMode,
  });

  final ControllerConnectionStatus status;
  final String controllerName;
  final int signalStrength;
  final String message;
  final bool mockMode;

  ConnectionStateModel copyWith({
    ControllerConnectionStatus? status,
    String? controllerName,
    int? signalStrength,
    String? message,
    bool? mockMode,
  }) {
    return ConnectionStateModel(
      status: status ?? this.status,
      controllerName: controllerName ?? this.controllerName,
      signalStrength: signalStrength ?? this.signalStrength,
      message: message ?? this.message,
      mockMode: mockMode ?? this.mockMode,
    );
  }

  static const initial = ConnectionStateModel(
    status: ControllerConnectionStatus.disconnected,
    controllerName: 'No controller',
    signalStrength: 0,
    message: 'Ready to scan',
    mockMode: true,
  );
}
