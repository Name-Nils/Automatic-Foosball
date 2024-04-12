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
        int step_pin;
        int dir_pin;
        int enable_pin;

        long int position = 0; // absolute steps
        double current_speed = 0; // steps per microsecond
        double max_speed = 0.000002;
        double acceleration = 100; // steps per microsecond per microsecond

        unsigned long int last_step_time = 0; // absolute time of last step

    public:

        Motor::Motor(int step, int dir, int enable)
        {
            this->step_pin = step;
            this->dir_pin = dir;
            this->enable_pin = enable;
        }

        void step(bool direction)
        {
            digitalWrite(this->dir_pin, direction);
            this->position += (direction) ? 1 : -1;
            digitalWrite(this->step_pin, true);
            digitalWrite(this->step_pin, false);
            this->last_step_time = micros();
        }

        void move_speed(double speed)
        {
            bool direction = speed > 0;
            speed = fabs(speed);
            if (micros() - this->last_step_time > 1. / speed)
            {
                step(direction);
            }
        }

        void move_position(long int target)
        {
            // make sure that motor never accelerates over the allowed amount and that the motor reaches target position
            //compare the distance left to the max speed allowed at that distance
            long int distance_left = target - this->position;
            double max_speed = sqrt(2 * this->acceleration * abs(distance_left));

        }
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
