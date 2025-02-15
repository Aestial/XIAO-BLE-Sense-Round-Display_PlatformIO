#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>

#define USE_TFT_ESPI_LIBRARY
#include "lv_xiao_round_screen.h"
// #include <PNGdec.h>

#define MAX_IMAGE_WIDTH 240

TFT_eSprite sprite = TFT_eSprite(&tft); // Off-screen buffer

void setup() {
  sprite.createSprite(240, 240); // Match display size
}

void loop() {
  sprite.fillScreen(TFT_GREENYELLOW);

  // Push sprite to display
  sprite.pushSprite(0, 0);

  delay(10);
}