
#include <avr/io.h>
#include <avr/interrupt.h>

// 4800baud @ 1mhz
#define BAUD_PRESCALE 12

volatile uint8_t rx_data;

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

void serial_write(uint8_t data)
{
    while ((UCSR0A & (1 << UDRE0)) == 0) ;
    UDR0 = data;
}


ISR(USART_RX_vect)
{
   //recieved byte
   rx_data = UDR0;
   //serial_write(data);
}

