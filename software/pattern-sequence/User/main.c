#include "debug.h"

struct pattern {
	int bright[10];// percentage of full
	int time;// milliseconds
};

struct pattern chase[] = {
	{{100,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{100,100,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,100,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,100,100,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,100,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,100,100,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,100,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,100,100,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,100,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,100,100,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,100,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,100,100,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,100,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,100,100,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,100,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,100,100,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,100,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,100,100}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,100}, 100},
	{{100,  0,  0,  0,  0,  0,  0,  0,  0,100}, 100},
};

struct pattern sweep[] = {
};

struct pattern heartbeat[] = {
};



void adjust_LED(int n, int target) {
	switch(n) {
	case 1:
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, target);
		break;
	case 2:
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, target);
		break;
	case 3:
		GPIO_WriteBit(GPIOD, GPIO_Pin_0, target);
		break;
	case 4:
		GPIO_WriteBit(GPIOD, GPIO_Pin_6, target);
		break;
	case 5:
		GPIO_WriteBit(GPIOD, GPIO_Pin_5, target);
		break;
	case 6:
		GPIO_WriteBit(GPIOD, GPIO_Pin_3, target);
		break;
	case 7:
		GPIO_WriteBit(GPIOD, GPIO_Pin_2, target);
		break;
	case 8:
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, target);
		break;
	case 9:
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, target);
		break;
	case 10:
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, target);
		break;
	}
}

void LED_on(int n) {
	if (n < 1 || n > 10) return;
	adjust_LED(n, 1);
}
void LED_off(int n) {
	if (n < 1 || n > 10) return;
	adjust_LED(n, 0);
}

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

    while(1)
    {
    	run_sequence();
    }
}
