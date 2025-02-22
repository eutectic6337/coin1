/*
 * systick.h
 *
 *  Created on: Feb 21, 2025
 *      Author: mlp
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

/* Borrow from ch32fun:
 * - interrupt each millisecond
 * - don't auto-reload
 */

#include <system_ch32v00x.h>
// Number of ticks elapsed per millisecond (48,000 when using 48MHz Clock)
#define SYSTICK_ONE_MILLISECOND (SystemCoreClock / 1000ul)
// Number of ticks elapsed per microsecond (48 when using 48MHz Clock)
#define SYSTICK_ONE_MICROSECOND (SystemCoreClock / 1000000ul)

//magic numbers used in demo code, not defined in MRS headers (that I can find)
//CTLR System Count Control
#define SYSTICK_CTLR_SWIE	(1u<<31)	/* software interrupt enable */
#define SYSTICK_CTLR_STRE	(1u <<3)	/* auto-reload */
#define SYSTICK_CTLR_STCLK_MASK	(1u <<2)
#define SYSTICK_CTLR_STCLK	(1u <<2)	/* use HCLK */
#define SYSTICK_CTLR_STCLK_8	( 0)	/* use HCLK/8 */
#define SYSTICK_CTLR_STIE	(1u <<1)	/* counter interrupt enable */
#define SYSTICK_CTLR_STE	(1u <<0)	/* counter enable */
//SR System Count Status
#define SYSTICK_SR_CNTIF	(1u <<0) /* write 0 to clear */


extern volatile u32 SysTick_milliseconds;
extern volatile u32 SysTick_seconds; // max 4 Gs is more than 130 years

// Simple macro functions to give a arduino-like functions to call
// millis() reads the incremented systick variable
// micros() reads the raw SysTick Count, and divides it by the number of
// ticks per microsecond ( WARN: Wraps every 90 seconds!)
#define micros() (SysTick->CNT / SYSTICK_ONE_MICROSECOND)
#define millis() (SysTick_milliseconds)
#define seconds() (SysTick_seconds)

#define wait_us(t) \
do { \
	u32 us = micros() + t; \
	while (micros() < us) ; \
} while(0)

#define wait_ms(t) \
do { \
	u32 ms = millis() + t; \
	while (millis() < ms) ; \
} while(0)

#define wait_s(t) \
do { \
	u32 s = seconds() + t; \
	while (seconds() < s) ; \
} while(0)

typedef uint64_t timestamp;
timestamp time_now(void);

void init_SysTick(void);

#endif /* SYSTICK_H_ */
