/*
 * main C program for project uartCMDs-noneos-v003
 *
 * 
 * 	CH32V003 uart commander
 * 
 *
 * 	Buzzer: 		C4 (T1C4 PWM)
 * 	Neopixel:	C3
 * 	LED:			D0
 *  	SWIO:			D1
 * 	uartTx:		D5
 * 	uartRx:		D6
 * 
 *    ----------------------------------------------------------------
 * 	RAM:   [=====     ]  46.1% (used 944 bytes from 2048 bytes)
 * 	Flash: [=======   ]  65.2% (used 10688 bytes from 16384 bytes)
 *    ----------------------------------------------------------------
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
 
#include <ctype.h>
#include <debug.h>
 
#include "timer2irq.h"
#include "myUsartInt.h"
#include "neo_sw.h"
#include "ansiColors.h"
#include "myCmds.h" 
 

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void Delay_Init(void);
//void Delay_Ms(uint32_t n);

void tone(uint16_t freq);
void notone(void);
void initC3C4(void);
void TIM1CH4_PWMOut_Init(u16 arr, u16 psc, u16 ccp);

//extern  void mouse(void);
extern bool isTimer2;
extern void ansiTitle(const char *title);

//6805-EC15 R/G/B mcd : 2/4/1 
uint8_t neo_buf[6][3]={{8,0,0}, {0,4,0}, {0,0,16}, {0,2,8}, {4,0,8}, {4,2,0} };
uint8_t neo_c=0;


uint8_t cmd[16];
uint8_t cmdCount=0;
#define kbhit() _RxCount
//char cmdBuf[MAX_RX_COUNT];
char cPtr=0;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
	Delay_Init();

	USARTx_CFG(); //config USART1 tx/rx
   TIM2_INT_Init(1000-1,  48000-1); // (48000-1)/48,000,000 * (1000-1) = 1 second. 
 
 	//GPIO PortD & Pin0
	GPIO_InitTypeDef GPIO_InitStructure = {0};
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
   GPIO_Init(GPIOD, &GPIO_InitStructure);
/*
#if (SDI_PRINT == SDI_PR_OPEN)
	SDI_Printf_Enable();
#else
	USART_InitTxRx(115200);
#endif
*/
	printf("SystemClk:%lu\r\n", SystemCoreClock);
#if defined(CH32V30X)
    printf("ChipID: %08x\r\n", (unsigned)DBGMCU_GetCHIPID());
#else
    printf("DeviceID: %08x\r\n", (unsigned)DBGMCU_GetDEVID());
#endif

ansiTitle("V0003 uartCMDs");
 
initC3C4(); 
NEO_init();

cmdHelp();
cmdPrompt();
	while (1) {
		////tone(100 + ((i%255)<<2) );

		if (isTimer2) {
			//printf("Timer2\n");
			isTimer2=0;
			//Update Neopixels
			NEO_writeColor(0, neo_buf[neo_c][0], neo_buf[neo_c][1], neo_buf[neo_c][2]); 
			neo_c++; 
			if (neo_c>5) neo_c=0;
			NEO_update();
			//toggle D0
			GPIO_WriteBit(GPIOD, GPIO_Pin_0, (neo_c%2)?Bit_SET:Bit_RESET);
			}
   
		char c=getRx();  // get incoming character
		if((c=='\r')) {	
			//printf("\nCMD>%s", cmd);
			strcpy((char*)cmdBuf, (const char*)cmd);
			char *myCmd = cmdParam(0);
			if (*myCmd) {
			   cmdMatch(myCmd);  //Matching cmd
            cmdPrompt();
				}
			for (int i=0;i<cmdCount;i++) cmd[i]=0; 	//clear All cmd buffer
			cmdCount=0; 	//reset cmd buffer
			}
		else if (isprint(c)){
			cmd[cmdCount++]=c; 	//Add c to cmd buffer
 			printf("%c", c); 		//Echo it
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
