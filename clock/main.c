
#include "display.h"

int main(void)
{

    display_init();
    display_test();
    display_set(1, 2, 3, 4);

    while(1)
    {
        ;
    }
   
    return 0;
}

