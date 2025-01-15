#include "debug.h"

//this is an affront to programming - never write code like this
#define LED1	GPIOC, GPIO_Pin_3
#define LED2	GPIOC, GPIO_Pin_2
#define LED3	GPIOD, GPIO_Pin_0
#define LED4	GPIOD, GPIO_Pin_6
#define LED5	GPIOD, GPIO_Pin_5
#define LED6	GPIOD, GPIO_Pin_3
#define LED7	GPIOD, GPIO_Pin_2
#define LED8	GPIOC, GPIO_Pin_7
#define LED9	GPIOC, GPIO_Pin_6
#define LED10	GPIOC, GPIO_Pin_5


void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
	Delay_Init();

	GPIO_Toggle_INIT();

    GPIO_WriteBit(LED1, 1);
    while(1)
    {
        Delay_Ms(100);
        GPIO_WriteBit(LED2, 1);
        Delay_Ms(100);
        GPIO_WriteBit(LED1, 0);
        Delay_Ms(100);
        GPIO_WriteBit(LED3, 1);
        Delay_Ms(100);
        GPIO_WriteBit(LED2, 0);
        Delay_Ms(100);
        GPIO_WriteBit(LED4, 1);
        Delay_Ms(100);
        GPIO_WriteBit(LED3, 0);
        Delay_Ms(100);
        GPIO_WriteBit(LED5, 1);
        Delay_Ms(100);
        GPIO_WriteBit(LED4, 0);
        Delay_Ms(100);
        GPIO_WriteBit(LED6, 1);
        Delay_Ms(100);
        GPIO_WriteBit(LED5, 0);
        Delay_Ms(100);
        GPIO_WriteBit(LED7, 1);
        Delay_Ms(100);
        GPIO_WriteBit(LED6, 0);
        Delay_Ms(100);
        GPIO_WriteBit(LED8, 1);
        Delay_Ms(100);
        GPIO_WriteBit(LED7, 0);
        Delay_Ms(100);
        GPIO_WriteBit(LED9, 1);
        Delay_Ms(100);
        GPIO_WriteBit(LED8, 0);
        Delay_Ms(100);
        GPIO_WriteBit(LED10, 1);
        Delay_Ms(100);
        GPIO_WriteBit(LED9, 0);
        Delay_Ms(100);
        GPIO_WriteBit(LED1, 1);
        Delay_Ms(100);
        GPIO_WriteBit(LED10, 0);
    }
}
