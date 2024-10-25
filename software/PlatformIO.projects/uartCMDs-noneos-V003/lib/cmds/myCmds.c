/*
 *  myCmds.c
 *
 *    CH55x CDC CMDs
 *
 *    by jimmy.su @ 2024
 *
 */

#include "myCmds.h"

uint8_t cmdBuf[32];  // cmd input buffer
 

typedef struct {
    const char* cmd;
    const char* helper;
    void (*function)(void);
} cmdDictionary;
//=============================================================================
const cmdDictionary tableCmds[] = {
    {"help",    "show this help!",                              cmdHelp},
    {"neo",     "neopixel color <R> <G> <B> 0~255",             cmdNeo},
    //{"pinmode", "pin Mode <A|C|D0~7> <AN|FL|PU|PD |OP|OD>",     cmdPinMode},
    //{"pin",     "pin IO <A|C|D0~7> [0|1]",                      cmdPin},
    {"tone",    "tone <0~9999>Hz",                              cmdTone},
    {"reset",   "reset me!",                                    cmdReset},
    /*
    {"bl", "jump boootloader!", cmdBL},
    {"pAw", "write PA(0~7), pAw <x=<1|0>", cmdPAW},
    {"pA",  "read PA(0~7)", cmdPAR},
    {"pBw", "write PB(0~7), pBw <x=<1|0>", cmdPBW},
    {"pB",  "read PB(0~7)", cmdPBR},
    //{"eew", "write EE(00~3FF), eew <xx=xx>", cmdEEW},
    //{"eer", "read EE", cmdEER},
    //{"neo", "neopixel <index>,<r>,<g>,<b>", cmdNeopixel},
    {"basic", "TinyBasic", cmdBasic},
    */
    /*---------------------------------------------------*/
    {"?", "show this help!", cmdHelp}};
//=============================================================================
void cmdHelp(void) {
    char iCmdCount = sizeof(tableCmds) / sizeof(cmdDictionary);

    printf("\ncmd HELP\n");
    for (int i = 0; i < 50; i++) printf("_");
    printf("\n");
    for (int i = 0; i < iCmdCount; i++) {
        printf("\033[" ANSI_YELLOW "m");
        printf("%s", tableCmds[i].cmd);
        printf("\033[0m");
        printf("\t%s\n", tableCmds[i].helper);
    }
    for (int i = 0; i < 50; i++) printf("_");
    printf("\n");
}

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

uint8_t cmdBuf1[32];  // CH559

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


///////////////////////////////////////////////////////////////////////////////
//
//
#define PFIC_RESETSYS                           ((uint32_t)0x00000080) /* System reset */
#define PFIC_KEY3                               ((uint32_t)0xBEEF0000)
void cmdReset(void) {

    printf("\033[2J\033[H"); //Clear all, home   
    //DLY_ms(100);  //system.h
    Delay_Ms(100); //debug.h

    //RST_now(); //system.h
    PFIC->CFGR    = PFIC_RESETSYS | PFIC_KEY3;
}


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


extern void tone(uint16_t freq);
extern void notone(void);
void cmdTone(void) {
    uint16_t hz =  atoi(cmdParam(1));
    if (hz==0) 
        notone();
    else 
        tone(hz);

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
void cmdNeo(void) {
    uint8_t red = atoi(cmdParam(1));
    uint8_t grn = atoi(cmdParam(2));
    uint8_t blu = atoi(cmdParam(3));
	
    for (int i=0;i<6;i+=2) {
        neo_buf[i][0] = red; //even
        neo_buf[i][1] = grn;
        neo_buf[i][2] = blu;
        neo_buf[i+1][0] = 0; //odd
        neo_buf[i+1][1] = 0;
        neo_buf[i+1][2] = 0;
        }
	NEO_update();		
    printf("\nNeo:%d/%d/%d\n", red,grn,blu);
}