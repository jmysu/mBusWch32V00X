#ifndef _MYUSART_H_
#define _MYUSART_H_
#include <ch32v00x.h>

//USART1 NORMAL
#define UART_TXD5_RXD6 1
//USART1 REMAP 
#define UART_TXD6_RXD5 0
#define MAX_RX_COUNT 16

extern u8 _RxCount;
extern u8 _RxBuffer[];
extern void USARTx_CFG(void);
extern char getRx();
extern void UART_write(const char c);
extern void UART_WaitReady(void);
extern void UartBufferSend(const char *buffer);
#endif