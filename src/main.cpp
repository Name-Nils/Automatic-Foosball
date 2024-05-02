#include <Arduino.h>
#include "hardware.h"
#include <Adafruit_NeoPixel.h>


HARDWARE::Control_buttons control(11, 7, 5, 6);

enum P
{
    P1,
    P2 
};
HARDWARE::Point_light point_led[2] = {HARDWARE::Point_light(3), HARDWARE::Point_light(8)};
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
            motor.enable();
            motor.advance(200*40*11); // around to 10 rotations of the motor
        }
    }
    void start_pressed()
    {
        if (control.state != control.GAME) 
        {
            for (int i = 0; i < 2; i++)
            {
                point_led[i].setup();
            }
            motor.stop(); // make sure motor doesnt have preloaded extra distance
            motor.enable();
            motor.advance(200*40*11);
        }
    }
    void reset_pressed()
    {
        motor.stop();
        motor.disable();
        for (int i = 0; i < 2; i++)
        {
            point_led[i].setup();
        }
    }
    void mode_pressed()
    {

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
HARDWARE::Button_func point_button[2] = {HARDWARE::Button_func(A5, BUTTON_FUNCS::p1), HARDWARE::Button_func(13, BUTTON_FUNCS::p2)};
HARDWARE::Button_func ball_button(4, BUTTON_FUNCS::ball_lift);


void setup()
{    
    motor.disable();
    control.setup();
    for (int i = 0; i < 2; i++)
    {
        point_led[i].setup();
    }

    control.start.function = BUTTON_FUNCS::start_pressed;
    control.reset.function = BUTTON_FUNCS::reset_pressed;
    control.mode.function = BUTTON_FUNCS::mode_pressed;
}


void loop()
{
    control.loop();
    motor.loop();
    ball_button.loop(true);
    for (int i = 0; i < 2; i++)
    {
        point_button[i].loop(true);
    }
}