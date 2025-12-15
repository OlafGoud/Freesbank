#include "stepper.h"

uint8 stepperState = STEPPER_EMPTY;

static StepperBlock stepBlock;

void initSteppers() {



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


  



}



ISR(TIMER1_COMPA_vect) {
  /** variable to prevent rentering the loop until it is finished */
  static bool isBusy = false;

  if(isBusy) return; /** return if previous cycle is not done */

  isBusy = true; /** set busy */

  float currentPosition[3]{};
  getCurrentMMFromEncoders(currentPosition);

  float nextPos[3]{};
  for(int i = 0; i < 3; i++) {
    nextPos[i] = stepBlock.beginPosition[i] + (stepBlock.time * (stepBlock.exitPos[i] - stepBlock.beginPosition[i]));
  }


  for(int i = 0; i < 3; i++) {
    
  }

  stepBlock.time += TIMECONST;


  /** Set direction */
  PORTD = (PORTD & ~STEPPER_DIR_MASK) | stepBlock.dirBits;

  /** step */
  PORTD = (PORTD & ~STEPPER_STEP_MASK) | stepBlock.stepBits;
  

  /** set reset isr */
  OCR1B = TCNT1 + (800 * 2); // 0.5us per tick at prescale=8 (800 = 800 microseconds (us))
  TIMSK1 |= (1 << OCIE1B);
  

  /** set next stepbits */

  /* model refrence control */

  isBusy = false;
}

ISR(TIMER1_COMPB_vect) {
  /** Set stepper low */

  PORTD &= ~(1 << PD5); /* set pin 5 low*/


  TIMSK1 &= ~(1 << OCIE1B);            // disable until next cycle
}