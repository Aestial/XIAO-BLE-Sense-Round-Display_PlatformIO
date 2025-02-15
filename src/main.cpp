#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>

#define USE_TFT_ESPI_LIBRARY
#include "lv_xiao_round_screen.h"
#include <PNGdec.h>

#include "archlinux_chan.h"
#include "archlinux_chan2.h"
#include "archlinux_chan3.h"

#define MAX_IMAGE_WIDTH 240

TFT_eSprite sprite = TFT_eSprite(&tft); // Off-screen buffer
// PNG decoder and TFT display instances
PNG png;

// Image file structure
struct Image
{
  const uint8_t *data;
  size_t size;
};

// Allocate current images with explicit casting
const Image images[] = {
    {(const uint8_t *)archlinux_chan, sizeof(archlinux_chan)},
    {(const uint8_t *)archlinux_chan2, sizeof(archlinux_chan2)},
    {(const uint8_t *)archlinux_chan3, sizeof(archlinux_chan3)},
};

const size_t numImages = sizeof(images) / sizeof(images[0]);

int currentImage = 0; // Index of the current image

// Function to draw PNG to the sprite (callback for PNG decoder)
void pngDrawToSprite(PNGDRAW *pDraw);

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nUsing the TFT eSPI library with touch interaction!");

  // Initialize TFT
  tft.begin();
  tft.fillScreen(TFT_RED);
  sprite.createSprite(240, 240); // Match display size

  pinMode(TOUCH_INT, INPUT_PULLUP);
  Wire.begin();

  Serial.println("Setup complete.");
}

void loop() {
  sprite.fillScreen(TFT_BLACK);
  // Render background and snow
  int16_t rc = png.openFLASH(
      (uint8_t *)images[currentImage].data,
      images[currentImage].size,
      pngDrawToSprite);

  if (rc != PNG_SUCCESS)
  {
    Serial.println("Failed to open PNG file!");
  }
  png.decode(NULL, 0); // Decode and render image

  // Push sprite to display
  sprite.pushSprite(0, 0);

  // Handle touch input
  if (chsc6x_is_pressed())
  {
    currentImage = (currentImage + 1) % numImages;
    delay(300);
  }

  delay(10);
}

void pngDrawToSprite(PNGDRAW *pDraw)
{
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  sprite.pushImage(0, pDraw->y, pDraw->iWidth, 1, lineBuffer);
}