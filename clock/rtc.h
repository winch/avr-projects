
#ifndef _RTC_H
#define _RTC_H

struct rtc_time
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    char period;
};

void rtc_init();

void rtc_read(struct rtc_time *time);

void rtc_write(struct rtc_time *time);

#endif

