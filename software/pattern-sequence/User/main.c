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
	{{100,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 100},//[0]
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
	{{100,  0,  0,  0,  0,  0,  0,  0,  0,100}, 100},//[19]
};

struct pattern heartbeat[] = {
    // First beat
    {{ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 }, 50},   // Dim start
    {{ 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 }, 100},  // Rising
    {{100,100,100,100,100,100,100,100,100,100 }, 150},  // Peak brightness
    {{ 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 }, 100},  // Falling
    {{ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 }, 50},   // Back to dim
    // Second beat
    {{ 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 }, 50},   // Slightly stronger start
    {{ 70, 70, 70, 70, 70, 70, 70, 70, 70, 70 }, 100},  // Rising
    {{100,100,100,100,100,100,100,100,100,100 }, 150},  // Peak brightness
    {{ 70, 70, 70, 70, 70, 70, 70, 70, 70, 70 }, 100},  // Falling
    {{ 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 }, 50},   // Back to dim
    // Pause between beats
    {{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, 200},  // Rest period
};

// This will be a practice in PWM or PWM emulation. The 'head' will be at 70-80% brightness, while the tail (3-5 segments) will be at 30-50% Brightness
// If the pattern isn't working as intended, we can simplify it by simply making the snake moving around the radius of the coin
struct pattern snake[] = {
    // Snake starting at D5
    {{ 80, 40, 30, 30, 30,  0,  0,  0,  0,  0 }, 100},  // Head at D5, body follows
    {{ 40, 80, 40, 30, 30,  0,  0,  0,  0,  0 }, 100},  // Head at D4
    {{ 30, 40, 80, 40, 30,  0,  0,  0,  0,  0 }, 100},  // Head at D3
    {{ 30, 30, 40, 80, 40,  0,  0,  0,  0,  0 }, 100},  // Head at D2
    {{ 30, 30, 30, 40, 80,  0,  0,  0,  0,  0 }, 100},  // Head at D1

    // Transition to D10
    {{  0, 30, 30, 30, 40, 80, 40, 30,  0,  0 }, 100},  // Head at D10
    {{  0,  0, 30, 30, 30, 40, 80, 40, 30,  0 }, 100},  // Head at D9
    {{  0,  0,  0, 30, 30, 30, 40, 80, 40, 30 }, 100},  // Head at D8
    {{  0,  0,  0,  0, 30, 30, 30, 40, 80, 40 }, 100},  // Head at D7
    {{  0,  0,  0,  0,  0, 30, 30, 30, 40, 80 }, 100},  // Head at D6

    // Complete the circle, back to D5
    {{ 40,  0,  0,  0,  0, 30, 30, 30, 30, 80 }, 100},  // Head at D5
};

// Simple alternating patten between the labeled D1-D10 (Evens and Odds)
struct pattern alternate_pulsate[] = {
    // Even LEDs increasing brightness, Odd LEDs decreasing brightness
    {{ 20,  80,  20,  80,  20,  80,  20,  80,  20,  80 }, 100},
    {{ 40,  60,  40,  60,  40,  60,  40,  60,  40,  60 }, 100},
    {{ 60,  40,  60,  40,  60,  40,  60,  40,  60,  40 }, 100},
    {{ 80,  20,  80,  20,  80,  20,  80,  20,  80,  20 }, 100},
    {{100,   0, 100,   0, 100,   0, 100,   0, 100,   0 }, 100}, // Even fully ON, Odd fully OFF
    // Reverse: Odd LEDs increasing brightness, Even LEDs decreasing brightness
    {{ 80,  20,  80,  20,  80,  20,  80,  20,  80,  20 }, 100},
    {{ 60,  40,  60,  40,  60,  40,  60,  40,  60,  40 }, 100},
    {{ 40,  60,  40,  60,  40,  60,  40,  60,  40,  60 }, 100},
    {{ 20,  80,  20,  80,  20,  80,  20,  80,  20,  80 }, 100},
    {{  0, 100,   0, 100,   0, 100,   0, 100,   0, 100 }, 100}, // Odd fully ON, Even fully OFF
};

// If holding the Coin at 45 degrees, you can see a five-point star form, pulsate, then repeat

// First pattern: Tracing the star
#define STAR_TRACE \
	{{  0,  0,  0,100,  0,  0,  0,  0,  0,  0}, 200}, /* D4 */ \
	{{  0,  0,  0,  0,  0,  0,  0,100,  0,  0}, 200}, /* D8 */ \
	{{  0,  0,100,  0,  0,  0,  0,  0,  0,  0}, 200}, /* D2 */ \
	{{  0,  0,  0,  0,  0,  0,100,  0,  0,  0}, 200}, /* D6 */ \
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,100}, 200}, /* D10 */ \
	{{  0,  0,  0,100,  0,  0,  0,  0,  0,  0}, 200} /* Back to D4 */
struct pattern star_trace[] = { STAR_TRACE };
// Second pattern: Keep traced LEDs at 80% brightness
#define STAR_HOLD \
	{{  0,  0, 80, 80,  0,  0, 80, 80,  0, 80}, 500}, \
	{{  0,  0, 80, 80,  0,  0, 80, 80,  0, 80}, 500}
struct pattern star_hold[] = { STAR_HOLD };
// Third pattern: Pulsate the 5 traced LEDs from 80% to 100%
#define STAR_PULSATE \
	{{  0,  0, 80, 80,  0,  0, 80, 80,  0, 80}, 100}, \
	{{  0,  0, 100,100,  0,  0,100,100,  0,100}, 100}, \
	{{  0,  0, 80, 80,  0,  0, 80, 80,  0, 80}, 100}, \
	{{  0,  0, 100,100,  0,  0,100,100,  0,100}, 100}, \
	{{  0,  0, 80, 80,  0,  0, 80, 80,  0, 80}, 100}
struct pattern star_pulsate[] = { STAR_PULSATE };

// Fourth pattern: Taper down the brightness in the traced order
#define STAR_TAPER \
	{{  0,  0,  0,100,  0,  0,  0,  0,  0,  0}, 200}, /* D4 */ \
	{{  0,  0,  0,  0,  0,  0,  0,100,  0,  0}, 200}, /* D8 */ \
	{{  0,  0,100,  0,  0,  0,  0,  0,  0,  0}, 200}, /* D2 */ \
	{{  0,  0,  0,  0,  0,  0,100,  0,  0,  0}, 200}, /* D6 */ \
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,100}, 200}, /* D10 */ \
	{{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, 200} /* All off */
struct pattern star_taper[] = { STAR_TAPER };

struct pattern star [] = {
		STAR_TRACE,
		STAR_HOLD,
		STAR_PULSATE,
		STAR_TAPER
};

// 9-3, 2-10, 4-7, starburst timer
struct pattern starburst [] = {
};

//Bomb timer count down d5, d4, d3, d2, d1, d10, d9, d8, d7, d6 >> Explosion effect
struct pattern bomb [] = {
};

//Chasing Stars - two 100% LEDs with 1 1/2 length tails chasing each other around the coin
struct pattern starchase [] = {
};

// Seeing the coin as a clock face > Sweep from 9 to 3 and 3 to 9 (can do an up down sweep as well with 11 and 1 down to 7 and 5 and back up)
struct pattern sweep[] = {
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

//FIXME
#define REMAP
#define N

void set_LED_dutycycle(int led, int duty)
{
	// first call after all have been set, clear the flags
	if (LED_pwm[NUM_LEDS-1].gp || LED_pwm[NUM_LEDS-1].tim) {
		for (int i=0; i<NUM_LEDS; i++) LED_pwm[i] = (struct pwm_in_use){0};
	}

	if (duty < 1 || duty > 99) { // full off, or full on
		int value = duty > 99;

		LED_pwm[led-1] = (struct pwm_in_use){.gp=1, .tim=0, .oc=0};

		GPIO_TypeDef *GPIOx = 0;
	    uint16_t GPIO_Pin_x = 0;

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

	GPIO_TypeDef *GPIOx = 0;
    uint16_t GPIO_Pin_x = 0;
    TIM_TypeDef *TIMx = 0;
    void (*OCxInit)(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct) = 0;
    void (*OCxPreloadConfig)(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload) = 0;
	switch(led) {
	case 1: GPIOx = GPIOC;
	    GPIO_Pin_x = GPIO_Pin_3;
		//T1CH3 -- T1CH1
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
		REMAP;
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
		REMAP;
		break;
	case 5: GPIOx = GPIOD;
	    GPIO_Pin_x = GPIO_Pin_5;
		// -- T2CH4_3
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=2, .oc=4};
		TIMx = TIM2;
		OCxInit = TIM_OC4Init;
		OCxPreloadConfig = TIM_OC4PreloadConfig;
		REMAP;
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
		REMAP;
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
			REMAP;
			N;
			break;
	    }
		LED_pwm[led-1] = (struct pwm_in_use){.gp=0, .tim=1, .oc=1};
		TIMx = TIM1;
		OCxInit = TIM_OC1Init;
		OCxPreloadConfig = TIM_OC1PreloadConfig;
		REMAP;
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
		REMAP;
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
			RCC_APB2Periph_TIM1 | RCC_APB1Periph_TIM2,
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
		if (sequence_step < sequence_length-1) {
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
