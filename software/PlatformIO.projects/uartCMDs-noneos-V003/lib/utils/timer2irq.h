#include <ch32v00x.h>
#include <stdbool.h>

extern bool _isTimer2;
extern uint32_t _tmr2millis;
/**
 * The function initializes the TIM2 timer with the specified period and prescaler, enables the TIM2
 * update interrupt, configures the NVIC interrupt settings, and enables the TIM2 timer.
 * 
 * @param arr The "arr" parameter is the value that determines the auto-reload register (ARR) value of
 * the timer. It specifies the maximum value that the timer counter will reach before generating an
 * update event.
 * @param psc The "psc" parameter stands for Prescaler value. It is used to divide the timer clock
 * frequency before it is fed into the timer counter. This division helps in achieving the desired
 * timer resolution.
 */
void TIM2_INT_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIMBase_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIMBase_InitStruct.TIM_Period = arr;
    TIMBase_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIMBase_InitStruct.TIM_Prescaler = psc;
    TIMBase_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIMBase_InitStruct);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * The function TIM2_IRQHandler handles the interrupt for Timer 2 and prints "In" when the update
 * interrupt flag is set.
 */
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        // this can be replaced with your code of flag so that in main's that flag can be handled 
        _isTimer2 = true;
        _tmr2millis++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 
    }
}

/*
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USARTx_CFG();
    TIM2_INT_Init(1000-1,  48000-1);
    while (1);
*/