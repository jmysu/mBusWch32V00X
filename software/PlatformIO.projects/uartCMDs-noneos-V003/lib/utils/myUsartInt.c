#include <ch32v00x.h>
#include "myUsartInt.h"

u8 _RxBuffer[MAX_RX_COUNT] = {0};
u8 _RxCount = 0;
//void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void) __attribute__((interrupt));
/*********************************************************************
 * @fn      USART1_IRQHandler
 *
 * @brief   This function handles USART1 global interrupt request.
 *
 * @return  none
 */
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        if (_RxCount < (MAX_RX_COUNT-1) ) {
            _RxBuffer[_RxCount++] = USART_ReceiveData(USART1);
            _RxBuffer[_RxCount]=0; //terminator
            }
        }
}

/*********************************************************************
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART1 peripheral.
 *
 * @return  none
 */
void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);
#if UART_TXD5_RXD6
    /* USART1 TX-->D.5   RX-->D.6 Normal*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#else //UART_TXD6_RXD5 ReMap
    GPIO_PinRemapConfig(GPIO_PartialRemap2_USART1, ENABLE);
    /* USART1 RX-->D.5   TX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);;

    USART_Cmd(USART1, ENABLE);
}

char getRx() {
   if (_RxCount) {
        _RxCount=0;          // string terminator
        return _RxBuffer[0]; // return first char received 
        }
   else
        return 0;
}

void UART_WaitReady(void) {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);/* waiting for sending finish */
}


void UART_write(const char c) {
    //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); /* waiting for sending finish */
    UART_WaitReady();
    USART_SendData(USART1, c);
}

void UartBufferSend(const char *buffer) {
    while (*buffer) {
        //while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);/* waiting for sending finish */
        UART_WaitReady();
        USART_SendData(USART1, *(buffer++));
        }
}

