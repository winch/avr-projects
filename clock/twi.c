
#include <avr/io.h>
#include <util/twi.h>

#include "twi.h"

void twi_init()
{
    //inable pull up resistors on i2c pins
    DDRC &= ~(1 << PC4);
    DDRC &= ~(1 << PC5);
    PORTC |= (1 << PC4) | (1 << PC5);
    
    //twi clock
    TWSR = 0;
    TWBR = 10;
}

static int twi_start()
{
    //transmit start condition
    TWCR = (1 << TWINT) | (1 <<TWSTA) | (1 <<TWEN);
    while ((TWCR & (1 << TWINT)) == 0) ;
    if (TW_STATUS != TW_START)
        ;//return 1;
    return 0;
}

int twi_start_read(uint8_t address)
{
    //start a read from address
    twi_start();
    //sla + r
    TWDR = (address << 1) | TW_READ;
    //start transmission
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0) ;
    if (TW_STATUS == TW_MR_DATA_ACK)
        ;
    
    return 0;
}

int twi_start_write(uint8_t address)
{
    //start a write to address
    twi_start();
    
    //sla + w
    TWDR = (address << 1) | TW_WRITE;
    //start transmission
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0) ;
    if (TW_STATUS == TW_MR_DATA_ACK)
        ;//return 2;
    return 0;
}

uint8_t twi_read(enum twi_read_mode mode)
{
    if (mode == more)
        TWCR = (1 << TWINT) | (1 << TWEN) | (1<<TWEA);
    else
        TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0) ;
    return TWDR;
}

int twi_write(uint8_t data)
{
    //write data
    TWDR = data;
    //start transmission
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0) ;
    if (TW_STATUS != TW_MT_DATA_NACK)
        ;//return 3;
    return 0;
}

void twi_stop()
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 <<TWEN);
    while ((TWCR & (1 << TWSTO)) == 0) ;
}

