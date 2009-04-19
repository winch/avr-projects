
#include <inttypes.h>

#include "rtc.h"
#include "twi.h"

//rtc chip address
#define DS3231 0x68

void rtc_init()
{
    twi_init();
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
    data = twi_read(MORE);
    time->hour = ((data & 0x10) >> 4) * 10;
    time->hour += data & 0x0f;
    //period
    if (data & 0x20 == 0x20)
        time->period = 'p';
    else
        time->period = 'a';
    
    //weekday number
    data = twi_read(MORE);
    
    //day
    data = twi_read(MORE);
    time->day = ((data >> 4) * 10) + (data & 0xf);
    
    //month
    data = twi_read(MORE);
    time->month = ((data >> 4) * 10) + (data & 0xf);
    
    //year
    data = twi_read(FINISHED);
    time->year = ((data >> 4) * 10) + (data & 0xf);

    twi_stop();    
}

static uint8_t value_to_bcd(uint8_t value)
{
  uint8_t bcd;
  bcd = (value / 10) << 4;
  bcd |= (value - (value / 10) * 10) & 0xf;
  return bcd;
}

void rtc_write(struct rtc_time *time)
{
    uint8_t bcd;
    //write memory location to read from
    twi_start_write(DS3231);
    twi_write(0);
    
    //second
    twi_write(value_to_bcd(time->second));
    //minute
    twi_write(value_to_bcd(time->minute));
    //hour
    bcd = value_to_bcd(time->hour);
    if (time->period == 'p')
        bcd |= 0x20; //set am/pm bit to pm 
    else
        bcd &= 0xdf; //clear am/pm bit to am
    bcd |= 0x40; //set 12h bit
    twi_write(bcd);
    //week day
    twi_write(0);
    //day
    twi_write(time->day);
    //month
    twi_write(time->month);
    //year
    twi_write(time->year);
       
    twi_stop();
}

