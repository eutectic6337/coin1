/*
 * CH32V003F4U6.h
 *
 *  Created on: Apr 29, 2025
 *      Author: mlp
 */

#ifndef USER_CH32V003F4U6_H_
#define USER_CH32V003F4U6_H_

const struct {
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
} GPIO_pin[] = {
		{0,0},

		#define PD7 1
		{GPIOD, GPIO_Pin_7},
		#define PA1 2
		{GPIOA, GPIO_Pin_1},
		#define PA2 3
		{GPIOA, GPIO_Pin_2},
		#define VSS 4
		{0,0},
		#define PD0 5
		{GPIOD, GPIO_Pin_0},
		#define VDD 6
		{0,0},
		#define PC0 7
		{GPIOC, GPIO_Pin_0},
		#define PC1 8
		{GPIOC, GPIO_Pin_1},
		#define PC2 9
		{GPIOC, GPIO_Pin_2},
		#define PC3 10
		{GPIOC, GPIO_Pin_3},
		#define PC4 11
		{GPIOC, GPIO_Pin_4},
		#define PC5 12
		{GPIOC, GPIO_Pin_5},
		#define PC6 13
		{GPIOC, GPIO_Pin_6},
		#define PC7 14
		{GPIOC, GPIO_Pin_7},
		#define PD1 15
		{GPIOD, GPIO_Pin_1},
		#define PD2 16
		{GPIOD, GPIO_Pin_2},
		#define PD3 17
		{GPIOD, GPIO_Pin_3},
		#define PD4 18
		{GPIOD, GPIO_Pin_4},
		#define PD5 19
		{GPIOD, GPIO_Pin_5},
		#define PD6 20
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
