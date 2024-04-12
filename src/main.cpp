#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(10, A4, NEO_GRB + NEO_KHZ800);

void setup()
{
 strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
    strip.setBrightness(255);
    for (int i = 0; i < 10; i++)
    {
        strip.setPixelColor(i, 255, 0, 0);
        int ii = i - 1;
        if (ii < 0) ii = 9;
        strip.setPixelColor(ii, 0,0,0);
        strip.show();
        delay(50);
    }
    for (int i = 9; i >= 0; i--)
    {
        strip.setPixelColor(i, 255, 0, 0);
        int ii = i + 1;
        if (ii > 9) ii = 0;
        strip.setPixelColor(ii, 0,0,0);
        strip.show();
        delay(50);
    }
}