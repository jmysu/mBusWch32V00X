/*
 * main C program for project uartCMDs-noneos-v003
 *
 *
 * 	CH32V003 uart commander
 *
 *
 * 	Buzzer: 	C4 (T1C4 PWM)
 * 	Neopixel:	C3
 * 	LED:		D0
 *  SWIO:		D1
 * 	uartTx:		D5
 * 	uartRx:		D6
 *  i2cSDA:     C1
 *  i2cSCL:     C2
 *
 * 	V1.0
 *    ----------------------------------------------------------------
 * 	RAM:   [=====     ]  46.1% (used 944 bytes from 2048 bytes)
 * 	Flash: [=======   ]  65.2% (used 10688 bytes from 16384 bytes)
 *    ----------------------------------------------------------------
 *
 * 	V1.1
 * 		-Add Version (ansiTitle.c)
 * 		-Add Super jogging 180 BPM Metronome (_main.c)
 * 	----------------------------------------------------------------
 *	RAM:   [=====     ]  46.3% (used 948 bytes from 2048 bytes)
 *  Flash: [=======   ]  66.1% (used 10832 bytes from 16384 bytes)
 * 	----------------------------------------------------------------
 * 
 *  _ENABLE_SLEEP   14696bytes
 */
#if defined(CH32V00X)
#include <ch32v00x.h>
#elif defined(CH32V10X)
#include <ch32v10x.h>
#elif defined(CH32V20X)
#include <ch32v20x.h>
#elif defined(CH32V30X)
#include <ch32v30x.h>
#endif
#include <ch32v00x_pwr.h>
#include <ctype.h>
#include <debug.h>

#include "_main.h"
// #include "timer2irq.h"
#include "ansiColors.h"
#include "myCmds.h"
#include "myUsartInt.h"
#include "neo_sw.h"
#include "i2cSlave.h"
#include "lib_i2c.h"

extern void i2cSlave(void);
extern void oledHello(void);
//extern int i2cScan(void);
extern int testSD(void);

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
 
void initC3C4D0(void);
void TIM1CH4_PWMOut_Init(u16 arr, u16 psc, u16 ccp);
 
uint32_t _millis = 0;
uint8_t _hour=0, _min=0, _sec=0;
 
uint32_t _millisLastJog=0;
int jogstep = 0; //jog sound control
 
extern void ansiTitle(const char *title);

uint8_t cmdKB[16];
uint8_t cmdKBptr = 0;
#define kbhit() _RxCount
// char cmdBuf[MAX_RX_COUNT];
//char cPtr = 0;
#ifdef _ENABLE_TIMER
// updateTime
// update hour, min, sec from _)millis
void updateTime() {
    if (_millis>999){//do sec routine---------------
        // Update Neopixels
        uint8_t k = _sec%6; //0~5 RGBCMY
        
        #ifdef _ENABLE_BLINK
         if (isCmdBlink) {
            #ifdef _ENABLE_NEO
             NEO_writeColor(0, myNeo[k][0], myNeo[k][1], myNeo[k][2]);
             NEO_update();
            #endif
            // toggle D0
            GPIO_WriteBit(GPIOD, GPIO_Pin_0, (k%2)?Bit_SET:Bit_RESET);
            }
        #endif
        
        // Update Timer
        if (isCmdTimer) {
            if (iCmdTimerSec-- > 0) {
            //printf("%lu", iCmdTimerSec);
                uint8_t _h = iCmdTimerSec/(60*60);
                uint8_t _m = (iCmdTimerSec-(_h*3600))/60;
                uint8_t _s = (iCmdTimerSec-(_h*3600)-(_m*60))%60;
        
                printf("\033[A");   //up one line
                printf("\33[2K\r"); //erase line
                printf("\033[" ANSI_YELLOW "m");  
                printf("\033[1m");  // BOLD
                printf("%3u:%02u:%02u", _h, _m, _s);
                }
            else {
                isCmdTimer=0; //stop timer
                if (isCmdJog) isCmdJog=0; //turn off Jog if on!
                printf("\n");
                printf("\033[0m\a"); // Normalied, bell               
                cmdPrompt();
               
                play("v");
                }
            }
        _millis=0;
        ++_sec;
        if (_sec>59) {
            _sec=0;_min++;
            if (_min>59) {
                _min=0;_hour++;
                }
            }
        }
}
#endif

 
int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();

    USARTx_CFG();  // config USART1 tx/rx
    // TIM2_INT_Init(1000-1,  48000-1); // (48000-1)/48,000,000 * (1000-1) = 1 second.
    // TIM2_INT_Init(1000-1,  480-1); // (480-1)/48,000,000 * (100-1) = 10 milli-second.

    /*
    #if (SDI_PRINT == SDI_PR_OPEN)
            SDI_Printf_Enable();
    #else
            USART_InitTxRx(115200);
    #endif
    
    printf("SystemClk:%lu\r\n", SystemCoreClock);
#if defined(CH32V30X)
    printf("ChipID: %08x\r\n", (unsigned)DBGMCU_GetCHIPID());
#else
    printf("DeviceID: %08x\r\n", (unsigned)DBGMCU_GetDEVID());
#endif
    */
    #ifdef _ENABLE_ANSITITLE
     ansiTitle("V0003 uartCMDs");
    #endif

    initC3C4D0(); //for neo, buzzer, GPIO LED
 
    #ifdef _ENABLE_NEO
     NEO_init();
    #endif

    #ifdef _ENABLE_I2C
    //i2cScan();
    i2c_init(100000);
    i2cScan();
    #endif
    
    #ifdef _ENABLE_OLED
    //OLED
    oledHello();
    #endif

    #ifdef _ENABLE_SDFS
    testSD();
    #endif

    #ifdef _ENABLE_I2C_SLAVE
    i2cSlave();
    #endif

    cmdHelp();
    cmdPrompt();
 
    #ifdef _ENABLE_EE
    cmdBootOptions();
    #endif
    while (1) {
        Delay_Us(999);
        _millis++;
        updateTime();

        #ifdef _ENABLE_JOG
        if (isCmdJog) {
            if ((_millis -_millisLastJog) > 83) {  // 60000/180bpm = 333ms,  333/4=83
                _millisLastJog = _millis;
/*
                if (jogstep==0) {
                    tone(NOTE_C5);
                    }
                else{
                    tone(0);
                    }
                ++jogstep;
                if (jogstep>15) jogstep=0;
*/
                jogstep++;
                if (jogstep>15) jogstep=0;
                switch (jogstep%16) {
                    case 0: tone(NOTE_C6);break;
                    case 4: tone(NOTE_C3);break;
                    case 8: tone(NOTE_C3);break;
                    case 12: tone(NOTE_C3);break;
                    default: tone(0);break;
                    }
                }
 
        }  // isCmdJog
        #endif
 
        char c = getRx();  // get incoming character
        if ((c == '\r')) {
            /*
            printf("tone\n");
            for (int i=NOTE_C2; i<NOTE_C6; i++) 
                {tone(i); Delay_Us(800);}
            printf("toneUs\n");
            for (int i=NOTE_C3; i<NOTE_C6; i++) 
                toneUs(i, 800); 
            printf("toneMs\n");
            for (int i=NOTE_C3; i<NOTE_C6; i++) 
                toneMs(i, 1); 
            continue;
            */
            // printf("\nCMD>%s", cmd);
            if (strcmp((char*)cmdKB, "/") == 0) {  // Check re-play
                if (*cmdBufLast) { //history cmd exist!
                    strcpy((char *)cmdBuf, (const char *)cmdBufLast); //copy last history back to cmdBuf
                    printf("\n/%s", cmdBuf);
                } else
                    printf("\n/None!");
            } else
                strcpy((char *)cmdBuf, (const char *)cmdKB);//COPY to cmdBuf

            char *myCmd = cmdParam(0); //get first cmd from cmdBuf
            if (*myCmd) {
                cmdMatch(myCmd);  // Matching cmd
                cmdPrompt();
                // save cmdBufLast
                if (myCmd)
                    strcpy((char *)cmdBufLast, (const char *)cmdBuf); //backup last history
            }
            for (int i = 0; i < cmdKBptr; i++) cmdKB[i] = 0;    // clear All cmd kb buffer
            cmdKBptr = 0;                                       // reset cmd kb buffer
        } else if (isprint(c)) {
            cmdKB[cmdKBptr++] = c;  // Add c to cmd buffer
            printf("%c", c);        // Echo it
        }
 
    }
}

void NMI_Handler(void) {}
void HardFault_Handler(void) {
    while (1) {
    }
}
