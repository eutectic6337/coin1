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

//#include "debug.h"
#include <ch32v00x.h>

#include "systick.h"
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
#if 0
	unsigned long s = SysTick_seconds;
	unsigned ms = SysTick_milliseconds;
	static int a = 0;
	static int b = 0;
	if (++a > 2) {
		a = 0;
    	printf("%d\t%lu-%u\r\n", b++, s, ms);
	}
#endif
	//loop_capsense();
	loop_pwmleds();
	//loop_sleepwake();
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemInit();
	SystemCoreClockUpdate();
#if 0
	SDI_Printf_Enable();
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("validate TEST\r\n");
#endif

    init_SysTick();

    // various things work better (or at all) if we delay launching real work
    wait_ms(5000);

    init();

    while(1) {
    	loop();
    }
    return 0;
}
