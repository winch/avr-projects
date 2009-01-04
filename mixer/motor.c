
#include <avr/io.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "led.h"

#define MOTOR_DDR DDRB
#define MOTOR_PORT PORTB
#define MOTOR_PWM PB2
#define MOTOR_ENABLE PB3

#define MOTOR_SPEED_STEP 1

static uint8_t speed;

void motor_init()
{
    //read speed from eeprom
    speed = eeprom_read_byte(0);
    
    //set pins as outputs
    MOTOR_DDR |= (1 << MOTOR_PWM) | (1 << MOTOR_ENABLE);
    
    //set OC0A pin behaviour
    TCCR0A |= (1 << COM0A1);
    
    //waveform generation mode 3
    TCCR0A |= (1 << WGM00) | (1 << WGM01);
}

void motor_start()
{
    //start the motor
    //enable clock source
    TCCR0B |= (1 << CS00);
    //pwm speed
    OCR0A = speed;
    //motor enable line
    MOTOR_PORT |= (1 << MOTOR_ENABLE);
}

void motor_stop()
{
    //stop the motor
    //disable clock source
    TCCR0B &= ~(1 << CS00);
    //motor enable line
    MOTOR_PORT &= ~(1 << MOTOR_ENABLE);
    led_flash();
}

void motor_faster()
{
    //increase motor speed and start motor if required
    if (speed == 0)
        motor_start();
    if (speed == 255)
        return;
    
    speed += MOTOR_SPEED_STEP;
    OCR0A = speed;
}

void motor_slower()
{
    //reduce motor speed and stop motor if required
    if (speed < MOTOR_SPEED_STEP)
        return;
    speed -= MOTOR_SPEED_STEP;
    OCR0A = speed;
    if (speed == 0)
        motor_stop();
}

void motor_store()
{
    //store speed of motor in eeprom
    _delay_ms(1000);
    eeprom_write_byte(0, speed);
    led_flash();    
}

