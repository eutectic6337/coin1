#include "debug.h"

#define NUM_ARRAY_ELEMS(a) ((sizeof a)/(sizeof a[0]))

#define NUM_LEDS 10
struct pattern {
	int bright[NUM_LEDS];// percentage of full
	int time;// milliseconds
};

// chip has 2 Timer channels with 4 Output Compare channels each
// that can drive PWM outputs,
// so we can have at most 8 LEDs at >0 <100 brightness
// but our specific pin choices for LEDs mean we have only
// 5 simultaneous PWMs available

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



/* PWM Output Mode Definition */
#define PWM_MODE1   0
#define PWM_MODE2   1

/* PWM Output Mode Selection */
//#define PWM_MODE PWM_MODE1
#define PWM_MODE PWM_MODE2

struct pwm_in_use {
	unsigned gp:1;
	unsigned tim:2;
	unsigned oc:3;
} LED_pwm[NUM_LEDS];

void set_LED_dutycycle(int led, int duty)
{
	// first call after all have been set, clear the flags
	if (LED_pwm[NUM_LEDS-1].gp || LED_pwm[NUM_LEDS-1].tim) {
		for (int i=0; i<NUM_LEDS; i++) LED_pwm[i] = (struct pwm_in_use){0};
	}

	if (duty < 1 || duty > 99) { // full off, or full on
		int value = duty > 99;

		LED_pwm[led-1] = (struct pwm_in_use){.gp=1, .tim=0, .oc=0};

		GPIO_TypeDef *GPIOx;
	    uint16_t GPIO_Pin_x;

	    switch(led) {
		case 1:	GPIOx = GPIOC;
		    GPIO_Pin_x = GPIO_Pin_3;
			break;
		case 2:	GPIOx = GPIOC;
		    GPIO_Pin_x = GPIO_Pin_2;
			break;
		case 3:	GPIOx = GPIOD;
		    GPIO_Pin_x = GPIO_Pin_0;
			break;
		case 4:	GPIOx = GPIOD;
		    GPIO_Pin_x = GPIO_Pin_6;
			break;
		case 5:	GPIOx = GPIOD;
		    GPIO_Pin_x = GPIO_Pin_5;
			break;
		case 6: GPIOx = GPIOD;
		    GPIO_Pin_x = GPIO_Pin_3;
			break;
		case 7:	GPIOx = GPIOD;
		    GPIO_Pin_x = GPIO_Pin_2;
			break;
		case 8:	GPIOx = GPIOC;
		    GPIO_Pin_x = GPIO_Pin_7;
			break;
		case 9:	GPIOx = GPIOC;
		    GPIO_Pin_x = GPIO_Pin_6;
			break;
		case 10: GPIOx = GPIOC;
		    GPIO_Pin_x = GPIO_Pin_5;
			break;
		}
	    GPIO_InitTypeDef GPIO_InitStructure = {
	    	.GPIO_Pin = GPIO_Pin_x,
	    	.GPIO_Mode = GPIO_Mode_Out_PP,
			.GPIO_Speed = GPIO_Speed_30MHz
	    };
	    GPIO_Init(GPIOx, &GPIO_InitStructure);
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, value);
		return;
	}

	TIM_OCInitTypeDef TIM_OCInitStructure = {
		.TIM_OCMode =
#if (PWM_MODE == PWM_MODE1)
				TIM_OCMode_PWM1
#elif (PWM_MODE == PWM_MODE2)
				TIM_OCMode_PWM2
#endif
				,
		.TIM_OutputState = TIM_OutputState_Enable,
		.TIM_Pulse = duty,
		.TIM_OCPolarity = TIM_OCPolarity_High
	};

	GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin_x;
    TIM_TypeDef *TIMx;
    void (*OCxInit)(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct);
    void (*OCxPreloadConfig)(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload);
	switch(led) {
	case 1: GPIOx = GPIOC;
	    GPIO_Pin_x = GPIO_Pin_3;
		//T1CH3
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=1, .oc=3};
		TIMx = TIM1;
		OCxInit = TIM_OC3Init;
		OCxPreloadConfig = TIM_OC3PreloadConfig;
		break;
	case 2: GPIOx = GPIOC;
	    GPIO_Pin_x = GPIO_Pin_2;
		// -- T2CH2_1
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=2, .oc=2};
		TIMx = TIM2;
		OCxInit = TIM_OC2Init;
		OCxPreloadConfig = TIM_OC2PreloadConfig;
		OTHER;
		break;
	case 3: GPIOx = GPIOD;
	    GPIO_Pin_x = GPIO_Pin_0;
		//T1CH1N
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=1, .oc=1};
		TIMx = TIM1;
		OCxInit = TIM_OC1Init;
		OCxPreloadConfig = TIM_OC1PreloadConfig;
		N;
		break;
	case 4: GPIOx = GPIOD;
	    GPIO_Pin_x = GPIO_Pin_6;
		// -- T2CH3_3
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=2, .oc=3};
		TIMx = TIM2;
		OCxInit = TIM_OC3Init;
		OCxPreloadConfig = TIM_OC3PreloadConfig;
		OTHER;
		break;
	case 5: GPIOx = GPIOD;
	    GPIO_Pin_x = GPIO_Pin_5;
		// -- T2CH4_3
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=2, .oc=4};
		TIMx = TIM2;
		OCxInit = TIM_OC4Init;
		OCxPreloadConfig = TIM_OC4PreloadConfig;
		OTHER;
		break;
	case 6: GPIOx = GPIOD;
	    GPIO_Pin_x = GPIO_Pin_3;
	    if (LED_pwm[2-1].tim==2 && LED_pwm[2-1].oc==2) {
	    	// punt to on/off if Timer+OC channel is already used
	    	set_LED_dutycycle(led, duty>50? 100: 0);
	    	return;
	    }
		//T2CH2
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=2, .oc=2};
		TIMx = TIM2;
		OCxInit = TIM_OC2Init;
		OCxPreloadConfig = TIM_OC2PreloadConfig;
		break;
	case 7: GPIOx = GPIOD;
	    GPIO_Pin_x = GPIO_Pin_2;
	    if (LED_pwm[3-1].tim==1 && LED_pwm[3-1].oc==1) {
	    	// punt to on/off if Timer+OC channel is already used
	    	set_LED_dutycycle(led, duty>50? 100: 0);
	    	return;
	    }
		//T1CH1
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=1, .oc=1};
		TIMx = TIM1;
		OCxInit = TIM_OC1Init;
		OCxPreloadConfig = TIM_OC1PreloadConfig;
		break;
	case 8: GPIOx = GPIOC;
	    GPIO_Pin_x = GPIO_Pin_7;
		// -- T1CH2_1 / T2CH2_3 / T1CH2_3
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=1, .oc=2};
		TIMx = TIM1;
		OCxInit = TIM_OC2Init;
		OCxPreloadConfig = TIM_OC2PreloadConfig;
		OTHER;
		break;
	case 9: GPIOx = GPIOC;
	    GPIO_Pin_x = GPIO_Pin_6;
		// -- T1CH1_1 / T1CH3N_3
	    if ((LED_pwm[2-1].tim==1 && LED_pwm[2-1].oc==1) ||
	    		(LED_pwm[7-1].tim==1 && LED_pwm[7-1].oc==1)) {
	    	//T1CH1 is already used
		    if (LED_pwm[1-1].tim==1 && LED_pwm[1-1].oc==3) {
		    	// punt to on/off if Timer+OC channel is already used
		    	set_LED_dutycycle(led, duty>50? 100: 0);
		    	return;
		    }
		    //but T1CH3 is available
			LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=1, .oc=3};
			TIMx = TIM1;
			OCxInit = TIM_OC3Init;
			OCxPreloadConfig = TIM_OC3PreloadConfig;
			OTHER;
			N;
			break;
	    }
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=1, .oc=1};
		TIMx = TIM1;
		OCxInit = TIM_OC1Init;
		OCxPreloadConfig = TIM_OC1PreloadConfig;
		OTHER;
		break;
	case 10: GPIOx = GPIOC;
	    GPIO_Pin_x = GPIO_Pin_5;
		// -- T1CH3_3
	    if ((LED_pwm[1-1].tim==1 && LED_pwm[1-1].oc==3) ||
	    		(LED_pwm[9-1].tim==1 && LED_pwm[9-1].oc==3)) {
	    	// punt to on/off if Timer+OC channel is already used
	    	set_LED_dutycycle(led, duty>50? 100: 0);
	    	return;
	    }
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=1, .oc=3};
		TIMx = TIM1;
		OCxInit = TIM_OC3Init;
		OCxPreloadConfig = TIM_OC3PreloadConfig;
		OTHER;
		break;
	}

	GPIO_InitTypeDef GPIO_InitStructure = {
    	.GPIO_Pin = GPIO_Pin_x,
    	.GPIO_Mode = GPIO_Mode_AF_PP,
		.GPIO_Speed = GPIO_Speed_30MHz
    };
    GPIO_Init(GPIOx, &GPIO_InitStructure);

    OCxInit(TIMx, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIMx, ENABLE);
    OCxPreloadConfig(TIMx, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIMx, ENABLE);
    TIM_Cmd(TIMx, ENABLE);
}


void LED_on(int n) {
	if (n < 1 || n > NUM_LEDS) return;
	set_LED_dutycycle(n, 100);
}
void LED_off(int n) {
	if (n < 1 || n > NUM_LEDS) return;
	set_LED_dutycycle(n, 0);
}


int get_current_time_ms(void)
{
	//FIXME
	return 0;
}


struct pattern *sequence_start;
int sequence_length;

int sequence_step;
int next_step_ms;

void setup(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();

    RCC_APB2PeriphClockCmd(
    		RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
			RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM1,
    		ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

	int arr = 100;
	unsigned psc = 480-1;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);


	sequence_start = chase;
	sequence_length = NUM_ARRAY_ELEMS(chase);
}
void loop(void)
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
