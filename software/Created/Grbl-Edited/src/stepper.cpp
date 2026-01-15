#include "stepper.h"

uint8 stepperState;

volatile static StepperData stepperData; 
float cururpos = 0;
float cururpos1 = 0;

void peras(int i, unsigned long currentcycle, unsigned long maxCycle, float currPos) {
  //print("b:");
  /*println(stepperData.currentBlock->beginPos);
  print("e:");
  println(stepperData.currentBlock->endPos);
  print("c:");
  println(currentcycle);*/

  float val = getYForLine(currentcycle, 0, maxCycle, stepperData.currentBlock->beginPos[i], stepperData.currentBlock->endPos[i]);  
  //print("cu:\0");
  //println(val, 4);
  if(stepperData.formulaValues[i] < 0) {
    if(currPos > val) {
      STEPPER_STEP_PORT |= (1 << X_STEP_PIN);
      print("F2:\0"); println(currPos, 2);
      /*if(i == 0) {
        cururpos -= 0.18;
        print("F1:\0"); println(cururpos, 2);
      } else if (i == 1) {
        cururpos1 -= 0.18;
        print("F2:\0"); println(cururpos1, 2);
      }*/
    }
  } else {
    if(currPos < val) {
      STEPPER_STEP_PORT |= (1 << X_STEP_PIN);
      print("F2:\0"); println(currPos, 2);

      /*if(i == 0) {
        cururpos += 0.18;
        print("F1:\0"); println(cururpos, 2);
      } else if (i == 1) {
        cururpos1 += 0.18;
        print("F2:\0"); println(cururpos1, 2);
      }*/
    }
    /** forward */
  }


}


/** not used for now */
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
    //println((int32)steps);
    if(steps <= 65535) {

      if(((float)F_CPU / options[i].prescaler) * ((float)RESET_STEPBITS_US / 1000000) >= 65535) {
        return 1;
      }
      
      restCount = ((float)F_CPU / options[i].prescaler) * ((float)RESET_STEPBITS_US / 1000000);
      //println((int32)restCount);
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
  stepperData.timerValue = 0;
  /** Timer config */
  //println((int32)compareValue);
  cli();

  TCCR1A = 0;
  TCCR1A = 0;

  TCNT1 = 0;
  OCR1A = compareValue;   /** Set target value */
  OCR1B = restCount;      /** Set rest target value */

  TCCR1B |= (1 << WGM12);   /** Enable CTC Mode */
  TCCR1B |= prescalerBits; /** Start counter */

  //TIMSK1 |= (1 << OCIE1A); /** Turn compare 1a on */


  sei();

  return 0;
}


void initSteppers() {

  /** set outputs for pins */
  STEPPER_DIR_PORT |= (1 << X_DIR_PIN);
  STEPPER_STEP_PORT |= (1 << X_STEP_PIN);

  /** setup stepperISR */
  uint8 hardwareCompareStatusCode = setHardwareCompareTimer(STEPPER_ISR_MS);
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
  print("StepperIRSMS: "); println(STEPPER_ISR_MS, 1);
  stepperState = STEPPER_EMPTY;
}


void loadNewSegment() {
  uint8 tail = (codeBlockBuffer.tail + 1) % CODEBLOCKBUFFERSIZE; /** Old block can be deleted. increment tail */
  if(tail == codeBlockBuffer.head) {
    if(systemState == RUNNING) systemState = IDLE; /** machine can not find new instructions, so it is idle */
    return;
  }
  println("New buffer for stepper."); /** debug line */
  codeBlockBuffer.tail = tail;

  CodeBlock* block = &codeBlockBuffer.block[tail]; /** Dont increment here, then it can be overritten.*/
  stepperData.currentBlock = &codeBlockBuffer.block[tail];
  if(block->command == 1 || block->command == 2) {
    stepperData.formulaValues[X_AXIS] = block->endPos[X_AXIS] - block->beginPos[X_AXIS];
    stepperData.formulaValues[Y_AXIS] = block->endPos[Y_AXIS] - block->beginPos[Y_AXIS];
    stepperData.formulaValues[Z_AXIS] = block->endPos[Z_AXIS] - block->beginPos[Z_AXIS];
    //print(stepperData.formulaValues[X_AXIS], 2);
    if (stepperData.formulaValues[X_AXIS] < 0) {
      STEPPER_DIR_PORT &= ~(1 << X_DIR_PIN);
    }



    float maxValue = abs(stepperData.formulaValues[X_AXIS]) > abs(stepperData.formulaValues[Y_AXIS]) ? abs(stepperData.formulaValues[X_AXIS]) : abs(stepperData.formulaValues[Y_AXIS]);
    maxValue = maxValue > abs(stepperData.formulaValues[Z_AXIS]) ? maxValue : abs(stepperData.formulaValues[Z_AXIS]);

    stepperData.modifier = (int)(maxValue / STEPPER_ACCURACY);
    println((int)stepperData.modifier);

  } else if (block->command == 2 || block->command == 3) {
    stepperData.formulaValues[X_AXIS] = block->I;
    stepperData.formulaValues[Y_AXIS] = block->J;
    stepperData.formulaValues[Z_AXIS] = block->R;
  } else if (block->command >= 17 || block->command <= 19) {
    stepperData.selectedPlane = block->subCommand;
  } else {
    return;
  }

  /** Here functions to turn on the spindle and other tings */

  /** Setting settings for machine */

  stepperData.timerValue = 0; /** set the timer back to 0 */
  stepperState = STEPPER_FULL; /** change state to indicate that there is a loaded stepper buffer */ 
  systemState = RUNNING; /** set the systemstate to running */
  TIMSK1 |= (1 << OCIE1A); /** activate the interrupts */
 
}

void setSegmentDone() {

}

ISR(TIMER1_COMPA_vect) {
  /** variable to prevent rentering the loop until it is finished */
  static bool isBusy = false;
  
  if(isBusy) return; /** return if previous cycle is not done */

  isBusy = true; /** set busy */
  float currentPosition[3]{};
  getCurrentMMFromEncoders(currentPosition);

  if(stepperData.timerValue >= stepperData.modifier) {
    if(!(currentPosition[0] < stepperData.currentBlock->endPos[0])) {
      //println("empty");
      stepperState = STEPPER_EMPTY; /** set the stepperstate to empty to get a new block */
      TIMSK1 &= ~(1 << OCIE1A); /** turn the stepper interrupt */
      /** Check if he is at the right place */
      println("DONE");
    }
    
  }

  peras(0, stepperData.timerValue, stepperData.modifier, currentPosition[0]);
  //peras(1, stepperData.timerValue, stepperData.modifier, cururpos1 /*currentPosition[0]*/);

  stepperData.timerValue++;

  /** set reset isr */
  TCNT1 = 0;                /** Set timer to 0 */
  TIMSK1 |= (1 << OCIE1B);  /** enable Reset ISR */
  
  isBusy = false;
}



ISR(TIMER1_COMPB_vect) {
  /** Set stepper low */

  PORTD &= ~(1 << X_STEP_PIN); /* set pin 5 low*/


  TIMSK1 &= ~(1 << OCIE1B); // disable until activated by stepperISR
}

//   \((x-h)^{2}+(y-k)^{2}=r^{2}\)
