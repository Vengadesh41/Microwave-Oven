
#include <xc.h>
#include "main.h"

extern unsigned char countdown_flag; 
extern int min, sec;
extern unsigned char blink;

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    
    if (TMR2IF == 1)
    {
        count++;
        
        if (count % 625 == 0)
        {
            blink = !blink;
        }
        
        if (count == 1250)
        {
            count = 0;
            
            if (countdown_flag == START_COUNTDOWN) 
            {
                if(sec > 0)
                {
                    sec--;
                }
                else if(sec == 0 && min > 0)
                {
                    min--;
                    sec = 59;
                }
            }
        }
        TMR2IF = 0;
    }
}