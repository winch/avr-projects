
#include <util/delay.h>

#include "led.h"
#include "button.h"
#include "motor.h"

int main(void)
{
    led_init();
    button_init();
    motor_init();
    
    led_flash();
    
    motor_start();
       
    while(1)
    {
        _delay_ms(15);

        if (button_up() && button_down())
        {
            //store speed
            motor_store();
        }
        else
        {
            if (button_up())
            {
                motor_faster();
            }
            if (button_down())
            {
                motor_slower();
            }
        }
    }
    
    return 0;
}


