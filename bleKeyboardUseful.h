#pragma once
#ifndef ESP32_BLE_KEYBOARD_H
#define ESP32_BLE_KEYBOARD_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#if defined(USE_NIMBLE)

#include "NimBLECharacteristic.h"
#include "NimBLEHIDDevice.h"

#define BLEDevice                  NimBLEDevice
#define BLEServerCallbacks         NimBLEServerCallbacks
#define BLECharacteristicCallbacks NimBLECharacteristicCallbacks
#define BLEHIDDevice               NimBLEHIDDevice
#define BLECharacteristic          NimBLECharacteristic
#define BLEAdvertising             NimBLEAdvertising
#define BLEServer                  NimBLEServer

#else

#include "BLEHIDDevice.h"
#include "BLECharacteristic.h"

#endif // USE_NIMBLE

#include "Print.h"
#define LILYGO_WATCH_2020_V3
#define LILYGO_WATCH_HAS_MOTOR
#include <LilyGoWatch.h>

typedef uint8_t MediaKeyReport[2];

const MediaKeyReport KEY_MEDIA_VOLUME_UP = {32, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_DOWN = {64, 0};


//  Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;




class BleKeyboard : public Print, public BLEServerCallbacks, public BLECharacteristicCallbacks
{
private:
  BLEHIDDevice* hid;
  BLECharacteristic* inputMediaKeys;
  BLEAdvertising*    advertising;
  KeyReport          _keyReport;
  MediaKeyReport     _mediaKeyReport;
  std::string        deviceName;
  std::string        deviceManufacturer;
  uint8_t            batteryLevel;
  bool               connected = false;
  uint32_t           _delay_ms = 7;

public:


  BleKeyboard(std::string deviceName = "ESP32 Keyboard", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 100);
  void begin(void);
  void begin0(void);
  void end(void);

  void sendReport(MediaKeyReport* keys);

  size_t press(const MediaKeyReport k);

  size_t release(const MediaKeyReport k);
size_t write(uint8_t c) {return 0;};
  size_t write(const MediaKeyReport c);

  bool isConnected(void);
  void setBatteryLevel(uint8_t level);
  void setName(std::string deviceName);  

protected:
  virtual void onStarted(BLEServer *pServer) { };
  virtual void onConnect(BLEServer* pServer) override;
  virtual void onDisconnect(BLEServer* pServer) override;
  virtual void onWrite(BLECharacteristic* me) override;

};

#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_KEYBOARD_H
