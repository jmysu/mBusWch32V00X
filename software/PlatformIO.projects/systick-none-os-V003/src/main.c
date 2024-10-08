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
//#include "mySystick003.h"

#define BLINKY_GPIO_PORT GPIOC
#define BLINKY_GPIO_PIN GPIO_Pin_0
#define BLINKY_CLOCK_ENABLE RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
	Delay_Init();
	//SysTick_init();
 

#if (SDI_PRINT == SDI_PR_OPEN)
	SDI_Printf_Enable();
#else
	USART_Printf_Init(115200); //TX only
#endif
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	BLINKY_CLOCK_ENABLE;
	GPIO_InitStructure.GPIO_Pin = BLINKY_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BLINKY_GPIO_PORT, &GPIO_InitStructure);

	printf("SystemClk:%ld\r\n", SystemCoreClock);
#if defined(CH32V30X)
    printf("ChipID: %08x\r\n", (unsigned)DBGMCU_GetCHIPID());
#else
    printf("DeviceID: %08x\r\n", (unsigned)DBGMCU_GetDEVID());
#endif

	uint32_t tMillis=0, tSec=0;
	while (1) {
  		    Delay_Ms(1);
            tMillis++;
            if (tMillis>999) {
                tMillis=0;
                //toggle LED
			    GPIO_WriteBit(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN, \
                              !GPIO_ReadOutputDataBit(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN));
                printf("sec: %08lu\r\n", tSec++);
			    }
		}
}

void NMI_Handler(void) {}
void HardFault_Handler(void)
{
	while (1)
	{
	}
}
