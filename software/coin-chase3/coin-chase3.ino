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
// 8 pin-pairs driving antiparallel LEDs, mapping per PCB
struct pin_pair {	int p1, p2;	}
const LED_pair_pins[NUM_LEDS_IN_PATTERN/2] = {
		{PC5, PC3/*TIM1_CH3*/},
		{PC2, PC0/*TIM2_CH3*/},
		{PC1, PA1/*TIM1_CH2*/},
		{PD0, PD7/*TIM2_CH4*/},
		{PD5, PD4/*TIM2_CH1*/},
		{PD1/*SWIO*/, PD3/*TIM2_CH2*/},
		{PC7, PD2/*TIM1_CH1*/},
		{PC6, PC4/*TIM1_CH4*/},
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
		digitalWrite(LED_pair_pins[led/2].p1, v1);
		digitalWrite(LED_pair_pins[led/2].p2, v2);

		return;
	}
	//FIXME: handle PWM dimming
}


void setup(void)
{
	for (int i = 0; i < NUM_ARRAY_ELEMS(LED_pair_pins); i++) {
		pinMode(LED_pair_pins[i].p1, OUTPUT);
		pinMode(LED_pair_pins[i].p2, OUTPUT);
	}	
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

#ifndef NDEBUG
    printf("%s %s\r\n", __DATE__, __TIME__);
    printf("SystemClk:%ld\r\n",(long)SystemCoreClock);
    printf("ChipID:%08lx\r\n", (long)DBGMCU_GetCHIPID() );
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
#ifndef NDEBUG
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
#endif
	if (current_time >= next_step_ms) {
		struct pattern p = get_next_pattern();

		next_step_ms = current_time + p.time;

		for (int i = 0; i < NUM_LEDS_IN_PATTERN; i++) {
#ifndef NDEBUG
			printf("set %d to %d%%\r\n", i, p.bright[i]);
#endif
			set_LED_dutycycle(i, p.bright[i]);
		}
	}
}
