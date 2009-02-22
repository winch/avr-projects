
#ifndef _RTC_H
#define _RTC_H

struct rtc_time
{
  char hour;
  char minute;
};

void rtc_init(struct rtc_time *time);

void rtc_read(struct rtc_time *time);

#endif

