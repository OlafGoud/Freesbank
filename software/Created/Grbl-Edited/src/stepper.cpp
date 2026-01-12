#include "stepper.h"

uint8 stepperState = STEPPER_EMPTY;

static StepperData stepperData; 


float getYForCircle(float x, float y) {
  float fTemp = sqrt((stepperData.formulaValues[Z_AXIS] * stepperData.formulaValues[Z_AXIS])- ((x - stepperData.formulaValues[Y_AXIS]) * (x - stepperData.formulaValues[Y_AXIS])));
  float y1 = stepperData.formulaValues[X_AXIS] + fTemp;
  float y2 = stepperData.formulaValues[X_AXIS] - fTemp;
  if(y1 - y > y2 - y) {
    /** y1 closer */
    return y1;
  } 

  /** y2 closer */
  return y2;
}

float getYForLine(float x, float x1, float x2, float y1, float y2) {
  return (((y2 - y1)/(x2 - x1)) * (x - x1)) + y1;
}


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
  stepperData.timerValue = compareValue;
  /** Timer config */
  println((int32)compareValue);
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
 // STEPPER_DIR_PORT |= STEPPER_DIR_MASK;
 // STEPPER_STEP_PORT |= STEPPER_STEP_MASK;
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


void loadNewSegment() {
  codeBlockBuffer.tail = (codeBlockBuffer.tail + 1) % CODEBLOCKBUFFERSIZE; /** Old block can be deleted. increment tail */
  if(codeBlockBuffer.tail == codeBlockBuffer.head) {
    return;
  }

  CodeBlock* block = &codeBlockBuffer.block[codeBlockBuffer.tail]; /** Dont increment here, then it can be overritten.*/
  if(block->command == 1 || block->command == 2) {
    stepperData.formulaValues[X_AXIS] = block->endPos[X_AXIS] - block->beginPos[X_AXIS];
    stepperData.formulaValues[Y_AXIS] = block->endPos[Y_AXIS] - block->beginPos[Y_AXIS];
    stepperData.formulaValues[Z_AXIS] = block->endPos[Z_AXIS] - block->beginPos[Z_AXIS];

    float maxValue = stepperData.formulaValues[X_AXIS] > stepperData.formulaValues[Y_AXIS] ? stepperData.formulaValues[X_AXIS] : stepperData.formulaValues[Y_AXIS];
    maxValue = maxValue > stepperData.formulaValues[Z_AXIS] ? maxValue : stepperData.formulaValues[Z_AXIS];

    stepperData.modifier = (int)stepperData.timerValue/maxValue;


  } else if (block->command == 2 || block->command == 3) {
    stepperData.formulaValues[X_AXIS] = block->I;
    stepperData.formulaValues[Y_AXIS] = block->J;
    stepperData.formulaValues[Z_AXIS] = block->R;
  } else if (block->command >= 17 || block->command <= 19) {
    stepperData.selectedPlane = block->subCommand;
  } else {
    return;
  }

  /** Here functions to turn on the spindle etc */


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

//   \((x-h)^{2}+(y-k)^{2}=r^{2}\)
