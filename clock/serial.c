
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

#include "rtc.h"

// 4800baud @ 1mhz
#define BAUD_PRESCALE 12

//comands
#define COMMAND_NONE 'n'
//print time
#define COMMAND_PRINT 'p'
//set time
#define COMMAND_SET 's'
//end command
#define COMMAND_END '|'
//maximum length of command data
//yyyy-mm-dd hh:mm:ssp|
#define COMMAND_LEN 22

static struct rtc_time time;
static volatile char command = COMMAND_NONE;
static volatile char command_status = COMMAND_NONE;
static volatile char command_data[COMMAND_LEN + 1] = "";

void serial_init()
{
    //turn on hardware
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    //8bit characters
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
    //setup baud rate
    UBRR0L = BAUD_PRESCALE;
    UBRR0H = (BAUD_PRESCALE >> 8);
    //turn on interrupt
    UCSR0B |= (1 << RXCIE0);
    //enable global interupts
    sei();
}

//write a single character to serial
void serial_write(uint8_t data)
{
    while ((UCSR0A & (1 << UDRE0)) == 0) ;
    UDR0 = data;
}

//write a string to serial
void serial_write_string(char *data)
{
    while(*data)
    {
        serial_write(*data);
        data++;
    }
}

//appends character to command_data
static void append_command_data(char data)
{
    size_t len = strlen(command_data);
    //check if command_data is full
    if (len == COMMAND_LEN)
    {
        command_data[0] = 0;
        command = COMMAND_NONE;
        serial_write_string(" cd over\n");
    }
    else
    {
        //append char
        command_data[len] = data;
        command_data[len + 1] = 0;
    }
}

//read the time from rtc and write it to serial
static void serial_print_time()
{
    rtc_read(&time);
    snprintf(command_data, COMMAND_LEN, "20%.2d-%.2d-%.2d %.2d:%.2d:%.2d%c\n", time.year, time.month, time.day, time.hour, time.minute, \
                time.second, time.period);
    serial_write_string(command_data);
}

//write the time recieved in command_data to rtc
static void serial_write_time()
{
    //yyyy-mm-dd hh:mm:ssp
    char *data;
    //period
    time.period = command_data[19];
    
    //seconds
    command_data[19] = 0;
    data = command_data + 17;
    time.second = atoi(data);
    
    //minutes
    command_data[16] = 0;
    data = command_data + 14;
    time.minute = atoi(data);
    
    //hours
    command_data[13] = 0;
    data = command_data + 11;
    time.hour = atoi(data);
    
    //day
    command_data[10] = 0;
    data = command_data + 8;
    time.day = atoi(data);
    
    //month
    command_data[7] = 0;
    data = command_data + 5;
    time.month = atoi(data);
    
    //year
    command_data[4] = 0;
    data = command_data + 2;
    time.year = atoi(data);

    rtc_write(&time);
    //print the set time
    serial_print_time();
}

void serial_do_command()
{
    if (command_status == COMMAND_END)
    {
        //perform command
        if (command == COMMAND_PRINT)
        {
            serial_print_time();
        }
        else if (command == COMMAND_SET)
        {
            serial_write_string(" set\n");
        }
        else
        {
            //bad command
            snprintf(command_data, COMMAND_LEN, "bad c %c\n", command);
            serial_write_string(command_data);
        }
        
        command_status = COMMAND_NONE;
        command_data[0] = 0;
        command = COMMAND_NONE;
    }
}

ISR(USART_RX_vect)
{
    //recieved byte
    uint8_t rx_data = UDR0;
    
    if (command_status != COMMAND_NONE)
    {
        return;
    }

    if (command == COMMAND_NONE)
    {
        command = rx_data;
    }
    else
    {
        if (rx_data == COMMAND_END)
        {
            //command ready to be perfomed
            command_status = COMMAND_END;
        }
        else
        {
            //add data to command_data
            append_command_data(rx_data);
        }
    }
    
}

