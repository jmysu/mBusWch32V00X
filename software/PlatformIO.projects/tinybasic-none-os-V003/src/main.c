#if defined(CH32V00X)
#include <ch32v00x.h>
#elif defined(CH32V10X)
#include <ch32v10x.h>
#elif defined(CH32V20X)
#include <ch32v20x.h>
#elif defined(CH32V30X)
#include <ch32v30x.h>
#endif

#include <debug.h>
#include "mySysTick.h"
#include "myUsartInt.h"
#include "ansiTitle.h"
#include "ttbasic.h"
#include "v00xflash.h"

//#define _GPIO_PORT GPIOC
//#define _GPIO_PIN GPIO_Pin_0
//#define BLINKY_CLOCK_ENABLE RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
	Delay_Init();
/*
#if (SDI_PRINT == SDI_PR_OPEN)
	SDI_Printf_Enable();
#else
	USART_Printf_Init(115200); //TX only
#endif
*/
   systick_init(SYSTICK_MILLIS);
	USARTx_CFG(); //config USART1 tx/rx

	//Init C0
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	//printf("SystemClk:%ld\r\n", SystemCoreClock);
	//char buf[32];
	//sprintf(buf, "\nClk:%luMHz", SystemCoreClock/1000000);
	//sprintf(buf, "Flash:%lukB\r\n", (*(uint32_t *)0x1FFFF7E0)&0x00FF);
	//UartBufferSend(buf);
/*
#if defined(CH32V30X)
    printf("ChipID: %08x\r\n", (unsigned)DBGMCU_GetCHIPID());
#else
	 sprintf(buf, "Ch32v%08x\n\n", (uint32_t)DBGMCU_GetCHIPID());
	 UartBufferSend(buf);
	 sprintf(buf, "   ID:%08lx\n\n", (uint32_t)DBGMCU_GetDEVID());
	 UartBufferSend(buf);
#endif
*/
	 //ansiTitle("uartCh32V003");
	 char cTitle[16];
	 //					 0123456789ABCDEF
	 sprintf( cTitle, "uartV%03lu(%2lukB)", (uint32_t)DBGMCU_GetCHIPID()>>20, (uint32_t)getChipFlashSize()/1024); //chipid right shift 20bit
	 ansiTitle( cTitle);

while (1) {
	basic();
}
/*
	uint32_t tMillis=0;
	//uint32_t tSec=0;
	while (1) {
		if (RxCount>0) {//uart available
			printf("RX[%d]:", RxCount);
			for (int i=0;i<RxCount;i++) printf("[%02X]", RxBuffer[i]);
			printf("\r\n");
			RxCount=0;
			}
      tMillis++;
      if (tMillis>999) {
         tMillis=0;
         //toggle LED
		 	GPIO_WriteBit(_GPIO_PORT, _GPIO_PIN, !GPIO_ReadOutputDataBit(_GPIO_PORT, _GPIO_PIN));
         //printf("sec: %08lu\r\n", tSec++);
		}
		Delay_Ms(1);
		}
*/
}

void NMI_Handler(void) {}
void HardFault_Handler(void)
{
	while (1)
	{
	}
}
