/*
 * 10-swdio
 */
#include "debug.h"

#define NUM_ARRAY_ELEMS(a) ((sizeof a)/(sizeof a[0]))

#define NUM_LEDS 16

#define REMAP 00
#include "CH32V003F4U6.h"
void pins_CFG(void)
{
#if 0
    GPIO_InitTypeDef GPIO_InitStructure = {
    	.GPIO_Pin = GPIO_Pin_x,
    	.GPIO_Mode = GPIO_Mode_Out_PP,
		.GPIO_Speed = GPIO_Speed_30MHz
    };
    GPIO_Init(GPIOx, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOx, GPIO_Pin_x, value);
#endif
}
void set_pin(unsigned pinID)
{
}
void clr_pin(unsigned pinID)
{
}

// 8 pin-pairs, mapping per PCB
#define G1 PC5
#define P1 TIM1_CH3
#define G2 PC2
#define P2 TIM2_CH3
#define G3 PC1
#define P3 TIM1_CH2
#define G4 PD0
#define P4 TIM2_CH4
#define G5 PD5
#define P5 TIM2_CH1
#define G6 SWIO
#define P6 TIM2_CH2
#define G7 PC7
#define P7 TIM1_CH1
#define G8 PC6
#define P8 TIM1_CH4

void set_LED_dutycycle(int led, int duty)
{
	if (duty < 1 || duty > 99) { // full off, or full on
		int value = duty > 99;

	    switch(led) {
		case 1:
			if (value) set_pin(P1);
			else clr_pin(P1);
			clr_pin(G1);
			break;
		case 2:
			if (value) set_pin(G1);
			else clr_pin(G1);
			clr_pin(P1);
			break;
		case 3:
			if (value) set_pin(P2);
			else clr_pin(P2);
			clr_pin(G2);
			break;
		case 4:
			if (value) set_pin(G2);
			else clr_pin(G2);
			clr_pin(P2);
			break;
		case 5:
			if (value) set_pin(P3);
			else clr_pin(P3);
			clr_pin(G3);
			break;
		case 6:
			if (value) set_pin(G3);
			else clr_pin(G3);
			clr_pin(P3);
			break;
		case 7:
			if (value) set_pin(P4);
			else clr_pin(P4);
			clr_pin(G4);
			break;
		case 8:
			if (value) set_pin(G4);
			else clr_pin(G4);
			clr_pin(P4);
			break;
		case 9:
			if (value) set_pin(P5);
			else clr_pin(P5);
			clr_pin(G5);
			break;
		case 10:
			if (value) set_pin(G5);
			else clr_pin(G5);
			clr_pin(P5);
			break;
		case 11:
			if (value) set_pin(P6);
			else clr_pin(P6);
			clr_pin(G6);
			break;
		case 12:
			if (value) set_pin(G6);
			else clr_pin(G6);
			clr_pin(P6);
			break;
		case 13:
			if (value) set_pin(P7);
			else clr_pin(P7);
			clr_pin(G7);
			break;
		case 14:
			if (value) set_pin(G7);
			else clr_pin(G7);
			clr_pin(P7);
			break;
		case 15:
			if (value) set_pin(P8);
			else clr_pin(P8);
			clr_pin(G8);
			break;
		case 16:
			if (value) set_pin(G8);
			else clr_pin(G8);
			clr_pin(P8);
			break;
		}
		return;
	}
}

void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    SDI_Printf_Enable();
    printf("SystemClk:%ld\r\n",(long)SystemCoreClock);
    printf( "ChipID:%08lx\r\n", (long)DBGMCU_GetCHIPID() );

    pins_CFG();

    while(1)
    {

    }
}
