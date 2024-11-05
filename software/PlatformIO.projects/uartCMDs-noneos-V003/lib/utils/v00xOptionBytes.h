
#include <ch32v00x.h>
#include <ch32v00x_flash.h>
#include <stdbool.h>
#include "_main.h"

//#define SIZE_OB26 26   // Option bytes available on CH32V003: 64-16=48; 48/2=24, 2+24=26

extern uint32_t ReadOptionByte01(void);

extern uint8_t getOBsize(void);
extern void initDataOB(void);
extern uint8_t readOBdata(int const idx);
extern uint8_t *getOBdataPtr(void);
extern void writeOBdata(int const idx, uint8_t const val);
extern void eraseOBdata(void);
extern bool commitOBdata(void);