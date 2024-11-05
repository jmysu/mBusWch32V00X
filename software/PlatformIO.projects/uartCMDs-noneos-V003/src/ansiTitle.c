/*         
 * myDate.c
 *
 *         01234567890                  
 *    Date Jul 11 2024  
 * 
 * */
#include <stdio.h>
#include <stdint.h>

#include "ansiColors.h"
#include "_main.h"
//#include "usb_cdc.h"
//#include "system.h" //for ESIG
// ===================================================================================
// Device Electronic Signature (ESIG)
// ===================================================================================
#define   __I     volatile const      /*!< defines 'read only' permissions    */
#define ESIG_FLASHSIZE      (*(__I uint16_t*)(0x1FFFF7E0))
#define ESIG_UID1           (*(__I uint32_t*)(0x1FFFF7E8))
#define ESIG_UID2           (*(__I uint32_t*)(0x1FFFF7EC))
#define ESIG_UID3           (*(__I uint32_t*)(0x1FFFF7F0))

extern uint32_t DBGMCU_GetCHIPID( void );

#define VerH  1
#define VerL  1

#ifdef _ENABLE_ANSITITLE
void ansiTitle(const char *title) {
  printf("\033[2J\033[H"); //Clear all, home    
  printf("\033[2J\033[H"); //Clear all, home                                          // Clear screen
  printf("\033[" ANSI_GREEN "m");                                                     // Green
 
  printf(__TIME__);   
  printf(" ");  
  printf(__DATE__);                                                                   // Build timestamp
  printf(" CH32x0%lx(%dK) %lx-%lx\n\r",  DBGMCU_GetCHIPID()>>16, ESIG_FLASHSIZE,  ESIG_UID1, ESIG_UID2);                             // Chip ID
  printf("\033[" ANSI_YELLOW "m");                                                    // Yellow
                                                                                      // Get date in number from __DATE__
  printf("\033[1m");                                                                  // BOLD
  printf("___%s___ V%d.%d\n\r", title, VerH, VerL);                  // Project Name
  printf("\033[0m\a");                            //bell               
}
#endif

