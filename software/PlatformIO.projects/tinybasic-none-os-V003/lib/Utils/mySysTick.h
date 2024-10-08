/*
 *  mySysTick.h
 *
 * 
 *      CH32V003 SysTick Utility
 * 
 *      by jimmy.su 
 */
#ifndef _MYSYSTICK_H
#define _MYSYSTICK_H

// Global System Timebase Precision (increment global systick counter every second, millisecond or microsecond)
// Higher precision means more counter increment ISR calls
enum {
	SYSTICK_SECONDS = 0,
	SYSTICK_MILLIS,
	SYSTICK_MICROS
};

extern void systick_init(uint32_t iPrecision);
extern uint32_t systick_get(void);

#endif