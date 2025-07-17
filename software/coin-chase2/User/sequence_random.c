/*
 * sequence_random.c
 *
 *  Created on: Jul 17, 2025
 *      Author: mlp
 *
 *  produce an infinite series of random patterns
 */
#include "sequence.h"
#include <stdlib.h>
struct pattern get_next_pattern(void)
{
	struct pattern p = {0};
	p.time = MIN_TIME + random() % (MAX_TIME - MIN_TIME);
	for (int i = 0; i < NUM_LEDS_IN_PATTERN; i++) {
		p.bright[i] = MIN_BRIGHT + random() %(MAX_BRIGHT - MIN_BRIGHT);
	}
	return p;
}
