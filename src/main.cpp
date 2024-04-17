#include <Arduino.h>
#include "hardware.h"
#include <Adafruit_NeoPixel.h>


HARDWARE::Control_buttons control(1,1,1,1);

enum P
{
    P1,
    P2 
};
HARDWARE::Point_light point_led[2] = {HARDWARE::Point_light(5), HARDWARE::Point_light(1)};
HARDWARE::Motor motor(A1, A0, A2);

namespace BUTTON_FUNCS
{
    void ball_lift()
    {
        motor.stop();
        motor.disable(); // motor should not be active continously
    }
    void update_points(int player)
    {
        if (control.state == control.GAME)
        {
            point_led[player].incriment();
            if (point_led[player].score >= 10)
            {
                control.state = control.OVER;
                control.color_update();
                return;
            }
        }
        motor.advance(200*40*10); // around to 10 rotations of the motor
    }
    void p1()
    {
        update_points(P1);
    }
    void p2()
    {
        update_points(P2);
    }
} // namespace BUTTON_FUNCS
HARDWARE::Button_func point_button[2] = {HARDWARE::Button_func(1, BUTTON_FUNCS::p1), HARDWARE::Button_func(1, BUTTON_FUNCS::p2)};
HARDWARE::Button_func ball_button(1, BUTTON_FUNCS::ball_lift);


void setup()
{
 
}


void loop()
{
    motor.loop();
    ball_button.loop(true);
    for (int i = 0; i < 2; i ++) { point_button[i].loop(true); }
    control.loop();
}