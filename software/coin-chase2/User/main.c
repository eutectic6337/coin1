/*
 * 10-swdio
 */
//#define NDEBUG
#define ONOFF_ONLY
#include "debug.h"

#define NUM_ARRAY_ELEMS(a) ((sizeof a)/(sizeof a[0]))
#define NUM_LEDS 16
struct pattern {
	int bright[NUM_LEDS];// percentage of full
	int time;// milliseconds
};
const struct pattern chase[] = {
	{{100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},//[0]
	{{100,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,100,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,100,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,100,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,100,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,100,100,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,100,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,100,100,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,100,  0,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,100,100,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,100,  0,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,100,100,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,100,  0,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,100,100,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,  0,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,100,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,  0,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,100,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,  0,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,100,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,  0,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,100,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,  0}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,100}, 100},
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100}, 100},
	{{100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100}, 100},//[31]
};
#define SELECTED_SEQUENCE chase

const struct pattern *sequence_start;
int sequence_length;

int sequence_step;
int next_step_ms;


#define REMAP 00
#include "CH32V003F4U6.h"
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
// actual I/O pins used:
// A 1
// C 0,1,2,3,4,5,6,7
// D 0,1,2,3,4,5,7

void set_LED_dutycycle(int led, int duty)
{
#ifdef ONOFF_ONLY
	if (duty >= 50) duty = 100;
	if (duty < 50) duty = 0;
#endif

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

int get_current_time_ms(void)
{
	//FIXME
	return 0;
}

void setup(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();

#ifndef NDEBUG
    Delay_Init();
    SDI_Printf_Enable();
    printf("SystemClk:%ld\r\n",(long)SystemCoreClock);
    printf("ChipID:%08lx\r\n", (long)DBGMCU_GetCHIPID() );
#endif

    RCC_APB2PeriphClockCmd(
    		RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
			RCC_APB2Periph_TIM1 | RCC_APB1Periph_TIM2,
    		ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =
    		GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
    		GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
    		GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =
    		GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
    		GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
    		GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

#ifndef ONOFF_ONLY
	int arr = 100;
	unsigned psc = 480-1;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
#endif

	sequence_start = SELECTED_SEQUENCE;
	sequence_length = NUM_ARRAY_ELEMS(SELECTED_SEQUENCE);
}
void loop(void)
{
	if (!sequence_start) return;

	int current_time = get_current_time_ms();
	if (current_time >= next_step_ms) {
		if (sequence_step < sequence_length-1) {
			sequence_step++;
		}
		else {
			sequence_step = 0;
		}

		const struct pattern *s = sequence_start+sequence_step;
		next_step_ms = current_time + s->time;

		for (int i = 0; i < NUM_LEDS; i++) {
			set_LED_dutycycle(i, s->bright[i]);
		}
	}
}

int main(void)
{
	setup();
    while(1)
    {
    	loop();
    }
}
