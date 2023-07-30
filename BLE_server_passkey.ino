/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#define LILYGO_WATCH_2020_V3
#define LILYGO_WATCH_HAS_MOTOR
#include <LilyGoWatch.h>
#include "pinInput.h"
#include "newKB.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

TTGOClass* ttgo = nullptr;
TFT_eSPI* tft = nullptr;

/*
  uint32_t my_pass_key = 0;
  pinInput * mkeys = new pinInput;

  bool is_connected = false;

  class MySecurity : public BLESecurityCallbacks {

    bool onConfirmPIN(uint32_t pin) {
      return false;
    }

    uint32_t onPassKeyRequest() {
      Serial.println("PassKeyRequest");

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
        is_connected = true;
        uint16_t length;
        esp_ble_gap_get_whitelist_size(&length);
        ESP_LOGD(LOG_TAG, "size: %d", length);
      }
    }
  };
*/
myKeyboard* kb;


void setup() {
  Serial.begin(115200);

  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();

  tft = ttgo->tft;

  tft->fillScreen(TFT_BLACK);
  tft->drawRect(110, 110, 20, 20, TFT_RED);
  tft->setTextColor(TFT_WHITE);
  tft->setTextSize(4);
  
  Serial.print("draw done");
  Serial.println("Starting BLE work!");

  kb = new myKeyboard(ttgo);
  kb->start();

  Serial.println("Characteristic defined! Now you can read it in your phone!");


}
int16_t x, y;
int16_t sx, sy;
bool is_updated = false;

void update_draw()
{
  tft->fillScreen(TFT_BLACK);
  tft->fillRect(110, 110, 20, 20, TFT_RED);
  is_updated = true;
}

bool is_circle_drawn = false;

void loop() {

  if (kb->is_connected)
  {
    if (!is_updated)
    {
      Serial.println("update draw");
      update_draw();
      
    }

    if (ttgo->getTouch(x, y))
    {
      Serial.println("got touch");
      while (ttgo->getTouch(x, y)) // wait for user to release
      {
        if (!is_circle_drawn)
        {
          sx = x;
          sy = y;
          tft->drawCircle(x, y, 80, TFT_WHITE);
          is_circle_drawn = true;
        }
      }
      kb->write(KEY_MEDIA_VOLUME_UP);
      tft->drawCircle(sx, sy, 80, TFT_BLACK);
      tft->fillRect(110, 110, 20, 20, TFT_RED);
      is_circle_drawn = false;   

    }
  }

}
