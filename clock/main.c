
#include <util/delay.h>
#include <inttypes.h>

#include "display.h"
#include "rtc.h"

int main(void)
{
    struct rtc_time time;
    
    int rtc_count = 21;
    
    display_init();
    display_test();
    
    rtc_init();
    serial_init();
    
    while(1)
    {
        if (rtc_count > 20)
        {
            rtc_count = 0;
            rtc_read(&time);
            display_set(time.hour / 10, time.hour - ((time.hour / 10) * 10), time.minute / 10, time.minute - ((time.minute / 10) * 10));
        }
        
        serial_do_command();
        
        rtc_count ++;
        _delay_ms(500);
    }
   
    return 0;
}

