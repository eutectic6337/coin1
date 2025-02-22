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

#define DEBUG
#ifdef DEBUG
#include "debug.h"
#else
#include <ch32v00x.h>
#endif

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
#ifdef DEBUG
	const timestamp PRINT_INTERVAL = 1000ul * 1000;
	static timestamp NEXT_PRINT = 0;
	timestamp now = time_now();
	if (now >= NEXT_PRINT) {
		NEXT_PRINT = now + PRINT_INTERVAL;
    	printf("%" PRINT_FORMAT_timestamp "\r\n", now);
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
#ifdef DEBUG
    Delay_Init();
    Delay_Ms(1000);

	SDI_Printf_Enable(); // uses MRS Delay() so must be before any SysTick stuff
    printf("\r\n");
    printf("SystemClk:%" PRIu32 "\r\n", SystemCoreClock);
    printf("ChipID:%08" PRIx32 "\r\n", DBGMCU_GetCHIPID() );
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
