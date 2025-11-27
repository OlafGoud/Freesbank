#include "system.h"
#include "string.h"
GCodeSettings gCodeSettings;

/**
 * Function to load 1 segement into the buffer for the stepper motors. Also execute all functions for other componements for this segment (like spindle, printer etc)
 * Load 1 segment into buffer to use for the stepper motors
 * @todo load segement
 * @todo spindle functions
 * @todo ( printer functions )
 */
void loadSegmentInStepperBuffer() {

}

/**
 * Function to get data from the RX buffer and make a line from it. And send the right commands to the right functions.
 * @todo system commands '$'
 * @todo status commands '?'
 * @todo gcode commands [all other]
 */
void readSerialLine() {
  static char* line = new char[MAX_LINE_SIZE];
  static uint8_t size = 0;
  static bool lineComment = false;

  while(size < MAX_LINE_SIZE) {
    char c = uartRead();
    
    if(c == EMPTY_CHAR) return; /** No data on RX Line */
    if(c == 32) continue;       /** filter spaces */

    if(lineComment == true) {
      if(c == ')') lineComment == false; /** comments off */
      continue; 
    }

    if(c == '\n' || c == '\0') {
      line[size++] = '\n';

      if(line[0] == '$' && size > 1) {
        /** @todo handle commands (I (info about system), G (print settings), $ (???)) */
        /** @fn funciton */
      } else if (line[0] == '?') {
        /** @todo print status */
      } else {
        readGCodeLine(line, size);
      }

      size = 0;
      line = new char[MAX_LINE_SIZE];
      continue;
    } else {
      if(c == '(') { /** comments on */
        lineComment = true;
        continue;
      } else if (c == ';') {
        /** ? */
      } else {
        if(c >= 'a' && c <= 'z') c = c - 'a' + 'A'; /** change lowercase letters into capital leters */

        line[size++] = c;
      }
    }
    
  }

  /** when this is reached, the input lines are or to big, or something is not correct. */
  println(size);
}


/**
 * function that processes gcode commands to be used by the rest.
 * @param line char* line of information
 * @param size size of the effective data.
 */
void readGCodeLine(char* line, uint8_t size) {
  
  GCodeBlock block;

  float value;
  uint32_t intValue;
  uint16_t mantissa;
  uint16_t command;
  uint8_t charIndex = 0;

  while(charIndex < size - 1) {
    
    char letter = line[charIndex++];
    if(letter < 'A' && letter > 'Z') {
      println("ERR: !LETTER");
      return;
    }
    if(!readFloat(line, &charIndex, &value)) {
      println("ERR: !NUMBER");
      return;
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
        return;
      }
      continue;
    }
    
    if (letter == 'M') {

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
    case 'O': break; /** @todo @note Not suported (YET) */
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

  if(command >= 0 && command <= 1) {
    addSegmentToPlanBuffer(block);
  }
  if(command >= 2 && command <= 3) {
    generateStraightSegementsFromCurve(block, gCodeSettings.selectedPlane);
  }


  /** @todo split movement in lines when curved, plan function */
  return;
}


PlannerBuffer plannerBuffer;
static float currPos[3] = {0,0,0};

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

    
}


void plannerRecalculate(float J) {
  if(plannerBuffer.head == plannerBuffer.tail) {
    plannerBuffer.block[plannerBuffer.head].exitSpeed = 0.0f;
    return;
  }

  /** set all speeds to feedrate */
  uint8_t idx = plannerBuffer.tail;
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

}

/**
 * generate segments for curves
 * the offset is absolute
 * @param block GCodeBlock what stores all important info.
 * @param selectedPlane plane that is selected to do the curve (XY, ZX, YZ).
 */
void generateStraightSegementsFromCurve(GCodeBlock &block, uint8_t selectedPlane) {

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
