/*
 * sequence.h
 *
 *  Created on: Jul 17, 2025
 *      Author: mlp
 */

#ifndef USER_SEQUENCE_H_
#define USER_SEQUENCE_H_

// change this if you change the circuit
#define NUM_LEDS_IN_PATTERN 16

struct pattern {
	int bright[NUM_LEDS_IN_PATTERN];// percentage of full
	int time;// milliseconds
};

// fetch the next LED pattern in the sequence
struct pattern get_next_pattern(void);

#endif /* USER_SEQUENCE_H_ */
