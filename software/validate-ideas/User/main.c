/*
 * clock/timing?
 * - use timer interrupt to update microseconds counter
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

int get_lock(void* p)
{
	//FIXME
}
void giveup_lock(void* p)
{
	//FIXME
}

#define TICKS_PER_MILLISECOND 100
u8 ticks;
u16 milliseconds;
u32 seconds; // max 4 Gs is more than 130 years

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
	ticks++;
	if (ticks > TICKS_PER_MILLISECOND) {
		ticks = 0;
		milliseconds++;
		if (milliseconds > 1000) {
			milliseconds = 0;
			seconds++;
		}
	}
	SysTick->SR = 0;
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

    init_TIM1_PWMOut(100, 4800-1, 1);
}
void loop_LEDout(void)
{
	GPIO_WriteBit(GPIOD, GPIO_Pin_3, 1);
	Delay_Ms(200);
	GPIO_WriteBit(GPIOD, GPIO_Pin_3, 0);
	Delay_Ms(300);
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
	static int a = 0;
	static int b = 0;
	if (++a > 2) {
		a = 0;
    	printf("%d\t%lu-%u\r\n", b++,
    			(unsigned long)seconds, (unsigned)milliseconds);
	}
	//loop_capsense();
	loop_LEDout();
	//loop_sleepwake();
}

//SysTick
//CTLR System Count Control
#define ST_CTLR_SWIE (1u<<31)	// software interrupt enable
#define ST_CTLR_STRE (1u<<3)	// auto-reload
#define ST_CTLR_STCLK_MASK	(1u<<2)
#define ST_CTLR_STCLK_HCLK	(1u<<2)	// use HCLK
#define ST_CTLR_STCLK_HCLK_8	(0)	// use HCLK/8
#define ST_CTLR_STIE	(1u<<1)	// counter interrupt enable
#define ST_CTLR_STE	(1u<<0)	// counter enable
//SR System Count Status
#define ST_SR_CNTIF	(1u<<0) // write 0 to clear


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();

    //SysTick setup
    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTick->SR &= ~ST_SR_CNTIF;
    SysTick->CMP = (SystemCoreClock/1000/TICKS_PER_MILLISECOND)-1;
    SysTick->CNT = 0;
    SysTick->CTLR = ST_CTLR_STRE|ST_CTLR_STCLK_HCLK|ST_CTLR_STIE|ST_CTLR_STE;

	Delay_Init();
    Delay_Ms(5000);
	SDI_Printf_Enable();
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("validate TEST\r\n");

	init();

    while(1) {
    	loop();
    }
    return 0;
}
