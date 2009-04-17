
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

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
//yyyy-mm-dd hh:mm:ss|
#define COMMAND_LEN 21

static struct rtc_time time;
static volatile char command = COMMAND_NONE;
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

//appends to command_data
static void append_command_data(char data)
{
    size_t len = strlen(command_data);
    //check if command_data is full
    if (len == COMMAND_LEN)
    {
        command_data[0] = 0;
        command = COMMAND_NONE;
        serial_write_string("cdata over\n");
    }
    else
    {
        //append char
        command_data[len] = data;
        command_data[len + 1] = 0;
    }
}

static void serial_print_time()
{
    rtc_read(&time);
    snprintf(command_data, COMMAND_LEN, "20%.2d-%.2d-%.2d %.2d:%.2d:%.2d\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
    serial_write_string(command_data);
}

ISR(USART_RX_vect)
{
    //recieved byte
    uint8_t rx_data = UDR0;

    if (command == COMMAND_NONE)
    {
        command = rx_data;
    }
    else
    {
        if (rx_data == COMMAND_END)
        {
            //end of command data, perform command
            if (command == COMMAND_PRINT)
            {
                serial_print_time();
            }
            command_data[0] = 0;
            command = COMMAND_NONE;
        }
        else
        {
            //add data to command_data
            append_command_data(rx_data);
        }
    }
    
}
