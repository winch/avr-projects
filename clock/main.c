
#include "display.h"
#include "rtc.h"
#include <util/delay.h>

int main(void)
{
    struct rtc_time time;
    
    display_init();
    display_test();
    
    rtc_init(&time);
    serial_init();
    
    while(1)
    {
        rtc_read(&time);
        
        serial_write('a');
        serial_write('g');
        serial_write('H');
        serial_write('6');
        serial_write('>');
        serial_write('0' + time.second / 10);
        serial_write('0' + time.second - ((time.second / 10) * 10));
        serial_write(10);
        
        display_set(time.hour / 10, time.hour - ((time.hour / 10) * 10), time.minute / 10, time.minute - ((time.minute / 10) * 10));
        
        _delay_ms(10000);
    }
   
    return 0;
}

