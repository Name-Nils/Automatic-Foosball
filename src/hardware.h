#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


namespace HARDWARE
{
    class Point_light
    {
        // point couting and extra animations
        Adafruit_NeoPixel leds;

        int pin;

        int led_count = 10;

    public:
        
        int score = 0;

        Point_light(int pin)
        {
            this->pin = pin;
            //this->setup();
        }

        Point_light() = default;

        void setup()
        {
            this->leds = Adafruit_NeoPixel(led_count, this->pin);
            this->leds.begin();
            this->leds.setBrightness(255); // max brightness
            for (int i = 0; i < led_count; i++)
            {
                this->leds.setPixelColor(i, 50,50,50); // set all leds to black
            }
            this->leds.show();
            this->score = 0;
        }

        void incriment()
        {
            for (int i = 0; i < this->led_count; i++)
            {
                if (i <= this->score)
                {
                    this->leds.setPixelColor(i, 0,255,0);
                }
                else
                {
                    this->leds.setPixelColor(i, 50,50,50);
                }
            }
            this->score = (this->score >= 10) ? 10 : this->score+1;
            this->leds.show();
        }

        void decrement()
        {
            if (score > 0)
            {
                this->leds.setPixelColor(this->score, 50, 50, 50);
                this->score --;
            }
            this->leds.show();
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
            //this->leds.show();
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
        double acceleration = 0.000000004; // steps per microsecond per microsecond

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
            this->last_loop = micros();
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

        void stop()
        {
            // add the minimum distance for deceleration
            this->current_speed = 0;
            this->position = this->target;
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
    protected:

        int pin;
        bool last_state = false;
        unsigned long int last_change = 0;
        unsigned long int min_time_change = 30; // button must be depressed for at least 50 milliseconds and likewise when released
        bool actuated = true;

    public:

        bool button_state = false;

        Button(int pin)
        {
            this->pin = pin;
            pinMode(this->pin, INPUT);
        }

        Button() = default;

        bool loop(bool actuate_on_press) // alternatively on release
        {
            bool rtn = false;
            this->button_state = digitalRead(this->pin);
            if (this->button_state != this->last_state)
            {
                // change of state
                this->actuated = true;
                this->last_change = millis();
            }
            if (millis() - this->last_change > this->min_time_change)
            {
                if (actuate_on_press == this->button_state && this->actuated)
                {
                    rtn = true;
                }
                this->actuated = false;
            }
            this->last_state = this->button_state;
            return rtn;
        }
    };
    class Button_func: protected Button
    {
        // removal of noise from button press
        // function pointer that points to functionality on button press (not release)
    public:
        
        void (*function)();

        Button_func(int pin, void (*function)())
        {
            this->pin = pin;
            pinMode(this->pin, INPUT);
            this->function = function;
        }

        Button_func(int pin)
        {
            this->pin = pin;
            pinMode(this->pin, INPUT);
        }

        Button_func() = default;

        bool loop(bool actuate_on_press)
        {
            if (this->Button::loop(actuate_on_press))
            {
                this->function();
                return true;
            }
            return false;
        }
    };
    class Control_buttons
    {
        // use the button class as either child for this class or as internal object and then layer with lighting elements
        int led_pin;

        enum PIN
        {
            RESET=0,
            MODE=1,
            START=2
        };

    public:
        Adafruit_NeoPixel leds;
      
        Button_func start;
        Button_func mode;
        Button_func reset;
        
        enum STATE
        {
            STANDBY,
            GAME,
            OVER
        };

        STATE state;

        Control_buttons(int led, int start, int mode, int reset)
        {
            this->start = Button_func(start);  
            this->mode = Button_func(mode);
            this->reset = Button_func(reset);
            this->led_pin = led;
        }

        void setup()
        {
            this->leds = Adafruit_NeoPixel(3, this->led_pin);
            this->leds.begin();

            this->state = this->STANDBY;

            this->leds.setPixelColor(this->START, 0,255,0);
            this->leds.setPixelColor(this->MODE, 255,255,255);
            this->leds.setPixelColor(this->RESET, 0,0,0);

            this->leds.show();
        }

        void start_loop()
        {
            if (this->start.loop(true))
            {
                // start button clicked
                this->state = this->GAME;
                this->color_update();
            }
        }

        void reset_loop()
        {
            if (this->reset.loop(true))
            {
                this->state = this->STANDBY;
                this->color_update();
            }
        }

        void color_update()
        {
            uint8_t r,g,b;
            r = g = b = 0;

            switch (this->state)
            {
            case this->STANDBY:
                g = 255;
                break;
            case this->GAME:
                r = 255;
                g = 165;
                break;
            case this->OVER:
                g = 255;
                break;
            default:
                break;
            }
            this->leds.setPixelColor(this->START, r,g,b);

            r = g = b = 0;  
            switch (this->state)
            {
            case this->STANDBY:
                // black
                break;
            case this->GAME:
                r = 200;
                break;
            case this->OVER:
                r = 255;
                break;
            default:
                break;
            }
            this->leds.setPixelColor(this->RESET, r,g,b);

            r = g = b = 0;
            switch (this->state)
            {
            case this->STANDBY:
                r = 255;
                g = 255; 
                b = 255;
                break;
            case this->GAME:
                // black
                break;
            case this->OVER:
                // black
                break;
            default:
                break;
            }
            this->leds.setPixelColor(this->MODE, r,g,b);

            this->leds.show();
        }

        void loop()
        {
            this->start_loop();
            this->reset_loop();
        }
    };
    class Lighting
    {
        // extra lighting such as underglow or goal lighting
    };
} // namespace HARDWARE
