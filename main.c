/*
 * File:   main.c
 * Author: VENGADESH
 *
 * Created on 30 March, 2026, 7:37 PM
 */


#include <xc.h>
#include "main.h"

//turn of WDT
#pragma config WDTE = OFF

int min = 0;
int sec = 0;
unsigned char countdown_flag;

void init_config(void) 
{
    init_clcd();
    init_matrix_keypad();
    init_timer2();
    /*for the buzzer*/
    BUZZ_DDR = 0;
    BUZZER = OFF;
    /*for the fan*/
    FAN_DDR = 0;
    FAN = OFF;

    
    PEIE = 1;
    GIE = 1;
}

void main(void) 
{
    init_config(); 
    
    unsigned char key;
    unsigned char operation_mode = MENU_MODE;
    unsigned char reset_flag = RESET_NOTHING;
    
    power_on(); 
    clear_screen();

    while (1) 
    {
        key = read_matrix_keypad(STATE);
        
        switch (operation_mode)
        {
            case MENU_MODE:
                clcd_print("1.Micro", LINE1(0));
                clcd_print("2.Grill", LINE2(0));
                clcd_print("3.Convection", LINE3(0));
                clcd_print("4.Start", LINE4(0));
                
                if (key == 1 || key == 2)
                {
                    operation_mode = SET_TIME;
                    reset_flag = RESET_TIME;
                }
                else if (key == 3)
                {
                    operation_mode = CONVECTION;
                    reset_flag = CONVECTION_RESET;
                }
                else if (key == 4)
                {
                    min = 0;
                    sec = 30;                    
                    operation_mode = DISPLAY_TIMER;
                    reset_flag = DISPLAY_RESET;
                }
                break;

            case SET_TIME:
                if (set_time(key, reset_flag) == DISPLAY_TIMER)
                {
                    operation_mode = DISPLAY_TIMER;
                    reset_flag = DISPLAY_RESET;
                }
                else
                {
                    reset_flag = RESET_NOTHING;
                }
                break;

            case CONVECTION:
                if (convection(key, reset_flag) == SET_TIME)
                {
                    operation_mode = SET_TIME;
                    reset_flag = RESET_TIME;
                }
                else
                {
                    reset_flag = RESET_NOTHING;
                }
                break;

            case DISPLAY_TIMER:
                if (display_timer(key, reset_flag) == MENU_MODE)
                {
                    operation_mode = MENU_MODE;
                    clear_screen();
                }
                else
                {
                    reset_flag = RESET_NOTHING;
                }
                break;
        }
    }
}