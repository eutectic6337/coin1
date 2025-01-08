#include "debug.h"


void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
	Delay_Init();

	GPIO_Toggle_INIT();

	int LED1 = 0;
    int LED2 = 0;

    while(1)
    {
        Delay_Ms(250);

        GPIO_WriteBit(GPIOC, GPIO_Pin_3, LED1 = !LED1);
        if (LED1) {
        	GPIO_WriteBit(GPIOD, GPIO_Pin_1, LED2 = !LED2);
        }
    }
}
