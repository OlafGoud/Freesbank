#include "system.h"
#include "string.h"

GCodeSettings gCodeSettings;

volatile int32 steps = 0;


/**
 * Function to get data from the RX buffer and make a line from it. And send the right commands to the right functions.
 * @todo system commands '$'
 * @todo status commands '?'
 * @todo gcode commands [all other]
 */
void readSerialLine() {
  static char line[MAX_LINE_SIZE];
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
          println("reset X encoder to 0");
        } else if (line[1] == 'Y') {
          /** @todo + docu */
        } else if (line[1] == 'Z') {
          /** @todo + docu */
        } else if (line[1] == '?') {
          float val[3]{};
          getCurrentMMFromEncoders(val);
          print("<"); print(getStatus(systemState)); print("|");
          print(val); print("|");
          print(codeBlockBuffer.block[codeBlockBuffer.tail].endPos); print("|"); print(codeBlockBuffer.block[codeBlockBuffer.tail].beginPos); println(">");

        } else if (line[1] == 'D') {
          /** @todo Debug + docu */
          //print("<"); print(encoderSteps); print(targetStep); 
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
      for(int i = 0; i < MAX_LINE_SIZE; i++) {
        line[i] = '\0';
      }

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

CodeBlockBuffer codeBlockBuffer{};


/**
 * function that processes gcode commands to be used by the rest.
 * @param line char* line of information
 * @param size size of the effective data.
 */
bool readGCodeLine(char* line, uint8 size) {
  static float prevPos[3] = {0, 0, 0};
  CodeBlock codeBlock;

  float value;
  int32 intValue;
  uint16 mantissa; /** used for commands like GX.X (G28.1) */
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
      codeBlock.letter = 'G';
      codeBlock.command = intValue;
      switch (intValue) {
      case 0: case 1: case 2: case 3:
        memcpy(codeBlock.beginPos, prevPos, sizeof(prevPos));
        break;
      case 17: case 18: case 19:
        codeBlock.subCommand = intValue - 17;
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
    case 'E': codeBlock.E = value; break; /** @note Speed of spindle?? Not suported (YET) */
    case 'F': codeBlock.F = value; break; /** Feedrate */
    case 'H': break; /** @todo @note Not suported (YET) */
    case 'I': codeBlock.I = value; break; /** @note circle offset X |> stored as a 2 component float  */
    case 'J': codeBlock.J = value; break; /** @note circle offset Y |                                 */
    case 'K': break; /** @todo @note Not suported (YET) */
    case 'L': break; /** @todo @note Not suported (YET) */
    case 'N': break; /** @todo @note Not suported (YET) */
    case 'O': /*encoderSteps[0] = intValue; println("encoder steps to value"); */break; /** @todo @note Not suported (YET) */
    case 'P': break; /** @todo @note Not suported (YET) */
    case 'Q': break; /** @todo @note Not suported (YET) */
    case 'R': codeBlock.R = value; break; /** @note Radius for circle */
    case 'S': break; /** @todo @note Not suported (YET) */
    case 'T': break; /** @todo @note Not suported (YET) */
    case 'U': break; /** @todo @note Not suported (YET) */
    case 'V': break; /** @todo @note Not suported (YET) */
    case 'W': break; /** @todo @note Not suported (YET) */
    case 'X': codeBlock.endPos[X_AXIS] = value; break; /** @note |                                 */
    case 'Y': codeBlock.endPos[Y_AXIS] = value; break; /** @note |> stored as a 3 component float  */
    case 'Z': codeBlock.endPos[Z_AXIS] = value; break; /** @note |                                 */
    
    default:
      break; /** LETTER NOT SUPORTED */
    }  
  }

  if(codeBlock.command >= 0 && codeBlock.command <= 4) {
    memcpy(prevPos, codeBlock.endPos, sizeof(prevPos));
  }
  print(codeBlock.endPos[0], 2);
  if(codeBlockBuffer.head == codeBlockBuffer.tail) {
    // full
    return false;
  }
  CodeBlock *block = &codeBlockBuffer.block[codeBlockBuffer.head];
  
  memcpy(block, &codeBlock, sizeof(codeBlock));
  codeBlockBuffer.head = (codeBlockBuffer.head + 1) % CODEBLOCKBUFFERSIZE;
  codeBlockBuffer.size++;
  print("done");
  /** @todo split movement in lines when curved, plan function */
  return true;
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