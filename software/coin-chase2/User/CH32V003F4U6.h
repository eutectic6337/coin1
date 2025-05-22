/*
 * CH32V003F4U6.h
 *
 *  Created on: Apr 29, 2025
 *      Author: mlp
 */

#ifndef USER_CH32V003F4U6_H_
#define USER_CH32V003F4U6_H_
#include <ch32v00x.h>

//refman 6.5.4.1 System Count Control Register (STK_CTLR)
#define	STK_CTLR_SWIE	(1u<<31)	//trigger software interrupt
#define	STK_CTLR_STRE	(1u<<3)	//auto-reload
#define	STK_CTLR_STCLK	(1u<<2)	//HCLK /1 not /8
#define	STK_CTLR_STIE	(1u<<1)	//counter interrupt enable
#define	STK_CTLR_STE	(1u<<0)	//counter enable

//refman 6.5.4.2 System Count Status Register (STK_SR)
#define	STK_SR_CNTIF	(1u<<0)	//write 0 => clear comparison


enum package_pins {
	PD7 = 1,
	PA1,
	PA2,
	VSS,
	PD0,

	VDD,
	PC0,
	PC1,
	PC2,
	PC3,

	PC4,
	PC5,
	PC6,
	PC7,
	PD1,

	PD2,
	PD3,
	PD4,
	PD5,
	PD6,

	total_package_pins
};
struct {
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
}
const GPIO_pin[] = {
		{0,0},//dummy element

		{GPIOD, GPIO_Pin_7},
		{GPIOA, GPIO_Pin_1},
		{GPIOA, GPIO_Pin_2},
		{0,0},
		{GPIOD, GPIO_Pin_0},

		{0,0},
		{GPIOC, GPIO_Pin_0},
		{GPIOC, GPIO_Pin_1},
		{GPIOC, GPIO_Pin_2},
		{GPIOC, GPIO_Pin_3},

		{GPIOC, GPIO_Pin_4},
		{GPIOC, GPIO_Pin_5},
		{GPIOC, GPIO_Pin_6},
		{GPIOC, GPIO_Pin_7},
		{GPIOD, GPIO_Pin_1},

		{GPIOD, GPIO_Pin_2},
		{GPIOD, GPIO_Pin_3},
		{GPIOD, GPIO_Pin_4},
		{GPIOD, GPIO_Pin_5},
		{GPIOD, GPIO_Pin_6},
};


#if REMAP == 00
#define UCK	PD4
#define A7	PD4
#define OPO	PD4
#define TIM2_ETR	PD4

#define UTX	PD5
#define A5	PD5
#define URX	PD6
#define A6	PD6

#define NRST	PD7
#define OPP1	PD7

#define A1	PA1
#define OPN0	PA1

#define A0	PA2
#define OPP0	PA2
#define TIM1_CH2N	PA2

#define OPN1	PD0
#define TIM1_CH1N	PD0

#define SDA	PC1
#define NSS	PC1

#define SCL	PC2
#define URTS	PC2
#define TIM1_BKIN	PC2

#define MCO	PC4
#define A2	PC4

#define SCK	PC5
#define TIM1_ETR	PC5

#define MOSI	PC6
#define MISO	PC7

#define SWIO	PD1
#define TIM1_CH3N	PD1
#define AETR2	PD1

#define A3	PD2

#define A4	PD3
#define AETR	PD3
#define UCTS	PD3

#define TIM1_CH1	PD2
#define TIM1_CH2	PA1
#define TIM1_CH3	PC3
#define TIM1_CH4	PC4

#define TIM2_CH1	PD4
#define TIM2_CH2	PD3
#define TIM2_CH3	PC0
#define TIM2_CH4	PD7

#elif REMAP == 01
#define TIM1_CH1	PC6
#define TIM1_CH2	PC7
#define TIM1_CH3	PC0
#define TIM1_CH4	PD3

#define TIM2_CH1	PC5
#define TIM2_CH2	PC2
#define TIM2_CH3	PD2
#define TIM2_CH4	PC1

#elif REMAP == 10
#define TIM1_CH1	PD2
#define TIM1_CH2	PA1
#define TIM1_CH3	PC3
#define TIM1_CH4	PC4

#define TIM2_CH1	PC1
#define TIM2_CH2	PD3
#define TIM2_CH3	PC0
#define TIM2_CH4	PD7

#elif REMAP == 11
#define TIM1_CH1	PC4
#define TIM1_CH2	PC7
#define TIM1_CH3	PC5
#define TIM1_CH4	PD4

#define TIM2_CH1	PC1
#define TIM2_CH2	PC7
#define TIM2_CH3	PD6
#define TIM2_CH4	PD5

#else
#error bad REMAP
#endif

#endif /* USER_CH32V003F4U6_H_ */
