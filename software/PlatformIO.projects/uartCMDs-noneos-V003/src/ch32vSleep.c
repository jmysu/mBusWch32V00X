/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32vSleep.c
 * Author             : WCH / jimmy.su
 * Version            : V1.1
 * Date               : 2024/10/25
 * Description        : Ch32V002 sleep.
*********************************************************************************/

/*
 *@Note
 low power, sleep mode routine:
 EXTI_Line0(PD0)
 This routine demonstrates that WFI enters the sleep mode, and the PD0 pin input
 low level triggers the external interrupt EXTI_Line0 to exit the sleep mode,
 Program execution continues after wake-up.

*/
#if defined(CH32V00X)
#include <ch32v00x.h>
#elif defined(CH32V10X)
#include <ch32v10x.h>
#elif defined(CH32V20X)
#include <ch32v20x.h>
#elif defined(CH32V30X)
#include <ch32v30x.h>
#endif

#include "debug.h"
#include "neo_sw.h"

#include "_main.h"

/* Global define */
extern void cmdReset(void);

/* Global Variable */

/*********************************************************************
 * @fn      EXTI0_INT_INIT
 *
 * @brief   Initializes EXTI0 collection.
 *
 * @return  none
 */ /*
void EXTI0_INT_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //GPIOD.0 ----> EXTI_Line0 
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
*/
#ifdef _ENABLE_SLEEP
void EXTI_D6RX_INT_INIT(void)
{
    //GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD, ENABLE);

    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* GPIOD.6 ----> EXTI_Line0 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource6);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
void mainSleep(void)
{
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    //Delay_Init();
    Delay_Ms(100);
    //USART_Printf_Init(115200);
    //printf("\nSystemClk:%ld\r\n", SystemCoreClock);

    //printf("Sleep Mode Test\r\n");
    //EXTI0_INT_INIT();
    EXTI_D6RX_INT_INIT();

    printf("\r\nzzzzzzZZZZZZZZ");
    #ifdef PIN_NEO
    #ifdef _ENABLE_NEO
      NEO_clearAll();
      NEO_update();
    #endif
    #endif

    __WFI();

    Delay_Ms(50); //debounce
    printf("\r\n### WakeUp ###\r\n");

    Delay_Ms(1000);
    printf("ReBooting...");
    Delay_Ms(1000);
    cmdReset();
}


void EXTI7_0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      EXTI0_IRQHandler
 *
 * @brief   This function handles EXTI0 Handler.
 *
 * @return  none
 */
void EXTI7_0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
  {
    printf("EXTI0 Wake_up\r\n");
    EXTI_ClearITPendingBit(EXTI_Line0);     /* Clear Flag */
  }
}

#endif