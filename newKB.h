#pragma once

#define LILYGO_WATCH_2020_V3
#define LILYGO_WATCH_HAS_MOTOR
#include <LilyGoWatch.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "BLEHIDDevice.h"


#include "pinInput.h"
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
uint32_t my_pass_key = 0;


pinInput * mkeys = new pinInput;

typedef uint8_t MediaKeyReport[2];

const MediaKeyReport KEY_MEDIA_VOLUME_UP = {32, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_DOWN = {64, 0};


class MySecurity : public BLESecurityCallbacks {
  public:
  
    MySecurity(TTGOClass* _ttgo, bool* _is_connected)
    {
      ttgo = _ttgo;
      is_connected = _is_connected;
    }
    TTGOClass* ttgo;
    bool* is_connected;

    
    bool onConfirmPIN(uint32_t pin) {
      return false;
    }

    uint32_t onPassKeyRequest() {
      Serial.println("PassKeyRequest");
      mkeys->draw(ttgo->tft);
      my_pass_key = mkeys->get_input(ttgo);
      Serial.println(my_pass_key);

      //ESP_LOGI(LOG_TAG, "PassKeyRequest");
      return my_pass_key;
    }

    void onPassKeyNotify(uint32_t pass_key) {
      Serial.print("On passkey Notify number: ");
      Serial.println(pass_key);

      //ESP_LOGI(LOG_TAG, "On passkey Notify number:%d", pass_key);
    }

    bool onSecurityRequest() {
      Serial.println("On Security Request");
      //ESP_LOGI(LOG_TAG, "On Security Request");
      return true;
    }

    void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl) {
      Serial.println("onAuthenticationComplete");
      if (cmpl.success) {
        *is_connected = true;
        uint16_t length;
        esp_ble_gap_get_whitelist_size(&length);
        ESP_LOGD(LOG_TAG, "size: %d", length);
      }
    }
};
// Report IDs:
#define KEYBOARD_ID 0x01
#define MEDIA_KEYS_ID 0x02

static const uint8_t _hidReportDescriptor[] = {
  USAGE_PAGE(1),      0x01,          // USAGE_PAGE (Generic Desktop Ctrls)
  USAGE(1),           0x06,          // USAGE (Keyboard)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  // ------------------------------------------------- Keyboard
  REPORT_ID(1),       KEYBOARD_ID,   //   REPORT_ID (1)
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0xE0,          //   USAGE_MINIMUM (0xE0)
  USAGE_MAXIMUM(1),   0xE7,          //   USAGE_MAXIMUM (0xE7)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   Logical Maximum (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x08,          //   REPORT_COUNT (8)
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 1 byte (Reserved)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE (8)
  HIDINPUT(1),        0x01,          //   INPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x05,          //   REPORT_COUNT (5) ; 5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  USAGE_PAGE(1),      0x08,          //   USAGE_PAGE (LEDs)
  USAGE_MINIMUM(1),   0x01,          //   USAGE_MINIMUM (0x01) ; Num Lock
  USAGE_MAXIMUM(1),   0x05,          //   USAGE_MAXIMUM (0x05) ; Kana
  HIDOUTPUT(1),       0x02,          //   OUTPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 3 bits (Padding)
  REPORT_SIZE(1),     0x03,          //   REPORT_SIZE (3)
  HIDOUTPUT(1),       0x01,          //   OUTPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x06,          //   REPORT_COUNT (6) ; 6 bytes (Keys)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE(8)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM(0)
  LOGICAL_MAXIMUM(1), 0x65,          //   LOGICAL_MAXIMUM(0x65) ; 101 keys
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0x00,          //   USAGE_MINIMUM (0)
  USAGE_MAXIMUM(1),   0x65,          //   USAGE_MAXIMUM (0x65)
  HIDINPUT(1),        0x00,          //   INPUT (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0),                 // END_COLLECTION
  // ------------------------------------------------- Media Keys
  USAGE_PAGE(1),      0x0C,          // USAGE_PAGE (Consumer)
  USAGE(1),           0x01,          // USAGE (Consumer Control)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  REPORT_ID(1),       MEDIA_KEYS_ID, //   REPORT_ID (3)
  USAGE_PAGE(1),      0x0C,          //   USAGE_PAGE (Consumer)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   LOGICAL_MAXIMUM (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x10,          //   REPORT_COUNT (16)
  USAGE(1),           0xB5,          //   USAGE (Scan Next Track)     ; bit 0: 1
  USAGE(1),           0xB6,          //   USAGE (Scan Previous Track) ; bit 1: 2
  USAGE(1),           0xB7,          //   USAGE (Stop)                ; bit 2: 4
  USAGE(1),           0xCD,          //   USAGE (Play/Pause)          ; bit 3: 8
  USAGE(1),           0xE2,          //   USAGE (Mute)                ; bit 4: 16
  USAGE(1),           0xE9,          //   USAGE (Volume Increment)    ; bit 5: 32
  USAGE(1),           0xEA,          //   USAGE (Volume Decrement)    ; bit 6: 64
  USAGE(2),           0x23, 0x02,    //   Usage (WWW Home)            ; bit 7: 128
  USAGE(2),           0x94, 0x01,    //   Usage (My Computer) ; bit 0: 1
  USAGE(2),           0x92, 0x01,    //   Usage (Calculator)  ; bit 1: 2
  USAGE(2),           0x2A, 0x02,    //   Usage (WWW fav)     ; bit 2: 4
  USAGE(2),           0x21, 0x02,    //   Usage (WWW search)  ; bit 3: 8
  USAGE(2),           0x26, 0x02,    //   Usage (WWW stop)    ; bit 4: 16
  USAGE(2),           0x24, 0x02,    //   Usage (WWW back)    ; bit 5: 32
  USAGE(2),           0x83, 0x01,    //   Usage (Media sel)   ; bit 6: 64
  USAGE(2),           0x8A, 0x01,    //   Usage (Mail)        ; bit 7: 128
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0)                  // END_COLLECTION
};

class myKeyboard
{
  public:

    myKeyboard(TTGOClass* _ttgo)
    {
      ttgo = _ttgo;
    }

    TTGOClass* ttgo;


    BLEHIDDevice* hid;
    BLECharacteristic* inputMediaKeys;
    uint32_t           _delay_ms = 7;
    
    MediaKeyReport     _mediaKeyReport;
    bool is_connected = false;
    
    

    void start()
    {
      BLEDevice::init("basil's esp");
      BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);

      //Required in authentication process to provide displaying and/or input passkey or yes/no butttons confirmation

      BLEDevice::setSecurityCallbacks(new MySecurity(ttgo, &is_connected));
      BLEServer *pServer = BLEDevice::createServer();
      BLEService *pService = pServer->createService(SERVICE_UUID);
      BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                             CHARACTERISTIC_UUID,
                                             BLECharacteristic::PROPERTY_READ |
                                             BLECharacteristic::PROPERTY_WRITE
                                           );

      pService->start();
      BLEAdvertising *pAdvertising = pServer->getAdvertising();
      pAdvertising->start();
      BLESecurity *pSecurity = new BLESecurity();
      pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);
      pSecurity->setCapability( ESP_IO_CAP_IN  );
      pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);


      hid = new BLEHIDDevice(pServer);
      inputMediaKeys = hid->inputReport(MEDIA_KEYS_ID);
      hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
      hid->hidInfo(0x00, 0x01);
      hid->reportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
      hid->startServices();


      Serial.println("Characteristic defined! Now you can read it in your phone!");
    }

    void sendReport(MediaKeyReport* keys)
    {
      if (is_connected)
      {
        this->inputMediaKeys->setValue((uint8_t*)keys, sizeof(MediaKeyReport));
        this->inputMediaKeys->notify();
#if defined(USE_NIMBLE)
        //vTaskDelay(delayTicks);
        this->delay_ms(_delay_ms);
#endif // USE_NIMBLE
      }
    }

    size_t press(const MediaKeyReport k)
    {
      uint16_t k_16 = k[1] | (k[0] << 8);
      uint16_t mediaKeyReport_16 = _mediaKeyReport[1] | (_mediaKeyReport[0] << 8);

      mediaKeyReport_16 |= k_16;
      _mediaKeyReport[0] = (uint8_t)((mediaKeyReport_16 & 0xFF00) >> 8);
      _mediaKeyReport[1] = (uint8_t)(mediaKeyReport_16 & 0x00FF);

      sendReport(&_mediaKeyReport);
      return 1;
    }

    size_t release(const MediaKeyReport k)
    {
      uint16_t k_16 = k[1] | (k[0] << 8);
      uint16_t mediaKeyReport_16 = _mediaKeyReport[1] | (_mediaKeyReport[0] << 8);
      mediaKeyReport_16 &= ~k_16;
      _mediaKeyReport[0] = (uint8_t)((mediaKeyReport_16 & 0xFF00) >> 8);
      _mediaKeyReport[1] = (uint8_t)(mediaKeyReport_16 & 0x00FF);

      sendReport(&_mediaKeyReport);
      return 1;
    }

    size_t write(const MediaKeyReport c)
    {
      uint16_t p = press(c);  // Keydown
      release(c);            // Keyup
      return p;              // just return the result of press() since release() almost always returns 1
    }

    void delay_ms(uint64_t ms) {
      uint64_t m = esp_timer_get_time();
      if (ms) {
        uint64_t e = (m + (ms * 1000));
        if (m > e) { //overflow
          while (esp_timer_get_time() > e) { }
        }
        while (esp_timer_get_time() < e) {}
      }
    }






};
