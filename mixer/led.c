
#include <avr/io.h>

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_PIN PB1

void led_init()
{
    //set led pin as output
    LED_DDR |= (1 << LED_PIN);
}

void led_on()
{
    //turn led on
    LED_PORT |= (1 << LED_PIN); 
}

void led_off()
{
    //turn led off
    LED_PORT &= ~(1 << LED_PIN); 
}


