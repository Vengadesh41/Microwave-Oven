/* 
 * File:   main.h
 * Author: VENGADESH
 *
 * Created on 30 March, 2026, 7:38 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "matrix_keypad.h"
#include "timers.h"
#include "clcd.h"

#define _XTAL_FREQ 20000000

#define BUZZER              RC1
#define BUZZ_DDR            TRISC1

#define ON                  1
#define OFF                 0

#define FAN                 RC2
#define FAN_DDR             TRISC2

#define MENU_MODE           0x01
#define SET_TIME            0x02
#define DISPLAY_TIMER       0x03
#define CONVECTION          0x04
#define START               0x05

#define RESET_NOTHING       0x10
#define CONVECTION_RESET    0x11
#define DISPLAY_RESET       0x12
#define RESET_TIME          0x13
#define NO_ACTION           0x14

#define STOP_COUNTDOWN      0
#define START_COUNTDOWN     1

void power_on(void);
unsigned char set_time(unsigned char key, unsigned char reset_flag);
unsigned char convection(unsigned char key, unsigned char reset_flag);
unsigned char display_timer(unsigned char key, unsigned char reset_flag);

#endif	/* MAIN_H */

