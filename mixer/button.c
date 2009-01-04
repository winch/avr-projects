
#include <avr/io.h>

#define BUTTON_DDR DDRA
#define BUTTON_PORT PORTA
#define BUTTON_PIN PINA
#define BUTTON_DOWN PA0
#define BUTTON_UP PA1

void button_init()
{
    //set as inputs
    BUTTON_DDR &= ~(1 << BUTTON_DOWN);
    BUTTON_DDR &= ~(1 << BUTTON_UP);
    
    //enable pull ups
    BUTTON_PORT |= (1 << BUTTON_DOWN) | (1 << BUTTON_UP);
}

int button_up()
{
    //return status of speed up button
    return ~BUTTON_PIN & (1 << BUTTON_UP);
}

int button_down()
{
    // return status of slow down button
    return ~BUTTON_PIN & (1 << BUTTON_DOWN);
}

