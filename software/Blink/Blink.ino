/*
  Blink
*/

#define NUM_ARRAY_ELEMS(a) ((sizeof a)/(sizeof a[0]))

struct pin_pair {	int p1, p2;	}
const LED_pair_pins[] = {
		{PC5, PC3/*TIM1_CH3*/},
		{PC2, PC0/*TIM2_CH3*/},
		{PC1, PA1/*TIM1_CH2*/},
		{PD0, PD7/*TIM2_CH4*/},
		{PD5, PD4/*TIM2_CH1*/},
		{PD1/*SWIO*/, PD3/*TIM2_CH2*/},
		{PC7, PD2/*TIM1_CH1*/},
		{PC6, PC4/*TIM1_CH4*/},
};

void setup() {
  delay(3000);
	for (int i = 0; i < NUM_ARRAY_ELEMS(LED_pair_pins); i++) {
		pinMode(LED_pair_pins[i].p1, OUTPUT);
		pinMode(LED_pair_pins[i].p2, OUTPUT);

		digitalWrite(LED_pair_pins[i].p1, LOW);
		digitalWrite(LED_pair_pins[i].p2, LOW);
	}
}

// the loop function runs over and over again forever
void loop() {
 	for (int i = 0; i < NUM_ARRAY_ELEMS(LED_pair_pins); i++) {
		digitalWrite(LED_pair_pins[i].p2, HIGH);
    delay(100);
		digitalWrite(LED_pair_pins[i].p1, HIGH);
		digitalWrite(LED_pair_pins[i].p2, LOW);
    delay(100);
		digitalWrite(LED_pair_pins[i].p1, LOW);
  }
}
