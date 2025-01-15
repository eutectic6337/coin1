#include "debug.h"


void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
	Delay_Init();

	GPIO_Toggle_INIT();

	int fLED1 = 0;
    int fLED2 = 0;

    while(1)
    {
        Delay_Ms(100);

        GPIO_WriteBit(GPIOC, GPIO_Pin_3, fLED1 = !fLED1);
        if (fLED1) {
			Delay_Ms(100);

			GPIO_WriteBit(GPIOC, GPIO_Pin_2, fLED2 = !fLED2);
        }
    }
}
