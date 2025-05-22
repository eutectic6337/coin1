/*
 * CH32V003F4U6.c
 *
 *  Created on: May 22, 2025
 *      Author: mlp
 */
#include "CH32V003F4U6.h"
#include <assert.h>
#include "utility.h"

struct complete_GPIO_pin
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

void set_pin(int pinID, int value)
{
	assert(NUM_ARRAY_ELEMS(GPIO_pin) == total_package_pins+1);
	assert(pinID > 0);
	assert(pinID <= total_package_pins);

	assert(GPIO_pin[pinID].GPIOx);
	assert(GPIO_pin[pinID].GPIO_Pin);

	GPIO_WriteBit(GPIO_pin[pinID].GPIOx, GPIO_pin[pinID].GPIO_Pin, value);
}

