
#include "display.h"
#include "rtc.h"
#include <util/delay.h>

int main(void)
{
    struct rtc_time time;
    
    display_init();
    display_test();
    
    rtc_init(&time);
    
    while(1)
    {
        rtc_read(&time);
        
        display_set(time.hour / 10, time.hour - ((time.hour / 10) * 10), time.minute / 10, time.minute - ((time.minute / 10) * 10));
        
        _delay_ms(10000);
    }
   
    return 0;
}

