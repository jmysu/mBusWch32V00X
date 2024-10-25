/*
 *  cdcCMDs.h
 *
 *    CH55x CDC with simple cmd-line commands!
 * 
 *    by jimmy.su @ 2024
 * 
 */
#ifndef _MYCMDS_H
#define _MYCMDS_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "ansiColors.h"
#include "debug.h" //Delay_Ms()
#include "gpio.h"
#include "neo_sw.h"

/*
#include "ch32v00x.h"
#include "gpio.h"
#include "system.h"

#include "ch32x035_gpio.h"
#include "ch32x035_rcc.h"
#include "usb_cdc.h"
#include "ttbasic.h"
*/
//NeoPixel
//#include "bitbang2812.h"
//#include "neo.h"
//#define printf CDC_printf

extern uint8_t cmdBuf[];
extern void cmdMatch(char *s);
extern char* cmdParam(char c);
extern void cmdPrompt(void);
extern void cmdHelp(void);
extern void cmdTone(void);
extern void cmdPin(void);
extern void cmdPinMode(void);
extern void cmdNeo(void);
extern uint8_t neo_buf[6][3];

/*
//forware declaration
extern void basic(void);
extern void cmdBasic(void);

extern void cmdHeap(void);
extern void cmdBL(void);
extern void cmdPAW(void);
extern void cmdPAR(void);
extern void cmdPBW(void);
extern void cmdPBR(void);
extern void cmdEER(void);
extern void cmdEEW(void);
*/
extern void cmdReset(void);

#endif