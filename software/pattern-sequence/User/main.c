#include "debug.h"

#define NUM_ARRAY_ELEMS(a) ((sizeof a)/(sizeof a[0]))

#define NUM_LEDS 10
struct pattern {
	int bright[NUM_LEDS];// percentage of full
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
	if (n < 1 || n > NUM_LEDS) return;
	adjust_LED(n, 1);
}
void LED_off(int n) {
	if (n < 1 || n > NUM_LEDS) return;
	adjust_LED(n, 0);
}


int get_current_time_ms(void)
{
	//FIXME
	return 0;
}


void setup_LEDs(void)
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

    //FIXME: PWM
}
void loop_LEDs(void)
{
	//FIXME: PWM
}
void set_LED_brightness(int led, int bright)
{
	//FIXME: PWM
}


struct pattern *sequence_start;
int sequence_length;

int sequence_step;
int next_step_ms;

void setup_sequence(void)
{
	sequence_start = chase;
	sequence_length = NUM_ARRAY_ELEMS(chase);
}
void loop_sequence(void)
{
	if (!sequence_start) return;

	int current_time = get_current_time_ms();
	if (current_time >= next_step_ms) {
		if (sequence_length > sequence_step) {
			sequence_step++;
		}
		else {
			sequence_step = 0;
		}

		struct pattern *s = sequence_start+sequence_step;
		next_step_ms = current_time + s->time;

		for (int i = 0; i < NUM_LEDS; i++) {
			set_LED_brightness(i, s->bright[i]);
		}
	}
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
	Delay_Init();

	setup_LEDs();
	setup_sequence();
    while(1)
    {
    	loop_sequence();
    	loop_LEDs();
    }
}
