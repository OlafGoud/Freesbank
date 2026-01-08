#include "system.h"
#include "string.h"

GCodeSettings gCodeSettings;

StepperBlock stepperBlock;
PlannerBuffer plannerBuffer;
volatile int32 steps = 0;
/**
 * Function to load 1 segement into the buffer for the stepper motors. Also execute all functions for other componements for this segment (like spindle, printer etc)
 * Load 1 segment into buffer to use for the stepper motors
 * @todo load segement
 * @todo spindle functions
 * @todo ( printer functions )
 */
void loadSegmentInStepperBuffer() {

  if(plannerBuffer.tail == plannerBuffer.head) {
    return;
  }
  uint8 idx = plannerBuffer.tail++;
  memcpy(stepperBlock.exitPos, plannerBuffer.block[idx].exitPoint, sizeof(stepperBlock.exitPos));
  stepperBlock.feedrate = plannerBuffer.block[idx].feedrate;
  stepperBlock.exitSpeed = plannerBuffer.block[idx].exitSpeed;

  targetStep[0] = static_cast<int32>(stepperBlock.exitPos[0]);
  stepperState = STEPPER_FULL;

}

//\((x-h)^{2}+(y-k)^{2}=r^{2}\)

/**
 * Function to get data from the RX buffer and make a line from it. And send the right commands to the right functions.
 * @todo system commands '$'
 * @todo status commands '?'
 * @todo gcode commands [all other]
 */
void readSerialLine() {
  static char* line = new char[MAX_LINE_SIZE];
  static uint8 size = 0;
  static bool lineComment = false;
  while(size < MAX_LINE_SIZE) {
    unsigned char c = uartRead();
    
    if(c == EMPTY_CHAR) return; /** No data on RX Line */
    //println("not empty");
    if(c == 32) continue;       /** filter spaces */
    if(lineComment == true) {
      if(c == ')') lineComment = false; /** comments off */
      continue; 
    }

    if(c == '\n' || c == '\0') {
      line[size++] = '\n';

      if(line[0] == '$' && size - 1 > 1) {

        if(line[1] == 'X') {
          encoderSteps[0] = 0; /** @todo change to struct with 3 values for 3 encoders */
          println("reset X encoder to 0");
        } else if (line[1] == 'Y') {
          /** @todo + docu */
        } else if (line[1] == 'Z') {
          /** @todo + docu */
        } else if (line[1] == '?') {

          print("<"); print(getStatus(systemState)); print("|");
          print(encoderSteps); print("|");
          print(stepperBlock.exitPos); print("|"); print(stepperBlock.feedrate, 1); println(">");

        } else if (line[1] == 'D') {
          /** @todo Debug + docu */
          print("<"); print(encoderSteps); print(targetStep); 
        }


        /** @todo handle commands (I (info about system), G (print settings), $ (???)) */
        /** @fn funciton */
      } else if (line[0] == '?') {
        print(steps);
      } else if (line[0] == ']') {
        stepperState = STEPPER_EMPTY;
      } else if (line[0] == 'M' || line[0] == 'G') {
        if(readGCodeLine(line, size)) {
          println(OK_MESSAGE);
        }
      } else {
        println("ERR");
      }

      size = 0;
      println("size reset!");
      line = new char[MAX_LINE_SIZE];
      continue;
    } else {
      if(c == '(') { /** comments on */
        lineComment = true;
        continue;
      } else if (c == ';') {
        /** rest of line comment @note not suported yet */
      } else {
        if(c >= 'a' && c <= 'z') c = c - 'a' + 'A'; /** change lowercase letters into capital leters */

        line[size++] = c;
      }
    }
  }

  /** when this is reached, the input lines are or to big, or something is not correct. */
  print("To big: ");
  println(size);
}


/**
 * function that processes gcode commands to be used by the rest.
 * @param line char* line of information
 * @param size size of the effective data.
 */
bool readGCodeLine(char* line, uint8 size) {
  
  GCodeBlock block;

  float value;
  int32 intValue;
  uint16 mantissa; /** used for commands like GX.X (G28.1) */
  int16 command = -1;
  uint8 charIndex = 0;

  while(charIndex < size - 1) {
    
    char letter = line[charIndex++];
    if(letter < 'A' && letter > 'Z') {
      println("ERR: !LETTER");
      return false;
    }
    if(!readFloat(line, &charIndex, &value)) {
      println("ERR: !NUMBER");
      return false;
    }
    intValue = trunc(value);
    mantissa = round(100 * (value - intValue)); /** @note get the value after the '.' for commands like: GXX.X */

    if(letter == 'G') {

      switch (intValue) {
      case 0: case 1: case 2: case 3:
        command = intValue;
        break;
      case 17: case 18: case 19:
        gCodeSettings.selectedPlane = intValue - 17;
        break;
      case 90: case 91: 
        gCodeSettings.absolute = (bool)(intValue - 90); 
        break;
      default:
        /** @note unsuported commands */
        print("ERR: UNSUPORTED G");
        println((int)intValue);
        return false;
      }
      continue;
    }
    
    if (letter == 'M') {
      switch (intValue) {
      case 1:
        /** STOP NOW @todo */
        break;
      
      default:
        break;
      }
      continue;
    }

    switch (letter) {
    case 'A': break; /** @todo @note reserverd for rotations??? */
    case 'B': break; /** @todo @note reserverd for rotations??? */
    case 'C': break; /** @todo @note Not suported (YET) */
    case 'D': break; /** @todo @note Not suported (YET) */
    case 'E': block.extrude = value; break; /** @note Speed of spindle?? Not suported (YET) */
    case 'F': block.feedrate = value; break; /** Feedrate */
    case 'H': break; /** @todo @note Not suported (YET) */
    case 'I': block.offset[0] = value; break; /** @note circle offset X |> stored as a 2 component float  */
    case 'J': block.offset[1] = value; break; /** @note circle offset Y |                                 */
    case 'K': break; /** @todo @note Not suported (YET) */
    case 'L': break; /** @todo @note Not suported (YET) */
    case 'N': break; /** @todo @note Not suported (YET) */
    case 'O': encoderSteps[0] = intValue; println("encoder steps to value"); break; /** @todo @note Not suported (YET) */
    case 'P': break; /** @todo @note Not suported (YET) */
    case 'Q': break; /** @todo @note Not suported (YET) */
    case 'R': block.radius = value; break; /** @note Radius for circle */
    case 'S': break; /** @todo @note Not suported (YET) */
    case 'T': break; /** @todo @note Not suported (YET) */
    case 'U': break; /** @todo @note Not suported (YET) */
    case 'V': break; /** @todo @note Not suported (YET) */
    case 'W': break; /** @todo @note Not suported (YET) */
    case 'X': block.position[0] = value; break; /** @note |                                 */
    case 'Y': block.position[1] = value; break; /** @note |> stored as a 3 component float  */
    case 'Z': block.position[2] = value; break; /** @note |                                 */
    
    default:
      break; /** LETTER NOT SUPORTED */
    }  
  }
  if(command == -1) return false;

  if(command >= 0 && command <= 1) {
    addSegmentToPlanBuffer(block);
  } else if(command >= 2 && command <= 3) {
    generateStraightSegementsFromCurve(block, gCodeSettings.selectedPlane);
  } else {
    return false;
  }


  /** @todo split movement in lines when curved, plan function */
  return true;
}


static float currPos[3] = {0.0f,0.0f,0.0f};

void addSegmentToPlanBuffer(GCodeBlock &gcode) {
  

  PlannerBlock *block = &plannerBuffer.block[plannerBuffer.head];
  memcpy(block->entryPoint, currPos, sizeof(block->entryPoint));
  memcpy(block->exitPoint, gcode.position, sizeof(block->exitPoint));

  block->feedrate = gcode.feedrate;

  float dx = block->entryPoint[0] - block->exitPoint[0]; 
  float dy = block->entryPoint[1] - block->exitPoint[1]; 
  float dz = block->entryPoint[2] - block->exitPoint[2]; 

  block->distance = sqrt(dx*dx + dy*dy + dz*dz);
  vecSub3(block->unitVector, block->entryPoint, block->exitPoint);
  vecNormalize3(block->unitVector);

  plannerBuffer.head = (plannerBuffer.head + 1) % MAX_PLANNER_BUFFER_SIZE;
  if (plannerBuffer.head == plannerBuffer.tail) return; /** Buffer full */

  memcpy(currPos, block->exitPoint, sizeof(currPos));
  
  plannerRecalculate(CORNER_DIVIDATION);
}


void plannerRecalculate(float J) {
  if(plannerBuffer.head == plannerBuffer.tail) {
    plannerBuffer.block[plannerBuffer.head].exitSpeed = 0.0f;
    return;
  }

  /** set all speeds to feedrate */
  uint8 idx = plannerBuffer.tail;
  while(idx != plannerBuffer.head) {
    plannerBuffer.block[idx].entrySpeed = plannerBuffer.block[idx].feedrate;
    plannerBuffer.block[idx].exitSpeed = plannerBuffer.block[idx].feedrate;
    idx = (idx + 1) % MAX_PLANNER_BUFFER_SIZE;
  }

  /** forward checking */
  idx = plannerBuffer.tail;
  while(idx != plannerBuffer.head) {
    int prev = (idx - 1 + MAX_PLANNER_BUFFER_SIZE) % MAX_PLANNER_BUFFER_SIZE;

    float maxJunctionSpeed = junctionSpeedFromDeviation(plannerBuffer.block[prev].unitVector, plannerBuffer.block[idx].unitVector, J, SYSTEM_MAX_ACCEL);

    if(plannerBuffer.block[idx].entrySpeed > maxJunctionSpeed) plannerBuffer.block[idx].entrySpeed = maxJunctionSpeed;
    if(plannerBuffer.block[prev].exitSpeed > maxJunctionSpeed) plannerBuffer.block[prev].exitSpeed = maxJunctionSpeed; 

    idx = (idx + 1) % MAX_PLANNER_BUFFER_SIZE;
  }

  /** backwards checking */
  bool changes = true;
  int iter = 0;
  while (changes && iter < PLANNER_MAX_ITERATIONS) {
    changes = false;
    int i = (plannerBuffer.head - 1 + MAX_PLANNER_BUFFER_SIZE) % MAX_PLANNER_BUFFER_SIZE;

    while (!(i == plannerBuffer.tail)) {
      int prev = (i - 1 + MAX_PLANNER_BUFFER_SIZE) % MAX_PLANNER_BUFFER_SIZE;

      PlannerBlock *B = &plannerBuffer.block[i];
      PlannerBlock *P = &plannerBuffer.block[prev];

      float maxEntryFromPrev = sqrt(fmax(0.0f, P->exitSpeed * P->exitSpeed + 2.0f * SYSTEM_MAX_ACCEL * P->distance));
      if(B->entrySpeed > maxEntryFromPrev) {
        B->entrySpeed = maxEntryFromPrev;
        changes = true;
      }

      if(P->exitSpeed > B->entrySpeed) {
        P->exitSpeed = B->entrySpeed;
        changes = true;
      }
      i = prev;
    }
    iter++;
  }

  /** set last block to have a exit speed of 0 */
  int last = (plannerBuffer.head - 1 + MAX_PLANNER_BUFFER_SIZE) % MAX_PLANNER_BUFFER_SIZE;
  plannerBuffer.block[last].exitSpeed = 0.0f;
  print("tail: "); print(plannerBuffer.tail); print(", head: "); println(plannerBuffer.head);
  for(int i = plannerBuffer.tail; i != plannerBuffer.head; i = (i + 1) % MAX_PLANNER_BUFFER_SIZE) {
    printHline(10);
    print(plannerBuffer.block[i].exitPoint[0], 2); print(", "); print(plannerBuffer.block[i].exitPoint[1], 2); print(", "); println(plannerBuffer.block[i].exitPoint[2], 2); 
    print(plannerBuffer.block[i].entrySpeed, 2); print(", "); println(plannerBuffer.block[i].exitSpeed, 2);
    println(plannerBuffer.block[i].feedrate, 2);
  }
  printHline(10);

}

bool readFloat(char *line, uint8 *char_counter, float *float_ptr) {

  char *ptr = line + *char_counter;
  unsigned char c;

  // Grab first character and increment pointer. No spaces assumed in line.
  c = *ptr++;

  // Capture initial positive/minus character
  bool isnegative = false;
  if (c == '-') {
    isnegative = true;
    c = *ptr++;
  }
  else if (c == '+') {
    c = *ptr++;
  }

  // Extract number into fast integer. Track decimal in terms of exponent value.
  uint32 intval = 0;
  int8 exp = 0;
  uint8 ndigit = 0;
  bool isdecimal = false;
  while (1) {
    c -= '0';
    if (c <= 9) {
      ndigit++;
      if (ndigit <= 10) { /** max int digits */
        if (isdecimal) {
          exp--;
        }
        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
      }
      else {
        if (!(isdecimal)) {
          exp++;
        } // Drop overflow digits
      }
    }
    else if (c == (('.' - '0') & 0xff) && !(isdecimal)) {
      isdecimal = true;
    }
    else {
      break;
    }
    c = *ptr++;
  }

  // Return if no digits have been read.
  if (!ndigit) {
    return (false);
  };

  // Convert integer into floating point.
  float fval;
  fval = (float)intval;

  // Apply decimal. Should perform no more than two floating point multiplications for the
  // expected range of E0 to E-4.
  if (fval != 0) {
    while (exp <= -2) {
      fval *= 0.01;
      exp += 2;
    }
    if (exp < 0) {
      fval *= 0.1;
    }
    else if (exp > 0) {
      do {
        fval *= 10.0;
      } while (--exp > 0);
    }
  }

  // Assign floating point value with correct sign.
  if (isnegative) {
    *float_ptr = -fval;
  }
  else {
    *float_ptr = fval;
  }
  *char_counter = ptr - line - 1; // Set char_counter to next statement

  return (true);
}


/** @todo */
/**
 * generate segments for curves
 * the offset is absolute
 * @param block GCodeBlock what stores all important info.
 * @param selectedPlane plane that is selected to do the curve (XY, ZX, YZ).
 * @todo 
 */
void generateStraightSegementsFromCurve(GCodeBlock &block, uint8 selectedPlane) {

  float centerAxis0 = block.offset[0];
  float centerAxis1 = block.offset[1];

}


/**
 * calculates the maximum junctionspeed from deviation with unit vectors.
 * 
 * @param u const float u[3] -> unit vector prev block.
 * @param v const float v[3] -> unit vector current block.
 * @param J float J -> dividation that is allowed in the corner smaller = more accurate.
 * @param a float a -> maxmimal system acceleration.
 * 
 * @return float - junction speed.
 */
float junctionSpeedFromDeviation(const float u[3], const float v[3], float J, float a) {
  float cos_theta = vecDot3(u, v);

  if(cos_theta > EPS_COS_STRAIGHT) {
    return __FLT_MAX__;
  }


  float numerator = a * J * (1.0f + cos_theta);
  float denom = (1.0f - cos_theta);
  if(denom <= 0.0f) {
    return 0.0f;
  }

  float vj_sq = numerator / denom;
  if(vj_sq <= 0.0f) {
    return 0.0f;
  }

  return sqrtf(vj_sq);
}



/**
 * Vector function for function: junctionSpeedFromDeviation. 
 * 
 * @param a const float a[3] -> vector of floats 
 * @param b const float b[3] -> vector of floats
 * @return angle between vectors
 */
float vecDot3(const float a[3], const float b[3]) {
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}


/**
 * Vector function for function: planBufferLine. Gives the displacement.
 *  
 * @param r float r[3] -> refrence to return vector
 * @param a float a[3] -> vector of floats
 * @param b float b[3] -> vector of floats
 */
void vecSub3(float r[3], const float a[3], const float b[3]) {
  r[0] = a[0]-b[0]; r[1] = a[1]-b[1]; r[2] = a[2]-b[2];
}

/**
 * Vector function for function: vecNormalize3. It calculates the length of the vector
 * 
 * @param a const float a[3] -> vector of floats to get length of.
 * @return float - lenght of vector
 */
float vecNorm3(const float a[3]) {
  return sqrtf(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
}

/**
 * Vector function for function: planBufferLine. It calculates the unit vector.
 * 
 * @param r float r[3] -> refrence to vector to change.
 */
void vecNormalize3(float r[3]) {
  float n = vecNorm3(r);
  if (n > 0.0f) { r[0]/=n; r[1]/=n; r[2]/=n; }
}

void setEncoderInterupts() {

  DDRD &= ~((1 << PD2) | (1 << PD3)); /** Set pins to input */

  PORTD |= (1 << PD2); /** pullup resistor pin 2 */ 
  PORTD |= (1 << PD3); /** pullup resistor pin 3 */

  /** onchange interupt pin 2 */
  EICRA |= (1 << ISC00);
  EICRA &= ~(1 << ISC01);

  /** onchange interupt pin 3 */
  EICRA |= (1 << ISC10);
  EICRA &= ~(1 << ISC11);

  /** enable  */
  EIMSK |= (1 << INT0) | (1 << INT1);

}

/*****
 * encoder interupts. will be called on change for each interrupt.
 */

/**
 * vec3 float for (x,y,z) axis encoders
 */
volatile int32 encoderSteps[3] {};

/***
 * Encoder state
 */
volatile uint8 encoderState = 0;

/** Interupt for pin 2 */
ISR(INT0_vect) {

  /** reading from pins
   * 
   * arduino uno has interupt pins 2 & 3. from that we calculate the current position in the cycle. 
   * 0 & 0 = 0
   * 1 & 0 = 1
   * 1 & 1 = 2
   * 0 & 1 = 3
   */
  uint8 b = (PIND >> PD3) & 1;
  uint8 curr = (((PIND >> PD2) & 1) ^ b) | (b << 1); 

  /** check the difference from the current encoder state. */
  uint8 diff = (curr  - encoderState) & 3;
  int8 delta = (diff == 1) - (diff == 3);

  encoderSteps[0] += delta;
  encoderState = curr;
}


/** Interupt for pin 3
 * Exact same function as above
 */
ISR(INT1_vect) {

  uint8 b = (PIND >> PD3) & 1;
  uint8 curr = (((PIND >> PD2) & 1) ^ b) | (b << 1); 

  uint8 diff = (curr  - encoderState) & 3;
  int8 delta = (diff == 1) - (diff == 3);

  encoderSteps[0] += delta;
  encoderState = curr;
}

/*****
 * end encoder interupts
 */


/** 
 * targetStep
 */
volatile int32 targetStep[3] {};


/***************************
 * 
 * stepper interupts
 * 
 * 
 */

void setStepperInterupts() {
  cli();

  // set pins to output
  DDRD |= (1 << PD5) | (1 << PD6); 
  PORTD |= (1 << PD6); // set pin 6 to 1;
  TCCR1A = 0;
  TCCR1B = 0;

  // prescaler 8 → Timer1 tick = 0.5 µs
  TCCR1B |= (1 << WGM12);      // CTC mode
  TCCR1B |= (1 << CS11);       // prescaler = 8

  // INT1 every 10ms:
  // 10,000 µs / 0.5 = 20,000 ticks
  OCR1A = 20000;

  TIMSK1 |= (1 << OCIE1A);     // enable compare A interrupt
  sei();

}

ISR(TIMER1_COMPA_vect) {
  /** call stepper */
  steps++;
  PORTD |= (1 << PD5); /* set pin 5 high*/
  

  /** set second isr */
  OCR1B = TCNT1 + (800 * 2); // 0.5us per tick at prescale=8 (800 = 800 microseconds (us))
  TIMSK1 |= (1 << OCIE1B);             // enable compare B interrupt
}

ISR(TIMER1_COMPB_vect) {
  /** Set stepper low */

  PORTD &= ~(1 << PD5); /* set pin 5 low*/


  TIMSK1 &= ~(1 << OCIE1B);            // disable until next cycle
}


void setDirection() {

  static bool on = true;


  if(encoderSteps[0] <= targetStep[0] + 4 && encoderSteps[0] >= targetStep[0] - 4) {
    TIMSK1 &= ~(1 << OCIE1A);   // disable Timer1 compare A interrupt
    on = false;
    stepperState = STEPPER_EMPTY;
    return;
  }
  if(on == false) {
    TIMSK1 |= (1 << OCIE1A);    // enable Timer1 compare A interrupt
    on = true;
  }

  if(encoderSteps[0] > targetStep[0]) {
    PORTD &= ~(1 << PD6);
  } else if (encoderSteps[0] < targetStep[0]) {
    PORTD |= (1 << PD6);
  }
}


char* getStatus(int s) {
  if(s == IDLE) {
    return "IDLE";
  } else if (s == RUNNING) {
    return "RUNNING";
  } else if (s == ERROR) {
    return "ERROR";
  } else if (s == INTERNAL_ERROR_RESTART_REQUIRED) {
    return "RESTARTERROR";
  } else {
    return "ERR!";
  }
}