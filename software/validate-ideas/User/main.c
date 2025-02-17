/*
 * clock/timing
 * - use SysTick interrupt to update milliseconds counter
 * - locking for exclusive access?
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

#include "debug.h"
//#include <ch32v00x.h>
#define SYSTEM_CORE_CLOCK (48ul*1000*1000)

/* Borrow from ch32fun:
 * - interrupt each millisecond
 * - don't auto-reload
 */

// Number of ticks elapsed per millisecond (48,000 when using 48MHz Clock)
#define SYSTICK_ONE_MILLISECOND (SYSTEM_CORE_CLOCK / 1000ul)
// Number of ticks elapsed per microsecond (48 when using 48MHz Clock)
#define SYSTICK_ONE_MICROSECOND (SYSTEM_CORE_CLOCK / 1000000ul)

//magic numbers used in demo code, not defined in MRS headers (that I can find)
//CTLR System Count Control
#define SYSTICK_CTLR_SWIE	(1u<<31)	// software interrupt enable
#define SYSTICK_CTLR_STRE	(1u <<3)	// auto-reload
#define SYSTICK_CTLR_STCLK_MASK	(1u <<2)
#define SYSTICK_CTLR_STCLK	(1u <<2)	// use HCLK
#define SYSTICK_CTLR_STCLK_8	( 0)	// use HCLK/8
#define SYSTICK_CTLR_STIE	(1u <<1)	// counter interrupt enable
#define SYSTICK_CTLR_STE	(1u <<0)	// counter enable
//SR System Count Status
#define SYSTICK_SR_CNTIF	(1u <<0) // write 0 to clear

volatile u32 SysTick_milliseconds;
volatile u32 SysTick_seconds; // max 4 Gs is more than 130 years

// Simple macro functions to give a arduino-like functions to call
// millis() reads the incremented systick variable
// micros() reads the raw SysTick Count, and divides it by the number of
// ticks per microsecond ( WARN: Wraps every 90 seconds!)
#define micros() (SysTick->CNT / SYSTICK_ONE_MICROSECOND)
#define millis() (SysTick_milliseconds)
#define seconds() (SysTick_seconds)

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

void wait_us(unsigned t)
{
	u32 us = micros() + t;
	while (micros() < us) ;
}
void wait_ms(unsigned t)
{
	u32 ms = millis() + t;
	while (millis() < ms) ;
}
void wait_s(unsigned t)
{
	u32 s = seconds() + t;
	while (seconds() < s) ;
}

typedef uint64_t timestamp;

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


void init_capsense(void)
{
#if 0
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    GPIO_InitTypeDef GPIO_InitStructure = {
    		.GPIO_Pin = GPIO_Pin_2,
			.GPIO_Mode = GPIO_Mode_AIN
    };
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitTypeDef  ADC_InitStructure = {
    		.ADC_Mode = ADC_Mode_Independent,
			.ADC_ScanConvMode = DISABLE,
			.ADC_ContinuousConvMode = ENABLE,
			.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None,
			.ADC_DataAlign = ADC_DataAlign_Right,
			.ADC_NbrOfChannel = 1
    };
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Calibration_Vol(ADC1, ADC_CALVOL_50PERCENT);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
#endif
}
void loop_capsense(void)
{

}

/* PWM Output Mode Selection */
#define PWM_MODE1   0
#define PWM_MODE2   1
#define PWM_MODE PWM_MODE2

void init_TIM1_PWMOut(u16 period, u16 prescaler, u16 pulsewidth)
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
void init_LEDout(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {
    		.GPIO_Pin = GPIO_Pin_3,
			.GPIO_Mode = GPIO_Mode_Out_PP,
			.GPIO_Speed = GPIO_Speed_30MHz
    };
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    init_TIM1_PWMOut(100, 4800-1, 20);
}

#define PD3_HIGH_MS 3000
#define PD3_LOW_MS 2000

timestamp next_PD3_high;
timestamp next_PD3_low;
void loop_LEDout(void)
{
	u32 now = millis();
	if (!next_PD3_high) next_PD3_low = now;
	if (!next_PD3_low) next_PD3_low = now + PD3_HIGH_MS;

	if (now >= next_PD3_high) {
		GPIO_WriteBit(GPIOD, GPIO_Pin_3, 1);
		next_PD3_high += PD3_HIGH_MS + PD3_LOW_MS;
	}
	if (now >= next_PD3_low) {
		GPIO_WriteBit(GPIOD, GPIO_Pin_3, 0);
		next_PD3_low += PD3_HIGH_MS + PD3_LOW_MS;
	}
}

void init_sleepwake(void)
{

}
void loop_sleepwake(void)
{

}


void init(void)
{
	//init_capsensing();
	init_LEDout();
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
	loop_LEDout();
	//loop_sleepwake();
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
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
