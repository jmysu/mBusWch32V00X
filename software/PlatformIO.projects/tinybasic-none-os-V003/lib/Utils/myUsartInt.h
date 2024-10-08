#ifndef _MYUSART_H_
#define _MYUSART_H_
#include <ch32v00x.h>

extern u8 RxCount;
extern u8 RxBuffer[];
extern void USARTx_CFG(void);
extern char getRx();
extern void UART_write(const char c);
extern void UART_WaitReady(void);
extern void UartBufferSend(const char *buffer);
#endif