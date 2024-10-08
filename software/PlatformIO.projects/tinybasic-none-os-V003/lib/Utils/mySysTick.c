/*
 *  mySysTick.c
 *
 * 
 *      CH32V003 SysTick Utility
 * 
 *      by jimmy.su 
 */
#include <ch32v00x.h>
#include "mySysTick.h"
 
uint32_t _systick_counter = 0;
//void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void) __attribute__((interrupt));
/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief   Interrupt service routine on SysTick event. Increments Systick count.
 *
 * @return  None
 */
void SysTick_Handler(void)
{
    SysTick->SR = 0;        // clear status register
    _systick_counter++;     // counter increment

    //SYSTICK_MILLIS
	if ((_systick_counter%500)==0) { //500ms?
	    GPIO_WriteBit(GPIOC, GPIO_Pin_0, !GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_0)); // toggle LED
		//_systick_counter++; // prevent re-toggling
		}
}

/*********************************************************************
 * @fn      systick_get
 *
 * @brief   Return the current Systick counter.
 *
 * @return  Current Systick counter as 64-Bit unsigned integer
 */
uint32_t systick_get(void)
{
    return _systick_counter;
}

/*********************************************************************
 * @fn      systick_init
 *
 * @brief   Initialize Systick time keeping. Select timebase for tracking. 
 * 
 * @param   iPrecision     SYSTICK_SECONDS, SYSTICK_MILLIS, SYSTICK_MICROS 
 * 
 * 
 * @return  None
 */
void systick_init(uint32_t iPrecision)
{
    SysTick->SR = 0;
    SysTick->CNT = 0;
    switch (iPrecision)
    {
    case SYSTICK_SECONDS:
        SysTick->CMP = (SystemCoreClock/1)-1;
        break;
    case SYSTICK_MILLIS:
        SysTick->CMP = (SystemCoreClock/1000)-1;
        break;
    case SYSTICK_MICROS:
        SysTick->CMP = (SystemCoreClock/1000000)-1;
        break;
    default:
        SysTick->CMP = (SystemCoreClock/iPrecision)-1;   // Default case: use whatever arbitrary non-negative, non-zero number is supplied
        break;
    }
    SysTick->CTLR = 0xF;
    
    #if defined(CH32X035) || defined(CH32X033)
     NVIC_SetPriority(SysTicK_IRQn, 15);
    #else
     NVIC_SetPriority(SysTicK_IRQn, 1);
    #endif
    NVIC_EnableIRQ(SysTicK_IRQn);
}
