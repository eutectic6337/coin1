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
void set_pin(int pinID, int value)
{
	assert(pinID);
	assert(GPIO_pin[pinID].GPIOx);
	assert(GPIO_pin[pinID].GPIO_Pin);

	GPIO_WriteBit(GPIO_pin[pinID].GPIOx, GPIO_pin[pinID].GPIO_Pin, value);
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
			set_pin(P1, value);
			set_pin(G1, 0);
			break;
		case 2:
			set_pin(G1, value);
			set_pin(P1, 0);
			break;
		case 3:
			set_pin(P2, value);
			set_pin(G2, 0);
			break;
		case 4:
			set_pin(G2, value);
			set_pin(P2, 0);
			break;
		case 5:
			set_pin(P3, value);
			set_pin(G3, 0);
			break;
		case 6:
			set_pin(G3, value);
			set_pin(P3, 0);
			break;
		case 7:
			set_pin(P4, value);
			set_pin(G4, 0);
			break;
		case 8:
			set_pin(G4, value);
			set_pin(P4, 0);
			break;
		case 9:
			set_pin(P5, value);
			set_pin(G5, 0);
			break;
		case 10:
			set_pin(G5, value);
			set_pin(P5, 0);
			break;
		case 11:
			set_pin(P6, value);
			set_pin(G6, 0);
			break;
		case 12:
			set_pin(G6, value);
			set_pin(P6, 0);
			break;
		case 13:
			set_pin(P7, value);
			set_pin(G7, 0);
			break;
		case 14:
			set_pin(G7, value);
			set_pin(P7, 0);
			break;
		case 15:
			set_pin(P8, value);
			set_pin(G8, 0);
			break;
		case 16:
			set_pin(G8, value);
			set_pin(P8, 0);
			break;
		}
		return;
	}
}

// SysTick code from ch32fun/systick_irq.h with mlp mods

// Number of ticks elapsed per millisecond (48,000 when using 48MHz Clock)
#define SYSTICK_ONE_MILLISECOND ((uint32_t)SystemCoreClock / 1000)
// Number of ticks elapsed per microsecond (48 when using 48MHz Clock)
#define SYSTICK_ONE_MICROSECOND ((uint32_t)SystemCoreClock / 1000000)
//FIXME: I'm not convinced using the WCH HAL variable here is ideal -mlp

// Simple macro functions to give a arduino-like functions to call
// millis() reads the incremented systick variable
// micros() reads the raw SysTick Count, and divides it by the number of
// ticks per microsecond ( WARN: Wraps every 90 seconds!)
#define millis() (SysTick_millis)
#define micros() (SysTick->CNT / SYSTICK_ONE_MICROSECOND)

// Incremented in the SysTick IRQ - in this example once per millisecond
volatile uint32_t SysTick_millis;
volatile uint32_t SysTick_days;

/*
 * SysTick ISR - must be lightweight to prevent the CPU from bogging down.
 * Increments Compare Register and systick_millis when triggered (every 1ms)
 */
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
	// advance the Compare Register for the next trigger

	// If more than this number of ticks elapse before the trigger is reset,
	// you may miss your next interrupt trigger
	// (Make sure the IQR is lightweight and CMP value is reasonable)
	SysTick->CMP += SYSTICK_ONE_MILLISECOND;

	// Clear the trigger state for the next IRQ
	SysTick->SR = 0x00000000;

	SysTick_millis++;
	if (SysTick_mills > 1000ul * 60 * 60 * 24) {
		SysTick_millis = 0;
		SysTick_days++;
	}
}

void SysTick_Setup(void)
{
	// Reset any pre-existing configuration
	SysTick->CTLR = 0x0000;

	// Set the compare register to trigger once per millisecond
	SysTick->CMP = SYSTICK_ONE_MILLISECOND - 1;

	// Reset the Count Register, and the global millis counter to 0
	SysTick->CNT = 0x00000000;
	SysTick_millis = 0x00000000;
	SysTick_days = 0x00000000;

	// Set the SysTick Configuration
	// NOTE: By not setting SYSTICK_CTLR_STRE, we maintain compatibility with
	// busywait delay functions [used by ch32v003_fun].
	SysTick->CTLR |= STK_CTLR_STE   |  // Enable Counter
	                 STK_CTLR_STIE  |  // Enable Interrupts
	                 STK_CTLR_STCLK ;  // Set Clock Source to HCLK/1

	// Enable the SysTick IRQ
	NVIC_EnableIRQ(SysTicK_IRQn);
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

    SysTick_Setup();

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

	int current_time = millis();
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
