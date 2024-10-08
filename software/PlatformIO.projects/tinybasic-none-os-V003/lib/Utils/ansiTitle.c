/*         
 * myDate.c
 *
 *         01234567890                  
 *    Date Jul 11 2024  
 * 
 * */
#include <ch32v00x.h>
#include <debug.h>
#include "myUsartInt.h"
#include "ansiTitle.h"

void ansiTitle(const char *title) {
  char buf[24];
  UartBufferSend("\033[2J\033[H"); //Clear all, home    
  UartBufferSend("\033[2J\033[H"); //Clear all, home        // Clear screen
  UartBufferSend("\033[" ANSI_GREEN "m");                   // Green
 
  UartBufferSend(__TIME__);   
  UartBufferSend(" ");  
  UartBufferSend(__DATE__);                                 // Build timestamp
  sprintf(buf, " #%08lx\n\r",  DBGMCU_GetDEVID());          // Chip ID
  UartBufferSend(buf);
  UartBufferSend("\033[" ANSI_YELLOW "m");                  // Yellow
                                                            // 
  UartBufferSend("\033[1m");                                // BOLD
  sprintf(buf, "___%s___\n\r", title);                      // Project Name
  UartBufferSend(buf);
  UartBufferSend("\033[0m\a");                              //bell               
}

