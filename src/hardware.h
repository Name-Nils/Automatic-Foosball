#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


namespace HARDWARE
{
    class Point_light
    {
        // point couting and extra animations
        Adafruit_NeoPixel leds;
        int score = 0;

        const int led_count = 10;

    public:

        Point_light(int pin)
        {
            this->leds = Adafruit_NeoPixel(led_count, pin, NEO_GRB + NEO_KHZ800);
            this->leds.begin();
            this->leds.setBrightness(255); // max brightness
            for (int i = 0; i < led_count; i++)
            {
                this->leds.setPixelColor(i, 255,255,255); // set all leds to white
            }
        }

        void incriment()
        {
            if (score < led_count)
            {
                this->leds.setPixelColor(this->score, 0, 255, 0); // green for received points
                this->score ++;
            }
        }

        void decrement()
        {
            if (score > 0)
            {
                this->leds.setPixelColor(this->score, 255, 255, 255);
                this->score --;
            }
        }

        void win_animation()
        {

        }

        void loss_animation()
        {

        }

        void rainbow_animation()
        {
            
        }

        void loop()
        {

        }
    };
    class Motor
    {
        // acceleration with absolute positioning
        int step_pin;
        int dir_pin;
        int enable_pin;

        long int position = 0; // absolute steps
        long int target = 0;
        double current_speed = 0; // steps per microsecond
        double max_speed = 0.001;
        double acceleration = 0.00000001; // steps per microsecond per microsecond

        unsigned long int last_step_time = 0; // absolute time of last step
        unsigned long int last_loop = 0;

    public:

        Motor(int step, int dir, int enable)
        {
            this->step_pin = step;
            this->dir_pin = dir;
            this->enable_pin = enable;
            pinMode(this->enable_pin, OUTPUT);
            pinMode(this->dir_pin, OUTPUT);
            pinMode(this->step_pin, OUTPUT);
        }

        void enable()
        {
            digitalWrite(this->enable_pin, false);
        }
        void disable()
        {
            digitalWrite(this->enable_pin, true);
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
            this->target = target;
        }

        void advance(long int distance)
        {
            this->target += distance;
        }

        void loop()
        {
            // make sure that motor never accelerates over the allowed amount and that the motor reaches target position
            //compare the distance left to the max speed allowed at that distance
            long int delta_time = micros() - this->last_loop;
            this->last_loop = micros();
            long int distance_left = this->target - this->position;
            if (distance_left == 0) return;
            double max_target_speed = sqrt(2 * this->acceleration * abs(distance_left));
            if (fabs(this->current_speed) > max_target_speed)
            {
                this->current_speed = max_target_speed * ((this->current_speed > 0) ? 1 : -1);
            }
            else
            {
                this->current_speed += this->acceleration * delta_time * ((distance_left > 0) ? 1 : -1);
            }
            if (fabs(this->current_speed) > this->max_speed) this->current_speed = this->max_speed * ((this->current_speed > 0) ? 1 : -1);
            this->move_speed(this->current_speed);
        }
    };
    class Button
    {
        // removal of noise from button press
        // function pointer that points to functionality on button press (not release)
        void (*function)();
        int pin;
        bool last_state = false;
        unsigned long int last_change = 0;
        const unsigned long int min_time_change = 50; // button must be depressed for at least 50 milliseconds and likewise when released
        bool actuated = true;

    public:

        Button(int pin, void (*function)())
        {
            this->pin = pin;
            pinMode(this->pin, INPUT);
            this->function = function;
        }

        void loop(bool actuate_on_press) // alternatively on release
        {
            bool button_state = digitalRead(this->pin);
            if (button_state == this->last_state)
            {
                if (millis() - this->last_change > this->min_time_change)
                {
                    this->actuated = true;
                    if (button_state == actuate_on_press) this->function();
                }
            }
            else
            {
                this->actuated = false;
                if (millis() - this->last_change <= this->min_time_change)
                {
                    this->actuated = true;
                }
                this->last_change = millis();
            }
        }
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
