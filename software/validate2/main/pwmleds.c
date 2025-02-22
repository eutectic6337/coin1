/*
 * pwmleds.c
 *
 *  Created on: Feb 21, 2025
 *      Author: mlp
 */
#include "ch32fun.h"
#include "pwmleds.h"

#ifdef USE_PWM
/* PWM Output Mode Selection */
#define PWM_MODE1   0
#define PWM_MODE2   1
#define PWM_MODE PWM_MODE2

static void init_TIM1_PWMOut(u16 period, u16 prescaler, u16 pulsewidth)
{
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_TIM1, ENABLE );

    GPIO_InitTypeDef GPIO_InitStructure = {
    		.GPIO_Speed = GPIO_Speed_30MHz,
			.GPIO_Pin = GPIO_Pin_2,
			.GPIO_Mode = GPIO_Mode_AF_PP
    };
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    TIM_TimeBaseInitTypeDef TimeBaseInitStructure={
    		.TIM_Period = period,
			.TIM_Prescaler = prescaler,
			.TIM_ClockDivision = TIM_CKD_DIV1,
			.TIM_CounterMode = TIM_CounterMode_Up
    };
    TIM_TimeBaseInit( TIM1, &TimeBaseInitStructure);

    TIM_OCInitTypeDef OCInitStructure={
    		.TIM_OutputState = TIM_OutputState_Enable,
			.TIM_Pulse = pulsewidth,
			.TIM_OCPolarity = TIM_OCPolarity_High,
			.TIM_OCMode =
#if (PWM_MODE == PWM_MODE1)
					TIM_OCMode_PWM1
#elif (PWM_MODE == PWM_MODE2)
					TIM_OCMode_PWM2
#endif
    };
    TIM_OC1Init( TIM1, &OCInitStructure );

    TIM_CtrlPWMOutputs(TIM1, ENABLE );
    TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable );
    TIM_ARRPreloadConfig( TIM1, ENABLE );
    TIM_Cmd( TIM1, ENABLE );
}
#endif

void init_pwmleds(void)
{
#ifdef USE_PWM
    //init_TIM1_PWMOut(100, 4800-1, 20);
#else
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;

	GPIOD->CFGLR &= ~(0xf<<(4*2));
	GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*2);

	GPIOD->CFGLR &= ~(0xf<<(4*3));
	GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*3);
#endif
}


void loop_pwmleds(void)
{
	uint32_t now = SysTick->CNT;
#ifdef USE_PWM
#else
	const uint32_t PD2_HI_ticks = Ticks_from_Ms(100ul);
	const uint32_t PD2_LO_ticks = Ticks_from_Ms(300ul);

	static uint32_t last_PD2_hi;
	static uint32_t last_PD2_lo = PD2_HI_ticks;

	const uint32_t PD3_HI_ticks = Ticks_from_Ms(300ul);
	const uint32_t PD3_LO_ticks = Ticks_from_Ms(200ul);

	static uint32_t last_PD3_hi;
	static uint32_t last_PD3_lo = PD3_HI_ticks;

	if ((now - last_PD2_hi) > PD2_HI_ticks) {
		GPIOD->BSHR = 1<<(2);
		last_PD2_hi = now;
	}
	if ((now - last_PD2_lo) > PD2_LO_ticks) {
		GPIOD->BCR = (1<<(2));
		last_PD2_lo = now;
	}
	if ((now - last_PD3_hi) > PD3_HI_ticks) {
		GPIOD->BSHR = 1<<(3);
		last_PD3_hi = now;
	}
	if ((now - last_PD3_lo) > PD3_LO_ticks) {
		GPIOD->BCR = (1<<(3));
		last_PD3_lo = now;
	}
#endif
}
