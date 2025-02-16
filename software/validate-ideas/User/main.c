/*
 * clock/timing?
 *
 * Capacitive sensing input: PA2
 *
 * back2back LEDs
 * PWM output:PD2af aka T1C1
 * GPIO output PD3
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

u16 micros;//FIXME
u16 millis;
u32 seconds;

void init_clock(void)
{

}
void loop_clock(void)
{
	if (get_lock(&micros)) {
		if (++micros > 1000) {
			micros = 0;
			giveup_lock(&micros);
			millis++;
			if (millis > 1000) {
				millis = 0;
				seconds++;
			}
		}
		else {
			giveup_lock(&micros);
		}
	}
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
    		.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2,
			.GPIO_Mode = GPIO_Mode_Out_PP,
			.GPIO_Speed = GPIO_Speed_30MHz
    };
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //init_TIM1_PWMOut(100, 48000-1, 50);
}
void loop_LEDout(void)
{
	GPIO_WriteBit(GPIOD, GPIO_Pin_2, 1);
	Delay_Ms(1);
	GPIO_WriteBit(GPIOD, GPIO_Pin_3, 1);
	Delay_Ms(2);
	GPIO_WriteBit(GPIOD, GPIO_Pin_2, 0);
	Delay_Ms(3);
	GPIO_WriteBit(GPIOD, GPIO_Pin_3, 0);
	Delay_Ms(4);
}

void init_sleepwake(void)
{

}
void loop_sleepwake(void)
{

}


void init(void)
{
	init_clock();
	//init_capsensing();
	init_LEDout();
	//init_sleepwake();
}
void loop(void)
{
	static int a = 0;
	static int b = 0;
	if (++a > 999) {
		a = 0;
    	printf("%d\t%lu-%u-%u\r\n", b++,
    			(unsigned long)seconds, (unsigned)millis, (unsigned)micros);
	}
	loop_clock();
	//loop_capsense();
	loop_LEDout();
	//loop_sleepwake();
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
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
