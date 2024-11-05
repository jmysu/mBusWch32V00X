/*
 * PWM output for mBusWch32V
 * 
 * Port:    PC4
 * Timer:   TIM1
 * OC:      Channel4
 *
 */

#include "debug.h"

/* PWM Output Mode Definition */
#define PWM_MODE1   0
#define PWM_MODE2   1

/* PWM Output Mode Selection */
#define PWM_MODE PWM_MODE1
//#define PWM_MODE PWM_MODE2

/*********************************************************************
 * @fn      TIM1_OutCompare_Init
 *
 * @brief   Initializes TIM1 output compare.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 */
void TIM1CH4_PWMOut_Init(u16 arr, u16 psc, u16 ccp) {
    TIM_OCInitTypeDef TIM_OCInitStructure = { 0 };
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = { 0 };

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

#if (PWM_MODE == PWM_MODE1)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
#elif (PWM_MODE == PWM_MODE2)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
#endif

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    
    //TIM_OC1Init( TIM1, &TIM_OCInitStructure);
    TIM_OC4Init( TIM1, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    //TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable);
    TIM_OC4PreloadConfig( TIM1, TIM_OCPreload_Disable);   
    TIM_ARRPreloadConfig( TIM1, ENABLE);
    TIM_Cmd( TIM1, ENABLE);
}

void initC3C4D0(void) {

    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM1, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_TIM1, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init( GPIOD, &GPIO_InitStructure);
}

void notone(void) {
    TIM_Cmd(TIM1, DISABLE);
}

void tone(uint16_t freq) {
    uint32_t duration;
    if (freq>0) {
        notone();
        duration = 1000000 / freq;  
        TIM1CH4_PWMOut_Init( duration, 48, duration>>1);
        }
    else
        notone();
}

void toneUs(uint16_t freq, uint32_t us) {
    uint32_t duration;
    if (freq>0) {
        notone();
        duration = 1000000 / freq;   
        TIM1CH4_PWMOut_Init(duration, 48, duration >>1);
        }
    else
        notone();
    Delay_Us(us);
    notone();
}
void toneMs(uint16_t freq, uint16_t ms) {
    uint32_t duration;
    if (freq>0) {
        notone();
        duration = 1000000 / freq;   
        TIM1CH4_PWMOut_Init(duration, 48, duration >>1);
        }
    else
        notone();
    Delay_Ms(ms);
    notone();
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
/*
int main(void) {

    Delay_Init();

    toneinit();

    while(1) {

        for(int i=0; i<9; i++) {

            if(tonedata[i]!=0) {
                tone(tonedata[i]);
            } else {
                notone();
            }

            Delay_Ms(200);

        }

        notone();
        Delay_Ms(5000);

    }
}
*/