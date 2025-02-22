/*
 * capsense.c
 *
 *  Created on: Feb 21, 2025
 *      Author: mlp
 */
#include <ch32v00x.h>
#include "capsense.h"

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
