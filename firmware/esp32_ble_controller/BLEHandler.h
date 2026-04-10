#pragma once

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "Config.h"

class BLEHandlerListener {
 public:
  virtual ~BLEHandlerListener() = default;
  virtual void onBleCommand(const String& command) = 0;
  virtual void onBleConnected() = 0;
  virtual void onBleDisconnected() = 0;
};

class BLEHandler {
 public:
  explicit BLEHandler(BLEHandlerListener& listener);

  void begin();
  void updateStatus(const String& statusText);

 private:
  class ServerCallbacks : public BLEServerCallbacks {
   public:
    explicit ServerCallbacks(BLEHandler& owner) : _owner(owner) {}
    void onConnect(BLEServer* server) override;
    void onDisconnect(BLEServer* server) override;

   private:
    BLEHandler& _owner;
  };

  class CommandCallbacks : public BLECharacteristicCallbacks {
   public:
    explicit CommandCallbacks(BLEHandler& owner) : _owner(owner) {}
    void onWrite(BLECharacteristic* characteristic) override;

   private:
    BLEHandler& _owner;
  };

  BLEHandlerListener& _listener;
  BLEServer* _server = nullptr;
  BLECharacteristic* _commandCharacteristic = nullptr;
  BLECharacteristic* _statusCharacteristic = nullptr;
};
