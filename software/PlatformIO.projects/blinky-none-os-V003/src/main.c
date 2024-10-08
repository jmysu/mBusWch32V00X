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

#define BLINKY_GPIO_PORT GPIOC
#define BLINKY_GPIO_PIN GPIO_Pin_0
#define BLINKY_CLOCK_ENABLE RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void Delay_Init(void);
void Delay_Ms(uint32_t n);

int main(void)
{
	u8 i = 0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
	Delay_Init();
	
#if (SDI_PRINT == SDI_PR_OPEN)
	SDI_Printf_Enable();
#else
	USART_Printf_Init(115200);
#endif
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	BLINKY_CLOCK_ENABLE;
	GPIO_InitStructure.GPIO_Pin = BLINKY_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BLINKY_GPIO_PORT, &GPIO_InitStructure);

	printf("SystemClk:%lu\r\n", SystemCoreClock);
	
	NVIC_EnableIRQ(SysTicK_IRQn);
	/*
   SysTick->SR &= ~(1 << 0); 				// clear counter status
   SysTick->CMP = SystemCoreClock-1; 	// compare value
   SysTick->CNT = 0;							// counter reset
   SysTick->CTLR = 0xF; 					// STK_STRE | STK_STCLK | STK_STIE | STK_STE;
 */

#if defined(CH32V30X)
    printf("ChipID: %08x\r\n", (unsigned)DBGMCU_GetCHIPID());
#else
    printf("DeviceID: %08x\r\n", (unsigned)DBGMCU_GetDEVID());
#endif
	uint8_t ledState = 0;
	while (1)
	{
		GPIO_WriteBit(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN, ledState);
		ledState ^= 1; // invert for the next run
		Delay_Ms(1000);
		printf("-DEBUG-PR-%08x\r\n", i++);

	}
}

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief   SysTick Interrupt Service Function.
 *
 * @return  none
 */
void SysTick_Handler(void)
{
    printf("Systick\r\n");
    SysTick->SR = 0;	//Clear status
}

void NMI_Handler(void) {}
void HardFault_Handler(void)
{
	while (1)
	{
	}
}
