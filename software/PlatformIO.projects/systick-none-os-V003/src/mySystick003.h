#ifndef _MYSYSTICK003_H
#define _MYSYSTICK003_H

#include <ch32v00x.h>
 
void SysTick_Handler(void) __attribute__((interrupt));   //中断使用软件压栈的方式, we don't need stack push/pop
void SysTick_Handler(void) {   //定时器中断服务函数
    SysTick->SR &= ~(1 << 0);   //清除标志位
}

void SysTick_init(void){
    NVIC_EnableIRQ(SysTicK_IRQn);//使能系统的SysTick快速中断
    SysTick->SR &= ~(1 << 0);    //计数器状态清零
    SysTick->CMP = SystemCoreClock/1000;      //48MHz，定时1ms的值 
    SysTick->CNT = 0;            //计数器清零
    SysTick->CTLR = 0xF;         //设置为系统时钟、自动重载、启用中断、开启定时器
}

static uint32_t tms=0;
uint32_t millis() {
   //return SysTick->CNT;
    return tms++;
}

#endif