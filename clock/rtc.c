
#include <inttypes.h>

#include "rtc.h"
#include "twi.h"

//rtc chip address
#define DS3231 0x68

void rtc_init(struct rtc_time *time)
{
    twi_init();
    
    //time
    time->hour = 7;
    time->minute = 8;
}

void rtc_read(struct rtc_time *time)
{
    uint8_t data;
    //write memory location to read from
    twi_start_write(DS3231);
    twi_write(0);
    twi_stop();
    
    //read from rtc
    twi_start_read(DS3231);

    //read seconds
    data = twi_read(MORE);
    time->second = ((data >> 4) * 10) + (data & 0xf);

    //read minutes
    data = twi_read(MORE);  
    time->minute = ((data >> 4) * 10) + (data & 0xf);
    
    //read hours
    data = twi_read(FINISHED);
    time->hour = data & 0xf;

    twi_stop();    
}

void rtc_write(struct rtc_time *time)
{
    //
}

