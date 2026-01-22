#include "stepper.h"

uint8 stepperState;

volatile static StepperData stepperData; 
volatile float currStepsPos[3];

void stepAxisFromStepVar(uint8 axisNumber, uint32 currentCycle, uint32 endCycle, volatile uint8* port, uint8 pin) {
  float val = getYForLine(currentCycle, 0, endCycle, stepperData.currentBlock->beginPos[axisNumber], stepperData.currentBlock->endPos[axisNumber]); 

  if(stepperData.formulaValues[axisNumber] <= 0) {
    if(stepperData.formulaValues[axisNumber] == 0) {
      return;
    }
    if(currStepsPos[axisNumber] < val) {
      return;
    }
    currStepsPos[axisNumber] -= STEPPER_ACCURACY;
  } else {
    if(currStepsPos[axisNumber] > val) {
      return;
    }
    currStepsPos[axisNumber] += STEPPER_ACCURACY;
  }
  print("CU:"); println(val, 2); println(currStepsPos[axisNumber], 2);
  *(port) |= (1 << pin);

}

/** 
 * Does all the calculations for 1 axis and steps accordingly.
 * 
 * This function gets a value from a linear line that plots from t=0 to t=end. the beginpos is the position at t=0 and increases linear to the endpos at t=end. 
 * By passing in the currentcycle you know the value that the currentposition needs to have. if it is lower than the desiredvalue the machine steps. else nothing happends.
 * 
 * @param axisNumber The number for the axis in vectors as a uint8(max 255).
 * @param currentCycle The current cycle of the segment.
 * @param endCycle The last cycle for the formula. 
 * @param currPos The current position to compare to.
 * @param port The IO port that the steppin is on.
 * @param pin The step pin for the axis.
 */
void stepAxisFromPos(uint8 axisNumber, uint32 currentCycle, uint32 endCycle, float currPos, volatile uint8* port, uint8 pin) {

  float val = getYForLine(currentCycle, 0, endCycle, stepperData.currentBlock->beginPos[axisNumber], stepperData.currentBlock->endPos[axisNumber]);  
  if(stepperData.formulaValues[axisNumber] <= 0) {
    if(stepperData.formulaValues[axisNumber] == 0) {
      return;
    }
    if(currPos < val) {
      println("w");
      return;
    }
  } else {
    if(currPos > val) {
      println("w");
      return;
    }
  }
  *(port) |= (1 << pin);
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




/** 
 * Function that gets the values from a linear formula by passing in the begin and end values of the formula and the current x value.
 * @example when you have P1(3; 0) and P2(40; 100) and want the y value for x=5: x=5, x1=0, x2=100, y1=3, y2=40.
 * @param x current x value to get the y value from.
 * @param x1 begin x value; begin of horizontal axis of formula
 * @param x2 end x value; end of horizontal axis of formula
 * @param y1 begin y value; end of vertical axis of formula
 * @param y2 end y value; end of vertical axis of formula
 * @returns the y value from the x value;
 */
float getYForLine(float x, float x1, float x2, float y1, float y2) {
  return (((y2 - y1)/(x2 - x1)) * (x - x1)) + y1;
}



/** 
 * Set timer1, 16bit, prescalers(1,8,64,256,1024)
 * give a target delay between interrupts in ms. the reset isr will always be 800us later, it will check if that is possible with the target ms.
 * @param targetms the target time between interrupts in miliseconds.
 * @returns 0 = all good, 1 = the delay is to high, 2 = reset isr delay is to low.
 * @note Only works on timer1 because of it being 16 bit and the other 8 bit.
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

  sei();

  return 0;
}




/**
 * Function to init the stepper motors.
 * Set the output for stepper pins defined in macros.h
 * Set ISR with STEPPER_ISR_MS out macros.h
 * Set reset ISR with RESET_STEPBITS_US out macros.h
 * resets stepperState to empty.
 * @note if there is not configuration for the stepper interval and the 800 us reset isr for it will set the systemstate to INTERNAL_ERROR_RESTART_REQUIRED.
 */
void initSteppers() {

  /** set outputs for pins */
  STEPPER_DIR_DDR |= (1 << X_DIR_PIN) | (1 << Y_DIR_PIN) | (1 << Z_DIR_PIN);
  STEPPER_STEP_DDR |= (1 << X_STEP_PIN) | (1 << Y_STEP_PIN) | (1 << Z_STEP_PIN);

  /**  */
  STEPPER_STEP_PORT &= ~(1 << X_STEP_PIN);
  STEPPER_STEP_PORT &= ~(1 << Y_STEP_PIN);
  STEPPER_STEP_PORT &= ~(1 << Z_STEP_PIN);

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
    systemState = INTERNAL_ERROR_RESTART_REQUIRED;
  }
  //print("StepperISRMS: "); println(STEPPER_ISR_MS, 1); // debug line
  stepperState = STEPPER_EMPTY;
}



/**
 * Loads a new segment in the stepper buffer. if there isn't a new block it will return and change the systemstate to IDLE.
 * It will exectue all things for each command.
 * 
 * Suported commands:
 *  1, 2
 *  
 * 
 * 
 * @todo controlling frees, feedrate.
 */
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

    if (stepperData.formulaValues[X_AXIS] < 0) {
      print("negx");
      STEPPER_DIR_PORT &= ~(1 << X_DIR_PIN);
    } else {
      STEPPER_DIR_PORT |= (1 << X_DIR_PIN);
    }
    if(stepperData.formulaValues[Y_AXIS] < 0) {
      print("negy");
      STEPPER_DIR_PORT &= ~(1 << Y_DIR_PIN);
    } else {
      STEPPER_DIR_PORT |= (1 << Y_DIR_PIN);
    }
    if(stepperData.formulaValues[Z_AXIS] > 0) {
      print("negy");
      STEPPER_DIR_PORT &= ~(1 << Z_DIR_PIN);
    } else {
      STEPPER_DIR_PORT |= (1 << Z_DIR_PIN);
    }


    float maxValue = abs(stepperData.formulaValues[X_AXIS]) > abs(stepperData.formulaValues[Y_AXIS]) ? abs(stepperData.formulaValues[X_AXIS]) : abs(stepperData.formulaValues[Y_AXIS]);
    maxValue = maxValue > abs(stepperData.formulaValues[Z_AXIS]) ? maxValue : abs(stepperData.formulaValues[Z_AXIS]);

    stepperData.modifier = (int)(maxValue / STEPPER_ACCURACY);
  }/* else if (block->command == 2 || block->command == 3) {
    stepperData.formulaValues[X_AXIS] = block->I;
    stepperData.formulaValues[Y_AXIS] = block->J;
    stepperData.formulaValues[Z_AXIS] = block->R;
  } */else if (block->command >= 17 || block->command <= 19) {
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



/** 
 * runs when a segment is done.
 * sets the stepperstete to empty,
 * turn off the interrupt for the steppers.
 */
void setSegmentDone() {
  stepperState = STEPPER_EMPTY; /** set the stepperstate to empty to get a new block */
  TIMSK1 &= ~(1 << OCIE1A); /** turn the stepper interrupt */
  /** Check if he is at the right place */
  println("DONE"); // debug print line
}



/** 
 * Checks if a axis has reached it end point.
 * @param axisNumber The number for the axis in vectors as a uint8(max 255).
 * @param currentPos The currentposition of the axis to compare to the end point.
 * @returns 0 if done and 1 if not done. 
 */
uint8 checkIfAxisNotDone(uint8 axisNumber, float currentPos) {
  if(stepperData.formulaValues[axisNumber] == 0) {
    return 0;
  }
  if(stepperData.formulaValues[axisNumber] < 0) {
    return currentPos > stepperData.currentBlock->endPos[axisNumber] ? 1 : 0;
  }
  return currentPos < stepperData.currentBlock->endPos[axisNumber] ? 1 : 0;
}



/** 
 * Main ISR, runs at intervalls defined in STEPPER_ISR_MS in macros.h. 
 * blocks a new interrupt if it fires before the previous interrupt is handled.
 * This ISR does all the movements for the steppers and activate the reset ISR.
 */
ISR(TIMER1_COMPA_vect) {
  /** variable to prevent rentering the loop until it is finished */
  static bool isBusy = false;
  
  if(isBusy) return; /** return if previous cycle is not done */

  isBusy = true; /** set busy */
  float currentPosition[ENCODERS_AXIS]{};
  getCurrentMMFromEncoders(currentPosition); /** get current encoder values */

  /** check if the timer has reached and check if the axis are at the right place. */ /** @note maybe add a -4 of time and slowly move to the target while counting down and checking? */
  if(stepperData.timerValue >= stepperData.modifier) {
    uint8 donecheck = 0;

    /****************************************************************************************************************************
     * @note add here axis that are used by the stepper motor.
     ***************************************************************************************************************************/
    
    donecheck += checkIfAxisNotDone(Z_AXIS, currentPosition[Z_AXIS]);

    /****************************************************************************************************************************
     * END for adding more steppers.
     ***************************************************************************************************************************/
    
     if(donecheck == 0) {
      println(currentPosition);
      setSegmentDone();
      isBusy = false;
      return;
    }
  }


  /****************************************************************************************************************************** 
   * @note add here axis to be used by the stepper motor. 
   * @note that they need to be added by the checker if they are done 
   *****************************************************************************************************************************/

  stepAxisFromStepVar(X_AXIS, stepperData.timerValue, stepperData.modifier, &STEPPER_STEP_PORT, X_STEP_PIN);
  stepAxisFromStepVar(Y_AXIS, stepperData.timerValue, stepperData.modifier, &STEPPER_STEP_PORT, Y_STEP_PIN);
  stepAxisFromPos(Z_AXIS, stepperData.timerValue, stepperData.modifier, currentPosition[Z_AXIS], &STEPPER_STEP_PORT, Z_STEP_PIN);

  /******************************************************************************************************************************
   * END for adding more steppers.
   *****************************************************************************************************************************/

  stepperData.timerValue >= stepperData.modifier ? stepperData.timerValue = stepperData.modifier : stepperData.timerValue++;

  /** set reset isr */
  TCNT1 = 0;                /** Set timer to 0 */
  TIMSK1 |= (1 << OCIE1B);  /** enable Reset ISR */
  isBusy = false;
}




/**
 * ISR to reset the stepperpins. Runs 800us after the main ISR.
 */
ISR(TIMER1_COMPB_vect) {
  /** Set stepper low */

  STEPPER_STEP_PORT &= ~(1 << X_STEP_PIN); /* set pin 5 low*/
  STEPPER_STEP_PORT &= ~(1 << Y_STEP_PIN);
  STEPPER_STEP_PORT &= ~(1 << Z_STEP_PIN);

  
  TIMSK1 &= ~(1 << OCIE1B); // disable until activated by stepperISR
}

//   \((x-h)^{2}+(y-k)^{2}=r^{2}\)
