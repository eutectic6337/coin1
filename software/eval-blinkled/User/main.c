#include "debug.h"


void GPIO_Toggle_INIT(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {
    		.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2,
			.GPIO_Mode = GPIO_Mode_Out_PP,
			.GPIO_Speed = GPIO_Speed_30MHz
    };
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
	Delay_Init();
	Delay_Ms(5000);
	SDI_Printf_Enable();
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("blinkLED TEST\r\n");

	GPIO_Toggle_INIT();

	int fLED1 = 0;
    int fLED2 = 0;

    while(1)
    {
    	printf(".\n");
        Delay_Ms(100);

        GPIO_WriteBit(GPIOD, GPIO_Pin_3, fLED1 = !fLED1);
        if (fLED1) {
			Delay_Ms(100);

			GPIO_WriteBit(GPIOD, GPIO_Pin_2, fLED2 = !fLED2);
        }
    }
}
