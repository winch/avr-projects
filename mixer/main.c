
#include "led.h"
#include "button.h"
#include "motor.h"

int main(void)
{
    led_init();
    button_init();
    motor_init();
    
    while(1)
    {
        if (button_up() || button_down())
            led_on();
        else
            led_off();
    }
    
    return 0;
}


