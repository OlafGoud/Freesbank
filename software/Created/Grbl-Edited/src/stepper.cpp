#include "stepper.h"

uint8 stepperState = STEPPER_EMPTY;

static StepperBlock stepBlock;

static const uint8 dirPins[] = {PB5, PB6, PB7};
static const uint8 stepPins[] = {PD5, PD6, PD7};
/** 
 * Set timer1, 16bit, prescalers(1,8,64,256,1024)
 */
uint8 setHardwareCompareTimer(float targetms) {

  uint32 steps = 0;
  uint16 compareValue = 0;
  uint8 prescalerBits = 0;
  uint16 restCount = 0;
  struct {
    uint16 prescaler; 
    uint8 bits;
  } options[] = {
    {1, (1 << CS10)},
    {8, (1 << CS11)},
    {64, (1 << CS11) | (1 << CS10)},
    {256, (1 << CS12)},
    {1024, (1 << CS12) | (1 << CS10)}
  };

  for(uint8 i = 0; i < 5; i++) {
    steps = static_cast<uint32>((F_CPU / options[i].prescaler) * (targetms / 1000));
    if(steps <= 65535) {

      if(((F_CPU / options[i].prescaler) * (RESET_STEPBITS_US / 1000000)) >= 65535) {
        return 1;
      }
      restCount = (F_CPU / options[i].prescaler) * (RESET_STEPBITS_US / 1000000);
      if(restCount == 0) {
        return 2; /** reset to low */
      }

      compareValue = (uint16)(steps - 1);
      prescalerBits = options[i].bits;
      i = 10;
    }
  }

  if(prescalerBits == 0) {
    return 1;
  }

  /** Timer config */

  cli();

  TCCR1A = 0;
  TCCR1A = 0;

  TCNT1 = 0;
  OCR1A = compareValue;   /** Set target value */
  OCR1B = restCount;      /** Set rest target value */

  TCCR1B |= (1 << WGM12);   /** Enable CTC Mode */
  TCCR1B |= prescalerBits; /** Start counter */

  TIMSK1 |= (1 << OCIE1A); /** Turn compare 1a on */


  sei();

  return 0;
}


void initSteppers() {

  /** Set stepper pins to output */
  STEPPER_DIR_PORT |= STEPPER_DIR_MASK;
  STEPPER_STEP_PORT |= STEPPER_STEP_MASK;
  uint8 hardwareCompareStatusCode = setHardwareCompareTimer(100.0f);
  if(!(hardwareCompareStatusCode == 0)) {
    if(hardwareCompareStatusCode == 1) {
      println("ERR: DelayToHigh");
    } else if (hardwareCompareStatusCode == 2) {
      println("ERR: DelayResetToLow");
    } else {
      println("ERR: UnexpectedStatusCodeCompareTimer");
    }
    systemState = ERROR;
  }




}


void loadSegmentInBuffer(float currentPosition[3], PlannerBuffer& planBuffer) {

  if(planBuffer.tail == planBuffer.head) return;
  PlannerBlock* planBlock = &planBuffer.block[planBuffer.tail++];




  uint8 changes = 0;
  if(currentPosition[0] <= stepBlock.exitPos[0] + STEPPER_ACCURACY && encoderSteps[0] >= targetStep[0] - 4) {
    TIMSK1 &= ~(1 << OCIE1A); /** Disable Timer1 compare A interrupt */
    systemState = IDLE;
    return;
  }

  if(systemState == IDLE) {
    TIMSK1 |= (1 << OCIE1A);
    systemState = RUNNING;
    
  }

  stepBlock.intervalMS = 1000; /** magic value ( @todo feed rate implementation (mm / sec)) */
  



}


ISR(TIMER1_COMPA_vect) {
  /** variable to prevent rentering the loop until it is finished */
  static bool isBusy = false;

  if(isBusy) return; /** return if previous cycle is not done */

  isBusy = true; /** set busy */

  float currentPosition[3]{};
  getCurrentMMFromEncoders(currentPosition);

  /** Calculate next position on the line */
  float nextPos[3]{};
  for(int i = 0; i < 3; i++) {
    nextPos[i] = stepBlock.beginPosition[i] + (stepBlock.time * (stepBlock.exitPos[i] - stepBlock.beginPosition[i]));
  }


  for(int i = 0; i < 3; i++) {
    if(nextPos[i] - currentPosition[i] > STEPPER_ACCURACY) {
      stepBlock.dirBits |= (1 << dirPins[i]); /** Forward */
    } else if (nextPos[i] - currentPosition[i] < STEPPER_ACCURACY) {
      stepBlock.dirBits |= (1 << dirPins[i]); /** Backwords */
    } else {
      continue;
    }

    stepBlock.stepBits |= (1 << stepPins[i]);

  }

  stepBlock.time += stepBlock.intervalMS;


  /** Set direction */
  PORTD = (PORTD & ~STEPPER_DIR_MASK) | stepBlock.dirBits;

  /** step */
  PORTD = (PORTD & ~STEPPER_STEP_MASK) | stepBlock.stepBits;
  

  /** set reset isr */
  TCNT1 = 0;                /** Set timer to 0 */
  TIMSK1 |= (1 << OCIE1B);  /** enable Reset IRS */
  

  /** set next stepbits */

  /* model refrence control */

  isBusy = false;
}

ISR(TIMER1_COMPB_vect) {
  /** Set stepper low */

  PORTD &= ~(1 << PD5); /* set pin 5 low*/


  TIMSK1 &= ~(1 << OCIE1B);            // disable until next cycle
}

