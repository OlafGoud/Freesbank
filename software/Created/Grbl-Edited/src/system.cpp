#include "system.h"
#include "string.h" /** for memcpy */

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
    if(c == ' ') continue;       /** filter spaces */
    if(lineComment) {
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
          print("<"); print(statusToString(systemState)); print("|");
          print(val); print("|");
          print(codeBlockBuffer.block[codeBlockBuffer.tail].endPos); print("|"); print(codeBlockBuffer.block[codeBlockBuffer.tail].beginPos); println(">");

        } else if (line[1] == 'D') {
          /** @todo Debug + docu */
          //print("<"); print(encoderSteps); print(targetStep); 
        }


        /** @todo handle commands (I (info about system), G (print settings), $ (???)) */
        /** @fn funciton */
      } else if (line[0] == 'M' || line[0] == 'G') {
        if(readGCodeLine(line, size)) {
          println(OK_MESSAGE);
        }
      } else {
        println("ERR");
      }

      size = 0;
      line[0] = '\0';

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
  size = 0;
}

CodeBlockBuffer codeBlockBuffer{};


/**
 * function that processes gcode commands to be used by the rest.
 * @param line char* line of information
 * @param size size of the effective data.
 */
bool readGCodeLine(char* line, uint8 size) {
  static float prevPos[3] = {0, 0, 0};
  static uint8 positionMode = 0; /** 0 = absolute, 1 = incrementeel */
  CodeBlock codeBlock;

  float floatValue;
  int32 intValue;
  uint16 mantissa; /** used for commands like GX.X (G28.1) @note not used for now */
  uint8 charIndex = 0;

  while(charIndex < size - 1) {
    
    char letter = line[charIndex++];
    if(letter < 'A' && letter > 'Z') {
      println("ERR: !LETTER");
      return false;
    }

    /** reading float */
    char* end;
    floatValue = strtof(line + charIndex, &end) ;/** not tested */
    if(end != line + charIndex) {
      charIndex = end - line; /** conversion finished and update current index */
    } else {
      println("ERR: !NUMBER");
      return false;
    }

    intValue = trunc(floatValue);
    mantissa = round(100 * (floatValue - intValue)); /** @note get the value after the '.' for commands like: GXX.X */

    if(letter == 'G') {
      codeBlock.letter = 'G';
      codeBlock.command = intValue;
      switch (intValue) {
      case 0: case 1: case 2: case 3: /** movement commandos */
        memcpy(codeBlock.beginPos, prevPos, sizeof(prevPos));
        break;
      case 17: case 18: case 19: /** plane switch commandos */
        codeBlock.subCommand = intValue - 17;
        break;
      case 90: case 91: /** absolute mode and incremental mode */
        positionMode = (bool)(intValue - 90); 
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
      case 0: break;
      case 1: 
        /** STOP NOW @todo */
        systemState = ERROR;
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
    case 'E': codeBlock.E = floatValue; break; /** @note Speed of spindle?? Not suported (YET) */
    case 'F': codeBlock.F = floatValue; break; /** @note not suported Feedrate */
    case 'H': break; /** @todo @note Not suported (YET) */
    case 'I': codeBlock.I = floatValue; break; /** @note circle offset X */
    case 'J': codeBlock.J = floatValue; break; /** @note circle offset Y */
    case 'K': break; /** @todo @note Not suported (YET) */
    case 'L': break; /** @todo @note Not suported (YET) */
    case 'N': break; /** @todo @note Not suported (YET) */
    case 'O': /*encoderSteps[0] = intValue; println("encoder steps to value"); */break; /** @todo @note Not suported (YET) */
    case 'P': break; /** @todo @note Not suported (YET) */
    case 'Q': break; /** @todo @note Not suported (YET) */
    case 'R': codeBlock.R = floatValue; break; /** @note Radius for circle */
    case 'S': break; /** @todo @note Not suported (YET) */
    case 'T': break; /** @todo @note Not suported (YET) */
    case 'U': break; /** @todo @note Not suported (YET) */
    case 'V': break; /** @todo @note Not suported (YET) */
    case 'W': break; /** @todo @note Not suported (YET) */
    case 'X': codeBlock.endPos[X_AXIS] = floatValue; break; /** @note |                                 */
    case 'Y': codeBlock.endPos[Y_AXIS] = floatValue; break; /** @note |> stored as a 3 component float  */
    case 'Z': codeBlock.endPos[Z_AXIS] = floatValue; break; /** @note |                                 */
    
    default:
      break; /** LETTER NOT SUPORTED */
    }  
  }

  if(positionMode == 1) { /** operating in incemental mode */
    codeBlock.endPos[X_AXIS] += prevPos[X_AXIS];
    codeBlock.endPos[Y_AXIS] += prevPos[Y_AXIS];
    codeBlock.endPos[Z_AXIS] += prevPos[Z_AXIS];
  }


  if(codeBlock.command >= 0 && codeBlock.command <= 4) {
    memcpy(prevPos, codeBlock.endPos, sizeof(prevPos));
  }
  if(codeBlockBuffer.head == codeBlockBuffer.tail) {
    // full
    return false;
  }
  CodeBlock *block = &codeBlockBuffer.block[codeBlockBuffer.head];
  
  memcpy(block, &codeBlock, sizeof(codeBlock));
  codeBlockBuffer.head = (codeBlockBuffer.head + 1) % CODEBLOCKBUFFERSIZE;
  codeBlockBuffer.size++;
  return true;
}

char* statusToString(SystemState s) {
  if(s == SYSTEM_IDLE) {
    return "IDLE";
  } else if (s == SYSTEM_RUNNING) {
    return "RUNNING";
  } else if (s == SYSTEM_ERROR) {
    return "ERROR";
  } else if (s == SYSTEM_INTERNAL_ERROR_RESTART_REQUIRED) {
    return "RESTARTERROR";
  } else {
    return "ERR!";
  }
}