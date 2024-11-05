/*
 *  myCmds.c
 *
 *    CH55x CDC CMDs
 *
 *    by jimmy.su @ 2024
 *
 */

#include "myCmds.h"

uint8_t cmdBuf[16];  // cmd input buffer
uint8_t cmdBufLast[16];
uint32_t iCmdTimerSec;

//CMD flag control
bool isCmdJog=0;
bool isCmdTimer=0;
bool isCmdBlink=1;

typedef struct {
    const char* cmd;
    const char* helper;
    void (*function)(void);
    uint8_t optionBytes;   //How many option bytes?

} cmdDictionary;
//=============================================================================
const cmdDictionary tableCmds[] = {
    {"help",    "show this help!",                              cmdHelp,    0},
    {"blink",   "led on|off blink [1|0]",                       cmdBlink,   0},
#ifdef _ENABLE_JOG
    {"jog",     "jogging 180BPM [99] 1~99 min.",                cmdJog,     1},
#endif
#ifdef _ENABLE_NEO
    {"neo",     "neopixel color <R> <G> <B> 0~255",             cmdNeo,     3},
#endif
    //{"pinmode", "pin Mode <A|C|D0~7> <AN|FL|PU|PD |OP|OD>",     cmdPinMode},
    //{"pin",     "pin IO <A|C|D0~7> [0|1]",                      cmdPin},
#ifdef _ENABLE_PLAY
    {"play",    "play [beep|+|-|v|135|531|dido|dodi|ohoh]",     cmdPlay,    1},
#endif
#ifdef _ENABLE_SLEEP
    {"sleep",   "sleep until uartD6RX!",                        cmdSleep,   0},
#endif
#ifdef _ENABLE_TIMER
    {"timer",   "timer [999] min.",                             cmdTimer,   2},
#endif
    {"tone",    "tone <0~9999>Hz",                              cmdTone,    0},
#ifdef _ENABLE_EE
    {"EE",      "Option Data EE [i] [HH]",                      cmdEE,      0},
#endif
    {"reset",   "reset me!",                                    cmdReset,   0},

    /*---------------------------------------------------*/
    {"/ | ?",    "re-play | show this help!", cmdHelp}};
//=============================================================================
void cmdHelp(void) {
    char iCmdCount = sizeof(tableCmds) / sizeof(cmdDictionary);
 
    printf("\ncmd HELP\n");
    for (int i = 0; i < 50; i++) printf("_");
    printf("\n");
    uint8_t iOptionBytedIdx=2; //skip data0,data1
    for (int i = 0; i < iCmdCount; i++) {
        printf("\033[" ANSI_YELLOW "m");
        printf("%s", tableCmds[i].cmd);     //print cmd
        printf("\033[0m");
        
        //print index of OB
        #ifdef _ENABLE_EE
        if (tableCmds[i].optionBytes >0)
            printf("\tEE[%u]", iOptionBytedIdx);
        else
            printf("\t      ");
        #endif

        printf("\t%s\n", tableCmds[i].helper); //print helper

        iOptionBytedIdx += tableCmds[i].optionBytes; //count idx
    }
    for (int i = 0; i < 50; i++) printf("_");
    printf("\n");
}

#ifdef _ENABLE_EE
void cmdBootOptions(void) {
    bool isBootOpt=0;
    char iCmdCount = sizeof(tableCmds) / sizeof(cmdDictionary);
 
    initDataOB(); //get Option Bytes
    uint8_t iOptionBytedIdx=2; //skip data0,data1
    for (int i = 0; i < iCmdCount; i++) {
        //print index of OB
        if (tableCmds[i].optionBytes >0) {
            uint8_t opt = readOBdata(iOptionBytedIdx);
            if (opt) {
                printf("\nOption EE[%u]:%d\t", iOptionBytedIdx, opt);
                sprintf((char*)cmdBuf, "%s %d",  tableCmds[i].cmd, opt);
                if (strcmp(tableCmds[i].cmd, "play")==0) {//syntax change
                    //printf("playTone %d", readOBdata(iOptionBytedIdx));
                    #ifdef _ENABLE_PLAY
                    playTone(readOBdata(iOptionBytedIdx));
                    #endif
                    }
                else if (strcmp(tableCmds[i].cmd, "neo")==0) {//syntax change
                    //printf("neo %02X/%02X/%02X",  readOBdata(iOptionBytedIdx),  readOBdata(iOptionBytedIdx+1),  readOBdata(iOptionBytedIdx+2) );
                    sprintf((char*)cmdBuf, "neo %d %d %d",  readOBdata(iOptionBytedIdx),  readOBdata(iOptionBytedIdx+1),  readOBdata(iOptionBytedIdx+2) );
                    cmdNeo();
                    }
                else {
                    printf("cmd:\"%s\"", cmdBuf);  //print cmd w/ param0
                    tableCmds[i].function(); //execute funtion
                    }
                isBootOpt = 1;
                }
            }
        iOptionBytedIdx += tableCmds[i].optionBytes; //count idx
        }
    if (isBootOpt) {
        printf("\n");cmdPrompt();
        }
    }
#endif
//=============================================================================
//
void cmdMatch(char* s) {
    int iCmd;
    int iCmdCount = sizeof(tableCmds) / sizeof(cmdDictionary);
    bool bCmdOK = false;

    // Serial.printf("Matching cmd %s:", sToken.c_str());
    //printf("Matching cmd:%s ",s);
    for (iCmd = 0; iCmd < iCmdCount; iCmd++) {
        if (strcmp(s, tableCmds[iCmd].cmd) == 0) {  // compare s with cmds
            // tableCmds[i].function();
            bCmdOK = true;
            break;
        }
    }
    if (!bCmdOK) {
        //for (int i=0;i<strlen(s);i++) printf("<%x>", *(s+i));
        printf("Err: %s no found!\n", s);  
        //
    } else {                               // Found cmd
        tableCmds[iCmd].function();
    }
}

//
//
void cmdPrompt(void) {
    printf("CMD>");
}

//=============================================================================
/* cmdParam(char c)
 *
 *  copy cmdBuf, search buffer and
 *  return the parameter at c
 *
 */
//

uint8_t cmdBuf1[16];  // CH559

char* cmdParam(char c) {
    strcpy((char*)cmdBuf1, (char*)cmdBuf);  // copy buf

    char idx;
    char* token;
    idx = 0;
    // printf("cmdParam:%s\n", buf1);
    //  walk through other tokens
    token = strtok((char*)cmdBuf1, " =,");
    while (token != NULL) {
        //printf ("[%d]:%s\n", idx, token);
        if (idx == c) break;
        idx++;
        token = strtok(NULL, " =,");
    }
    //printf("cmdParam%d:%s\n", c, token);
    return token;
}

/* 
 *  hexToInt()
 *
 *      convert hex str to int value
 * 
 */
unsigned int hexToInt(const char* hex) {
    unsigned int result = 0;

    while (*hex) {
        if (*hex > 47 && *hex < 58) /*0123456789*/
            result += (*hex - 48);
        else if (*hex > 64 && *hex < 71) /*ABCDEF*/
            result += (*hex - 55);
        else if (*hex > 96 && *hex < 103) /*abcdef*/
            result += (*hex - 87);

        if (*++hex)
            result <<= 4;
    }

    return result;
}
///////////////////////////////////////////////////////////////////////////////
//
//
#define PFIC_RESETSYS                           ((uint32_t)0x00000080) /* System reset */
#define PFIC_KEY3                               ((uint32_t)0xBEEF0000)
void cmdReset(void) {

    printf("\033[2J\033[H"); //Clear all, home   
    //DLY_ms(100);  //system.h
    //Delay_Ms(100); //debug.h

    //RST_now(); //system.h
    PFIC->CFGR    = PFIC_RESETSYS | PFIC_KEY3;
}

void cmdTone(void) {
    uint16_t hz =  atoi(cmdParam(1));
    if (hz) 
        tone(hz);
    else
        notone();

    printf("\ntone=%03dHz\n", hz);
}
/*
void cmdPinMode(void) {
    char *portpin = cmdParam(1); //A0~A7, C0~C7, D0~D7
    char *mode = cmdParam(2);
 
    char port = *portpin;
    if ((port == 'A') || (port == 'a') ) 
        printf("PortA");
    else if ((port == 'C') || (port == 'c') ) 
        printf("PortC");
    else if ((port == 'D') || (port == 'd') ) 
        printf("PortD");
    else
        printf("N/A");

    uint8_t pin = atoi(portpin+1);
    if ((pin<8) && (pin>0))
        printf("-%d", pin);
    printf("\n");

}

void cmdPin(void) {
    char *portpin = cmdParam(1); //A0~A7, C0~C7, D0~D7
}
*/
#ifdef _ENABLE_BLINK
void cmdBlink(void) {
    int iOn = atoi(cmdParam(1));
    if (iOn)
        isCmdBlink=1;
    else
        isCmdBlink=0;
    printf("\nBlink %s\n", isCmdBlink? "On":"Off");
 }
#endif

#ifdef _ENABLE_NEO
//cmdNeo
//
//
void cmdNeo(void) {
    uint8_t red = atoi(cmdParam(1));
    uint8_t grn = atoi(cmdParam(2));
    uint8_t blu = atoi(cmdParam(3));
	
    for (int i=0;i<6;i+=2) {
        myNeo[i][0] = red; //even
        myNeo[i][1] = grn;
        myNeo[i][2] = blu;
        myNeo[i+1][0] = 0; //odd
        myNeo[i+1][1] = 0;
        myNeo[i+1][2] = 0;
        NEO_writeColor(i, myNeo[i][0], myNeo[i][1], myNeo[i][2]);
        }
	NEO_update();		
    printf("\nNeo:%d/%d/%d\n", red,grn,blu);
}
#endif
#ifdef _ENABLE_JOG
//cmdJog
//
// Jog180 [min]
void cmdJog(void) {
    int iMin = atoi(cmdParam(1));
    if (iMin >0) {
        isCmdJog = true;
        iCmdTimerSec = iMin*60;
        isCmdTimer = 1;
        }
    else {
        isCmdJog = 0;
        isCmdTimer = 0;
        }

    notone();
    printf("\nJog180 is %s\n", isCmdJog?"ON":"OFF");
      
    printf("\nTimer %s\n", isCmdTimer ? "On":"Off");
    if (isCmdTimer==0)   printf("\033[0m\a");       //reset ANSI       
}
#endif
#ifdef _ENABLE_SLEEP
void cmdSleep(void) {
    mainSleep();
}
#endif
#ifdef _ENABLE_PLAY
void cmdPlay(void){
     char *list = cmdParam(1);

     play(list);

     printf("\n");
}
#endif
#ifdef _ENABLE_TIMER
void cmdTimer(void) {
    int iMin = atoi(cmdParam(1));
    iCmdTimerSec = iMin * 60; 
    isCmdTimer = (iMin > 0) ? 1:0;

    printf("\nTimer %s\n", isCmdTimer ? "On":"Off");
    if (isCmdTimer==0)   printf("\033[0m\a");                            //bell, Normal color
}
#endif
#ifdef _ENABLE_EE
/*
 * cmdEER()
 *
 *  read data flash
 */
void cmdEER(void) {
    uint16_t i, k;
    printf("\n\033[" ANSI_CYAN "m");
    printf("CH32V00x OB(0x%lx) Bytes:%d\r\n", (uint32_t)0x1FFFF800, (uint8_t )getOBsize());

    for (i = 0; i < 16; i++) {
        printf("%02X ", i);
        if (i==7) printf(" ");
        }
    printf("\r\n");
    printf("\033[0m");
    for (i = 0; i < 16; i++) printf("---");
    printf("\r\n");

    initDataOB();   //read OBdata
    for (i = 0, k = 1; i < getOBsize(); i++) {
        printf("%02X ", readOBdata(i));  // 0xF000~0xF3FF
        if (k==8) printf(" "); //k start from 1
        if ((k++) > 15) {
                printf("\n");
                k = 1;
            }

    }
    printf("\r\n");
    for (i = 0; i < 16; i++) printf("===");
    printf("\n");
}

/*
 * cmdEEW()
 *
 *  write data flash
 */
void cmdEEW(void) {
    uint8_t addr = hexToInt(cmdParam(1));
    uint8_t val = hexToInt(cmdParam(2));

    initDataOB(); //read OBdata
    writeOBdata(addr, val);
    commitOBdata();

    printf("\nEE[%02X]:%02X\n", addr, val);
}

void cmdEE(void) {
    if (cmdParam(1)!=NULL)
        cmdEEW();
    else
        cmdEER(); 
}
#endif
#ifdef _ENABLE_PLAY
//==========================================================================
                        //0    1   2   3    4     5      6     7       8
const char *playlist[]={"beep","+","-","v","135","531","dido","dodi","ohoh"};
 
//playTone
//
//  Play by tone #
//
void playTone(uint8_t t) {
    switch (t) {
        case 0: toneMs(NOTE_C6, 300); break;
        case 1: for (int i=NOTE_C3; i<NOTE_C6; i++) toneUs(i, 600); break;
        case 2: for (int i=NOTE_C5; i>NOTE_C2; i--) toneMs(i, 1); break;
        case 3: toneMs(NOTE_C4, 100);toneMs(NOTE_E4, 100);toneMs(NOTE_G4, 200);toneMs(0, 200);
                toneMs(NOTE_C5, 600);toneMs(0, 100); toneMs(NOTE_B4, 100);toneMs(NOTE_C5, 1000);
                break;
        case 4: toneMs(NOTE_C4, 100);toneMs(NOTE_E4, 100);toneMs(NOTE_G4, 400);break;
        case 5: toneMs(NOTE_G4, 100);toneMs(NOTE_E4, 100);toneMs(NOTE_C4, 600);break;
        case 6: toneMs(NOTE_E5, 100);toneMs(NOTE_C5, 400);break;
        case 7: toneMs(NOTE_C5, 200);toneMs(NOTE_E5, 100);break;
        case 8:
        default: toneMs(NOTE_C6, 200); toneMs(NOTE_G5, 600); break;
        }
    notone();
    }

//play
//
// play w/ name
void play(char *list) {
     uint8_t idx =0;
     bool isOK=0;
     while (playlist[idx] != NULL) {
        //printf("%d %s\n", idx, playlist[idx]);
        if (strcmp(list, playlist[idx])==0) {
            playTone(idx);
            isOK=1;break;   
            }
        idx++;
        }
    if (!isOK) playTone(99);
}
#endif