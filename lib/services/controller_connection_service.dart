import '../models/connection_state_model.dart';

abstract class ControllerConnectionService {
  Future<List<String>> scan();
  Future<ConnectionStateModel> connect(String controllerId);
  Future<ConnectionStateModel> disconnect();
  Future<void> sendCommand(String command);
  Stream<String> get incomingMessages;
}
