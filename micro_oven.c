#include "main.h"
#include <xc.h>

extern int min;
extern int sec;
unsigned char blink;

char Mins[3];
char Secs[3];
char Temp[4];

static unsigned char key_count;
static unsigned char blink_pos;

extern unsigned char countdown_flag;
void power_on(void)
{
    for(int i = 0; i<16; i++)
    {
        clcd_putch(BLOCK, LINE1(i));
    }
    clcd_print("  Powering ON   ", LINE2(0));
    clcd_print(" Microwave Oven ", LINE3(0));
    for(int i = 0; i<16; i++)
    {
        clcd_putch(BLOCK, LINE4(i));
    }
    __delay_ms(1500);
    
}

unsigned char set_time(unsigned char key, unsigned char reset_flag)
{
    if (reset_flag == RESET_TIME)
    {
        clear_screen();
        clcd_print("SET TIME <MM:SS>", LINE1(0));
        clcd_print("TIME: 00:00", LINE2(0));
        clcd_print("*:CLEAR  #:ENTER", LINE4(0));
        
        sec = 0;
        min = 0;
        key_count = 0;
        TMR2ON = 1;
    }

    if (key != ALL_RELEASED && key != '*' && key != '#')
    {
        if (key_count < 2)
        {
            sec = (sec % 10) * 10 + key;
            blink_pos = 0;
        }
        else if (key_count > 1 && key_count < 4)
        {
            min = (min % 10) * 10 + key;
            blink_pos = 1;
        }
        key_count++;
    }
    else if (key == '*')
    {
        blink_pos = 0;
        key_count = 0;
        sec = 0;
        min = 0;
    }
    else if (key == '#')
    {
        TMR2ON = 0;
        return DISPLAY_TIMER;
    }

    Mins[0] = (min / 10) + '0';
    Mins[1] = (min % 10) + '0';
    Secs[0] = (sec / 10) + '0';
    Secs[1] = (sec % 10) + '0';
    
    if(blink_pos == 0)
    {
        clcd_print(Mins, LINE2(6));
        if(blink == 1)
        {
            clcd_print("  ",LINE2(9));
        }
        else 
        {
            clcd_print(Secs, LINE2(9));
        }
            
    }
    else if(blink_pos == 1)
    {
        clcd_print(Secs, LINE2(9));
        
        if(blink == 1)
        {
            clcd_print("  ", LINE2(6));
        }
        else
        {
            clcd_print(Mins, LINE2(6));
        }
    }
    
    return NO_ACTION;
}

unsigned char convection(unsigned char key, unsigned char reset_flag)
{
    static unsigned char temp = 0;
    static unsigned char set_temp_step = 1;
    
    if (reset_flag == CONVECTION_RESET)
    {
        clear_screen();
        clcd_print("SET TEMP (^C)  ", LINE1(0));
        clcd_print("TEMP: 000", LINE2(0));
        clcd_print("*:CLEAR  #:ENTER", LINE4(0));
        
        TMR2ON = ON;
        
        temp = 0;
        key_count = 0;
        set_temp_step = 1; 
        return NO_ACTION;
    }
    
    if (set_temp_step == 1)
    {
        if (key != ALL_RELEASED && key != '*' && key != '#')
        {
            if (key_count < 3)
            {
                temp = (temp * 10) + key;
                key_count++;
            }
        }
        else if (key == '*')
        {
            temp = 0;
            key_count = 0;
        }
        else if (key == '#')
        {
            if (temp > 250) temp = 250;
            if (temp == 0) temp = 180;
            
            set_temp_step = 0;
            sec = 60;
            countdown_flag = START_COUNTDOWN;
            clear_screen();
        }
        
        Temp[0] = (temp / 100) + '0';
        Temp[1] = ((temp / 10) % 10) + '0';
        Temp[2] = (temp % 10) + '0';
        Temp[3] = '\0';
        
        if (blink) clcd_print(Temp, LINE2(6));
        else clcd_print("   ", LINE2(6));
    }
    else
    {
        
        clcd_print("  Pre-Heating   ", LINE1(0));
        clcd_print(" Time Left: ", LINE2(0));
        
        Secs[0] = (sec / 10) + '0';
        Secs[1] = (sec % 10) + '0';
        Secs[2] = '\0';
        
        clcd_print(Secs, LINE2(12));
        
        if (sec == 0)
        {
            return SET_TIME; 
        }
    }
    
    return NO_ACTION;
}

unsigned char display_timer(unsigned char key, unsigned char reset_flag)
{
    static unsigned char pause;
    if(reset_flag == DISPLAY_RESET)
    {
        clear_screen();
        key = ALL_RELEASED;
        clcd_print("TIME =  ",LINE1(1));
        clcd_print("4.START/RESUME", LINE2(1));
        clcd_print("5.PAUSE", LINE3(1));
        clcd_print("6.STOP", LINE4(1));
        
        countdown_flag = START_COUNTDOWN;
        
        TMR2ON = 1;
        FAN = 1;
    }
    
    if(sec == 0 && min == 0)
    {
        clear_screen();
        TMR2ON = 0;
        FAN = OFF;
        countdown_flag = STOP_COUNTDOWN;
        clcd_print("Time UP!!!", LINE2(4));
        clcd_print("Enjoy your meal", LINE3(0));
        
        BUZZER = ON;
        
        __delay_ms(2500);
        BUZZER = OFF;
        return MENU_MODE;
    }
    
    if(key == 4 && pause == 0)
    {
        sec = sec + 30;
    }
    else if(key == 4 && pause == 1)
    {
        TMR2ON = 1;
        FAN = 1;
    }
    if(key == 5)
    {
        pause = 1;
        FAN = 0;
        TMR2ON = 0;
    }
    if(key == 6)
    {
        TMR2ON = 0;
        FAN = OFF;
        countdown_flag = STOP_COUNTDOWN;
        return MENU_MODE;
    }
    
    Mins[0] = min / 10 + '0';
    Mins[1] = min % 10 + '0';
    Mins[2] = '\0';
    
    Secs[0] = sec / 10 + '0';
    Secs[1] = sec % 10 + '0';
    
    clcd_print(Mins, LINE1(9));
    clcd_putch(':', LINE1(11));
    clcd_print(Secs, LINE1(12));
    
    return NO_ACTION;
}