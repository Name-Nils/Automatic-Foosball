#include <Arduino.h>
#include "hardware.h"
#include <Adafruit_NeoPixel.h>


HARDWARE::Motor motor(A1, A0, A2);
HARDWARE::Point_light p1_light(A3);

void point_incriment()
{
    p1_light.incriment();
    motor.advance(200*10);
}

HARDWARE::Button p1_button(A5, point_incriment);


void setup()
{
 
}


void loop()
{
    motor.loop();
    p1_button.loop(true);
}