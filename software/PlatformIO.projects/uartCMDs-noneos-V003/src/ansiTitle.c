/*         
 * myDate.c
 *
 *         01234567890                  
 *    Date Jul 11 2024  
 * 
 * */
#include <stdio.h>

#include "ansiColors.h"
//#include "usb_cdc.h"
 #include "system.h" //for ESIG
extern uint32_t DBGMCU_GetCHIPID( void );

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
  printf("___%s___\n\r", title);                  // Project Name
  printf("\033[0m\a");                            //bell               
}

