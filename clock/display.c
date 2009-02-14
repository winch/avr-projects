
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "display_values.h"

#define DISPLAY_PORT PORTB
#define DISPLAY_DDR DDRB
#define DISPLAY_DATA PB0
#define DISPLAY_OE PB1
#define DISPLAY_LA PB2
#define DISPLAY_CL PB3

//functions
static void display_write(int);
static void display_clear();

//data to output when pin 1 is sinking
static int a_data;

//data to output when pin 2 is sinking
static int b_data;

void display_init()
{
    //set pins as outputs
    DISPLAY_DDR |= (1 << DISPLAY_DATA) | (1 << DISPLAY_OE) | (1 << DISPLAY_CL) |  (1 << DISPLAY_LA);
    DISPLAY_PORT &= ~(1 << DISPLAY_OE);
    DISPLAY_PORT |= (1 << DISPLAY_LA);
    
    //setup timer clock source cs/32
    TCCR2B |= (1 << CS20) | (1 << CS21);
    
    //inable timer overflow interupt
    TIMSK2 |= (1 << TOIE2);
    
    //enable global interupts
    sei();
    
    a_data = 0x0;
    b_data = 0x0;
}

static void display_toggle()
{
    //switch display to output other data set
    static int set = 0;
    if (set == 0)
    {
        //display a data
        display_write(0x8000 | a_data);
        set = 1;
    }
    else
    {
        //display b data
        display_write(0x4000 | b_data);
        set = 0;
    }
}

ISR(TIMER2_OVF_vect)
{
    display_toggle();
}

void display_set(char a, char b, char c, char d)
{
    a_data = a_values[a][0] | a_values[b][0] | a_values[c][0] | a_values[d][0];
    a_data = b_values[a][0] | b_values[b][0] | b_values[c][0] | b_values[d][0];
    
    //clear sinking pins
    a_data &= 0x3fff;
    b_data &= 0x3fff;
}

static void display_write(int data)
{
    //write data to shift registers
    int bit;
    
    DISPLAY_PORT &= ~(1 << DISPLAY_CL);
    DISPLAY_PORT &= ~(1 << DISPLAY_LA);
    
    //clock out bits
    for (bit = 0; bit < 16; bit++)
    {
        DISPLAY_PORT &= ~(1 << DISPLAY_CL);
        if (data & 0x1)
             DISPLAY_PORT |= (1 << DISPLAY_DATA);
        else
            DISPLAY_PORT &= ~(1 << DISPLAY_DATA);
        data >>= 1;
        DISPLAY_PORT |= (1 << DISPLAY_CL);
    }
    
    DISPLAY_PORT &= ~(1 << DISPLAY_CL);
    DISPLAY_PORT |= (1 << DISPLAY_LA);
}

void display_test()
{
    //turn on all segments for 1 sec
    cli();
    display_write(0x3fff);
    _delay_ms(1000);
    display_clear();
    sei();
}

static void display_clear()
{
    display_write(0);
}

