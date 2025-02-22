/*
 * clock/timing
 * - use SysTick interrupt to update milliseconds counter
 * - locking for exclusive access? disable interrupt around any read of
 *   multiple words
 *
 * Capacitive sensing input: PA2
 *
 * back2back LEDs
 * PWM output:PD2af aka T1C1
 * GPIO output PD3
 * - success: back-to-back LEDs in parallel across PD2/PD3 blink alternately
 *   per PD3, one at brightness set by T1C1 and one at inverse brightness
 * - to test: can switch PD3 to hi-Z for no LEDs lit, and switch back to hi/lo
 * - to test: can adjust T1C1 duty cycle in flight
 *
 * sleep/wakeup
 */

#include "ch32fun.h"
#include <stdio.h>
#include <inttypes.h>

#include "capsense.h"
#include "pwmleds.h"
#include "sleepwake.h"

void init(void)
{
	//init_capsense();
	init_pwmleds();
	//init_sleepwake();
}
void loop(void)
{
#ifdef DEBUG
	Delay_Ms(10);

	const uint32_t PRINT_INTERVAL_ticks = Ticks_from_Ms(1000ul);
	static uint32_t LAST_PRINT = 0;
	uint32_t now = SysTick->CNT;
	if ((now - LAST_PRINT) > PRINT_INTERVAL_ticks) {
	    LAST_PRINT = now;
    	printf("%" PRIu32 "\r\n", now);
	}
#endif
	//loop_capsense();
	loop_pwmleds();
	//loop_sleepwake();
}

int main(void)
{
	SystemInit();
#ifdef DEBUG
	Delay_Ms(1000);

    printf("\r\n");
    printf("validate TEST\r\n");
#endif

    // various things work better (or at all) if we delay launching real work
	Delay_Ms(5000);

    init();

    while(1) {
    	loop();
    }
    return 0;
}
