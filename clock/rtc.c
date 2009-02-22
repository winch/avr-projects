
#include <avr/io.h>
#include <util/twi.h>

#include "rtc.h"

//rtc chip address
#define DS3231 0x68

void rtc_init(struct rtc_time *time)
{
    //inable pull up resistors on i2c pins
    DDRC &= ~(1 << PC4);
    DDRC &= ~(1 << PC5);
    PORTC |= (1 << PC4) | (1 << PC5);
    
    //twi clock
    TWSR = 0;
    TWBR = 10;
    
    //time
    time->hour = 7;
    time->minute = 8;
}

void rtc_read(struct rtc_time *time)
{
    //transmit start condition
    TWCR = (1 << TWINT) | (1 <<TWSTA) | (1 <<TWEN);
    while ((TWCR & (1 << TWINT)) == 0) ;
    if (TW_STATUS != TW_START)
        ;//return 1;
    
    //sla + w
    TWDR = (DS3231 << 1) | TW_WRITE;
    //start transmission
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0) ;
    if (TW_STATUS == TW_MR_DATA_ACK)
        ;//return 2;
    
    //write address 0
    TWDR = 0;
    //start transmission
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0) ;
    if (TW_STATUS != TW_MT_DATA_NACK)
        ;//return 3;

    //stop
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 <<TWEN);
    while ((TWCR & (1 << TWSTO)) == 0) ;


    //transmit start condition
    TWCR = (1 << TWINT) | (1 <<TWSTA) | (1 <<TWEN);
    while ((TWCR & (1 << TWINT)) == 0) ;
    if (TW_STATUS != TW_START)
        ;//return 1;

    //sla + r
    TWDR = (DS3231 << 1) | TW_READ;
    //start transmission
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0) ;
    if (TW_STATUS == TW_MR_DATA_ACK)
        ;
    
    //read seconds
    TWCR = (1 << TWINT) | (1 << TWEN) | (1<<TWEA);
	while ((TWCR & (1 << TWINT)) == 0) ;

    //read minutes
    TWCR = (1 << TWINT) | (1 << TWEN) |  (1<<TWEA);
	while ((TWCR & (1 << TWINT)) == 0) ;
        
    time->minute = ((TWDR >> 4) * 10) + (TWDR & 0xf);
    
    //read hours
    TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0) ;
	
	time->hour = TWDR & 0xf;

    
    //stop
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 <<TWEN);
    while ((TWCR & (1 << TWSTO)) == 0) ;
    
}


