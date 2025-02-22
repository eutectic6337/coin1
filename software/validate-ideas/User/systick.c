/*
 * systick.c
 *
 *  Created on: Feb 21, 2025
 *      Author: mlp
 */
#include <ch32v00x.h>
#include "systick.h"

volatile u32 SysTick_milliseconds;
volatile u32 SysTick_seconds;

void init_SysTick(void)
{
	// Reset any pre-existing configuration
	SysTick->CTLR = 0;

	// Set the compare register to trigger once per millisecond
	SysTick->CMP = SYSTICK_ONE_MILLISECOND - 1;

	// Reset the Count Register, and the global counters to 0
	SysTick->CNT = 0;
	SysTick_milliseconds = 0;
	SysTick_seconds = 0;
    SysTick->SR &= ~SYSTICK_SR_CNTIF;

	// Set the SysTick Configuration
	// NOTE: By not setting SYSTICK_CTLR_STRE, we maintain compatibility with
	// busywait delay funtions used by ch32v003_fun.
	SysTick->CTLR |= SYSTICK_CTLR_STE   |  // Enable Counter
	                 SYSTICK_CTLR_STIE  |  // Enable Interrupts
	                 SYSTICK_CTLR_STCLK ;  // Set Clock Source to HCLK/1

	// Enable the SysTick IRQ
	NVIC_EnableIRQ(SysTicK_IRQn);
}

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
	// Bump the Compare Register for the next trigger
	// If more than this number of ticks elapse before the trigger is reset,
	// you may miss your next interrupt trigger
	// (Make sure the IRQ is lightweight and CMP value is reasonable)
	SysTick->CMP += SYSTICK_ONE_MILLISECOND;

	// don't need to lock within this handler, because only NMI is higher priority

	// Increment the milliseconds count
	SysTick_milliseconds++;
	if (SysTick_milliseconds > 1000) {
		SysTick_milliseconds = 0;
		SysTick_seconds++;
	}

	// Acknowledge interrupt
	SysTick->SR = 0;
}

timestamp time_now(void)
{
	__disable_irq();
	const u32 ticks = SysTick->CNT;
	const u32 ms = SysTick_milliseconds;
	timestamp now = SysTick_seconds;
	__enable_irq();

	now *= 1000;
	now += ms;
	now *= 1000;
	now += ticks / SYSTICK_ONE_MICROSECOND;

	return now;
}
