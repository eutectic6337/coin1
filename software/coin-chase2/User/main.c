/*
 * 10-coin-chase2
 */

//#define NDEBUG
#include <assert.h>
#include "debug.h"

/* code features */
#define ONOFF_ONLY

#include "utility.h"
#include "sequence.h" // defines NUM_LEDS_IN_PATTERN

/* hardware layout */
#define REMAP 00
#include "CH32V003F4U6.h"
// 8 pin-pairs driving antiparallel LEDs, mapping per PCB
struct pin_pair {	enum package_pins p1, p2;	}
const LED_pair_pins[NUM_LEDS_IN_PATTERN/2] = {
		{PC5, TIM1_CH3},
		{PC2, TIM2_CH3},
		{PC1, TIM1_CH2},
		{PD0, TIM2_CH4},
		{PD5, TIM2_CH1},
		{SWIO, TIM2_CH2},
		{PC7, TIM1_CH1},
		{PC6, TIM1_CH4},
};
// actual I/O pins used:
// A 1
// C 0,1,2,3,4,5,6,7
// D 0,1,2,3,4,5,7


int next_step_ms;

void set_LED_dutycycle(int led, int duty)
{
	assert(led > 0);
	assert(led < NUM_LEDS_IN_PATTERN+1);

	assert(duty >= 0);
	assert(duty <= 100);

#ifdef ONOFF_ONLY
	if (duty >= 50) duty = 100;
	if (duty < 50) duty = 0;
#endif

	if (duty < 1 || duty > 99) { // full off, or full on
		int value = duty > 99;

		int oddLED = led & 1;//"odd" per PCB (1-based)
		int v1 = value * oddLED;
		int v2 = value * !oddLED;

		led--;
#ifndef NDEBUG
		// don't try to adjust pins being used for debug I/O
		if (LED_pair_pins[led/2].p1 == SWIO
				|| LED_pair_pins[led/2].p2 == SWIO
				|| LED_pair_pins[led/2].p1 == NRST
				|| LED_pair_pins[led/2].p2 == NRST) {
			return;
		}
#endif
		set_pin(LED_pair_pins[led/2].p1, v1);
		set_pin(LED_pair_pins[led/2].p2, v2);

		return;
	}
	//FIXME: handle PWM dimming
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
	if (SysTick_millis > 1000ul * 60 * 60 * 24) {
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
    printf("%s %s\r\n", __DATE__, __TIME__);
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

}

#define INNER1 1000
void usleep(int n)
{
	volatile int m;
	m = n;
	while (m > 0) {
		m--;
		for (volatile int a = 0; a < INNER1; a++) {
			;
		}
	}
}
#define INNER2 1000000
void sleep(int n)
{
	volatile int m;
	m = n;
	while (m > 0) {
		m--;
		for (volatile int a = 0; a < INNER2; a++) {
			;
		}
	}
}

void loop(void)
{
	int current_time = millis();
	printf("%d ms .. %d ms\r\n", current_time, next_step_ms);
	usleep(123);
	//sleep(1);
	printf("%d ms\r\n", (int)millis());
	usleep(456);
	//sleep(2);
	printf("%d ms\r\n", (int)millis());
	usleep(789);
	//sleep(3);
	printf("%d ms\r\n", (int)millis());

	if (current_time >= next_step_ms) {
		struct pattern p = get_next_pattern();

		next_step_ms = current_time + p.time;

		for (int i = 0; i < NUM_LEDS_IN_PATTERN; i++) {
			set_LED_dutycycle(i, p.bright[i]);
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
