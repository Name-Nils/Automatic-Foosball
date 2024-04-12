#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


namespace HARDWARE
{
    class Point_light
    {
        // point couting and extra animations
    };
    class Motor
    {
        // acceleration with absolute positioning
    };
    class Button
    {
        // removal of noise from button press
        // function pointer that points to functionality on button press (not release)
    };
    class Control_buttons
    {
        // use the button class as either child for this class or as internal object and then layer with lighting elements
    };
    class Lighting
    {
        // extra lighting such as underglow or goal lighting
    };
} // namespace HARDWARE
