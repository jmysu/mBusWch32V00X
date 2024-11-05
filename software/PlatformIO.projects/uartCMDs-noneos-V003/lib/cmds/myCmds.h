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
#include "v00xOptionBytes.h"
#include "notes.h"

#include "_main.h" //for modules enable/disable

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
extern uint8_t cmdBufLast[];
extern void cmdMatch(char *s);
extern char* cmdParam(char c);
extern void cmdPrompt(void);
extern void cmdHelp(void);
extern void cmdBootOptions(void);
extern void cmdBlink(void);
extern bool isCmdBlink;

extern void cmdNeo(void);
extern uint8_t myNeo[6][3];

extern void cmdJog(void);
extern bool isCmdJog;
extern void cmdPlay(void);

extern void cmdSleep(void);
extern void mainSleep(void);

extern void tone(uint16_t freq);
extern void notone(void);
//extern void tones(uint16_t freq, uint16_t us);
extern void toneMs(uint16_t freq, uint16_t ms);
extern void toneUs(uint16_t freq, uint32_t us);
extern void cmdTone(void);
extern void play(char *list);

extern void cmdTimer(void);
extern bool isCmdTimer;
extern uint32_t iCmdTimerSec;

extern void cmdEE(void);

extern void cmdReset(void);

#endif