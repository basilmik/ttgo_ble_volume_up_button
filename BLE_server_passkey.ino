/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#define LILYGO_WATCH_2020_V3
#define LILYGO_WATCH_HAS_MOTOR
#include <LilyGoWatch.h>
#include "pinInput.h"
#include "newKB.h"

TTGOClass* ttgo = nullptr;
TFT_eSPI* tft = nullptr;

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
