/*
  CH32V003 Random Circle Pulse Pattern
  Delayed-Overlap Origin-Based Handoff
  No peak hold / 60% brightness cap

  Behavior:
  - Starts from one single origin LED
  - Excludes D11 and D12 entirely
  - Randomly passes light between D1-D10 and D13-D16
  - Prevents same diode from repeating back-to-back
  - Prevents adjacent physical diodes from being selected as the next handoff
  - Uses delayed overlap:
      current LED fades out alone
      when current LED is ~75% through its fade-out, next LED starts fading in
  - Removes the peak hold to avoid visible blinking/stutter at max brightness
  - Hard-caps all brightness at 60%
  - Keeps original pin declaration pattern
  - Keeps PD7 in original position
  - Does NOT rewrite option bytes

  WCH-LinkUtility:
  - PD7 should be configured as:
    "Disable mul-func, PD7 used for IO function"
*/

#define NUM_ARRAY_ELEMS(a) ((sizeof(a)) / (sizeof((a)[0])))

struct pin_pair {
  int p1;
  int p2;
};

struct diode_drive {
  int diodeNumber;
  int highPin;
  int lowPin;
};

const pin_pair LED_pair_pins[] = {
  {PC5, PC3/*TIM1_CH3*/},            // D1 / D2
  {PC2, PC0/*TIM2_CH3*/},            // D3 / D4
  {PC1, PA1/*TIM1_CH2*/},            // D5 / D6
  {PD0, PD7/*TIM2_CH4*/},            // D7 / D8
  {PD5, PD4/*TIM2_CH1*/},            // D9 / D10
  {PD1/*SWIO*/, PD3/*TIM2_CH2*/},    // D11 / D12 - EXCLUDED
  {PC7, PD2/*TIM1_CH1*/},            // D13 / D14
  {PC6, PC4/*TIM1_CH4*/},            // D15 / D16
};

const int ALL_LED_PINS[] = {
  PC5, PC3,
  PC2, PC0,
  PC1, PA1,
  PD0, PD7,
  PD5, PD4,
  PD1, PD3,
  PC7, PD2,
  PC6, PC4
};

/*
  Physical diode order correction based on observed board behavior:

  For each pair:
  - Odd diode  = p2 HIGH, p1 LOW
  - Even diode = p1 HIGH, p2 LOW

  D11/D12 are intentionally omitted.
*/
const diode_drive DIODES[] = {
  {1,  PC3, PC5},
  {2,  PC5, PC3},

  {3,  PC0, PC2},
  {4,  PC2, PC0},

  {5,  PA1, PC1},
  {6,  PC1, PA1},

  {7,  PD7, PD0},
  {8,  PD0, PD7},

  {9,  PD4, PD5},
  {10, PD5, PD4},

  // D11 / D12 skipped entirely

  {13, PD2, PC7},
  {14, PC7, PD2},

  {15, PC4, PC6},
  {16, PC6, PC4},
};

/*
  Origin point.
  Valid options here: 1-10, 13-16.
*/
const int ORIGIN_DIODE = 1;

/*
  Brightness cap.
*/
const int MIN_DUTY_PERCENT = 3;
const int MAX_DUTY_PERCENT = 60;

/*
  More steps = smoother fade.
*/
const int HANDOFF_STEPS = 72;

/*
  Next LED starts when current LED is this far through its fade-out.
  75 = next LED starts at 3/4 through the current LED's cycle.
*/
const int OVERLAP_START_PERCENT = 75;

/*
  Handoff timing.
*/
const unsigned int STEP_HOLD_MS = 24;

/*
  Faster PWM reduces visible flicker/stutter.
  250us = ~4kHz software PWM target before overhead.
*/
const unsigned int PWM_PERIOD_US = 250;

/*
  Origin timing.
*/
const unsigned int ORIGIN_FADE_IN_HOLD_MS = 24;
const unsigned int ORIGIN_HOLD_MS         = 900;

/*
  No peak hold.
*/
const unsigned int HOLD_AFTER_PASS_MS = 20;
const unsigned int LOOP_GAP_MS        = 10;
const unsigned int STARTUP_WAIT_MS    = 1000;

int currentIndex = 0;

int clampDuty(int dutyPercent) {
  if (dutyPercent < 0) {
    return 0;
  }

  if (dutyPercent > MAX_DUTY_PERCENT) {
    return MAX_DUTY_PERCENT;
  }

  return dutyPercent;
}

void enableAFIOClock() {
  #ifdef RCC_APB2Periph_AFIO
    RCC->APB2PCENR |= RCC_APB2Periph_AFIO;
  #elif defined(RCC_AFIOEN)
    RCC->APB2PCENR |= RCC_AFIOEN;
  #else
    RCC->APB2PCENR |= 0x00000001;
  #endif
}

void use_PA1_mode() {
  /*
    PA1-friendly mode.
    Keeps D5/D6 working.
  */
  enableAFIOClock();
  AFIO->PCFR1 &= ~(1 << 15);
}

void allPinsHiZ() {
  for (int i = 0; i < NUM_ARRAY_ELEMS(ALL_LED_PINS); i++) {
    digitalWrite(ALL_LED_PINS[i], LOW);
    pinMode(ALL_LED_PINS[i], INPUT);
  }
}

void setupDiodePins(int highPin, int lowPin) {
  pinMode(lowPin, OUTPUT);
  digitalWrite(lowPin, LOW);

  pinMode(highPin, OUTPUT);
  digitalWrite(highPin, HIGH);
}

void pulseOnePWMWindow(int highPin, int lowPin, int dutyPercent) {
  /*
    This is the only function that actually lights an LED.
    Duty is clamped here, so nothing should exceed 60%.
  */

  dutyPercent = clampDuty(dutyPercent);

  if (dutyPercent <= 0) {
    allPinsHiZ();
    delayMicroseconds(PWM_PERIOD_US);
    return;
  }

  unsigned int onTimeUs  = (PWM_PERIOD_US * dutyPercent) / 100;
  unsigned int offTimeUs = PWM_PERIOD_US - onTimeUs;

  setupDiodePins(highPin, lowPin);
  delayMicroseconds(onTimeUs);

  allPinsHiZ();

  if (offTimeUs > 0) {
    delayMicroseconds(offTimeUs);
  }
}

void holdDiode(int diodeIndex, int dutyPercent, unsigned int holdMs) {
  int highPin = DIODES[diodeIndex].highPin;
  int lowPin  = DIODES[diodeIndex].lowPin;

  dutyPercent = clampDuty(dutyPercent);

  unsigned long endTime = millis() + holdMs;

  while (millis() < endTime) {
    pulseOnePWMWindow(highPin, lowPin, dutyPercent);
  }

  allPinsHiZ();
}

void fadeInOrigin(int diodeIndex) {
  int highPin = DIODES[diodeIndex].highPin;
  int lowPin  = DIODES[diodeIndex].lowPin;

  for (int duty = MIN_DUTY_PERCENT; duty <= MAX_DUTY_PERCENT; duty++) {
    unsigned long endTime = millis() + ORIGIN_FADE_IN_HOLD_MS;

    while (millis() < endTime) {
      pulseOnePWMWindow(highPin, lowPin, duty);
    }
  }

  allPinsHiZ();
}

void delayedOverlapHandoff(int fromIndex, int toIndex) {
  /*
    Handoff logic:

    0% -> 75%:
      - Current LED fades down alone.
      - Next LED is off.

    75% -> 100%:
      - Current LED continues fading down.
      - Next LED fades up.

    No peak hold:
      - Avoids visible blink/stutter at max brightness.
  */

  int fromHigh = DIODES[fromIndex].highPin;
  int fromLow  = DIODES[fromIndex].lowPin;

  int toHigh = DIODES[toIndex].highPin;
  int toLow  = DIODES[toIndex].lowPin;

  int overlapStartStep = (HANDOFF_STEPS * OVERLAP_START_PERCENT) / 100;

  for (int step = 0; step <= HANDOFF_STEPS; step++) {
    int fromDuty = map(step, 0, HANDOFF_STEPS, MAX_DUTY_PERCENT, MIN_DUTY_PERCENT);
    fromDuty = clampDuty(fromDuty);

    int toDuty = 0;

    if (step >= overlapStartStep) {
      toDuty = map(step, overlapStartStep, HANDOFF_STEPS, MIN_DUTY_PERCENT, MAX_DUTY_PERCENT);
      toDuty = clampDuty(toDuty);
    }

    unsigned long endTime = millis() + STEP_HOLD_MS;

    while (millis() < endTime) {
      if (fromDuty > 0) {
        pulseOnePWMWindow(fromHigh, fromLow, fromDuty);
      }

      if (toDuty > 0) {
        pulseOnePWMWindow(toHigh, toLow, toDuty);
      }
    }
  }

  /*
    No max-brightness hold here.
    The next diode immediately becomes the new source for the next handoff.
  */
  allPinsHiZ();
  delay(HOLD_AFTER_PASS_MS);
}

int wrapIndex(int idx) {
  int count = NUM_ARRAY_ELEMS(DIODES);

  while (idx < 0) {
    idx += count;
  }

  while (idx >= count) {
    idx -= count;
  }

  return idx;
}

int findDiodeIndexByNumber(int diodeNumber) {
  for (int i = 0; i < NUM_ARRAY_ELEMS(DIODES); i++) {
    if (DIODES[i].diodeNumber == diodeNumber) {
      return i;
    }
  }

  return 0;
}

bool areAdjacentDiodes(int diodeA, int diodeB) {
  /*
    Physical circle adjacency.

    D1 is adjacent to D2 and D16.
    D16 is adjacent to D15 and D1.

    D11/D12 are excluded from the usable list,
    but they still exist physically, so:
      D10 is adjacent to D9 and D11, not D13.
      D13 is adjacent to D12 and D14, not D10.
  */

  if (diodeA == diodeB) {
    return true;
  }

  int diff = abs(diodeA - diodeB);

  if (diff == 1) {
    return true;
  }

  if (diff == 15) {
    return true;
  }

  return false;
}

bool isValidNextIndex(int candidateIndex) {
  int currentDiode   = DIODES[currentIndex].diodeNumber;
  int candidateDiode = DIODES[candidateIndex].diodeNumber;

  // No same-light repeat
  if (candidateIndex == currentIndex) {
    return false;
  }

  // No adjacent physical diode handoff
  if (areAdjacentDiodes(currentDiode, candidateDiode)) {
    return false;
  }

  return true;
}

int chooseNextIndex() {
  int count = NUM_ARRAY_ELEMS(DIODES);

  /*
    Random non-adjacent handoff:
    - No same-light repeat
    - No adjacent physical diode
    - D11/D12 impossible because they are not in DIODES[]
  */

  for (int attempts = 0; attempts < 50; attempts++) {
    int candidateIndex = random(count);

    if (isValidNextIndex(candidateIndex)) {
      return candidateIndex;
    }
  }

  /*
    Fallback: walk until a valid non-adjacent diode is found.
  */
  for (int offset = 2; offset < count; offset++) {
    int candidateIndex = wrapIndex(currentIndex + offset);

    if (isValidNextIndex(candidateIndex)) {
      return candidateIndex;
    }
  }

  return currentIndex;
}

void setup() {
  delay(STARTUP_WAIT_MS);

  enableAFIOClock();
  use_PA1_mode();
  allPinsHiZ();

  randomSeed(micros());

  currentIndex = findDiodeIndexByNumber(ORIGIN_DIODE);

  /*
    Point of origin:
    Start with only one LED.
  */
  fadeInOrigin(currentIndex);
  holdDiode(currentIndex, MAX_DUTY_PERCENT, ORIGIN_HOLD_MS);

  allPinsHiZ();

  /*
    Do NOT rewrite option bytes here.

    PD7 should be configured in WCH-LinkUtility:
    "Disable mul-func, PD7 used for IO function"
  */
}

void loop() {
  use_PA1_mode();

  int nextIndex = chooseNextIndex();

  delayedOverlapHandoff(currentIndex, nextIndex);

  currentIndex = nextIndex;

  delay(LOOP_GAP_MS);
}
