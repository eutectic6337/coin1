/*
 * pwmleds.c
 *
 *  Created on: Feb 21, 2025
 *      Author: mlp
 */
#include <ch32v00x.h>
#include "pwmleds.h"

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

void init_pwmleds(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {
    		.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2,
			.GPIO_Mode = GPIO_Mode_Out_PP,
			.GPIO_Speed = GPIO_Speed_30MHz
    };
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //init_TIM1_PWMOut(100, 4800-1, 20);
}

#include "systick.h"

#define PD2_HIGH_MS 100u
#define PD2_LOW_MS 300u

static timestamp next_PD2_high;
static timestamp next_PD2_low;

#define PD3_HIGH_MS 300u
#define PD3_LOW_MS 200u

static timestamp next_PD3_high;
static timestamp next_PD3_low;
void loop_pwmleds(void)
{
	u32 now = millis();
	if (!next_PD2_high) next_PD2_high = now;
	if (!next_PD2_low) next_PD2_low = now + PD2_HIGH_MS;
	if (!next_PD3_high) next_PD3_high = now;
	if (!next_PD3_low) next_PD3_low = now + PD3_HIGH_MS;

	if (now >= next_PD2_high) {
		GPIO_WriteBit(GPIOD, GPIO_Pin_2, 1);
		next_PD2_high += PD2_HIGH_MS + PD2_LOW_MS;
	}
	if (now >= next_PD2_low) {
		GPIO_WriteBit(GPIOD, GPIO_Pin_2, 0);
		next_PD2_low += PD2_HIGH_MS + PD2_LOW_MS;
	}
	if (now >= next_PD3_high) {
		GPIO_WriteBit(GPIOD, GPIO_Pin_3, 1);
		next_PD3_high += PD3_HIGH_MS + PD3_LOW_MS;
	}
	if (now >= next_PD3_low) {
		GPIO_WriteBit(GPIOD, GPIO_Pin_3, 0);
		next_PD3_low += PD3_HIGH_MS + PD3_LOW_MS;
	}
}
