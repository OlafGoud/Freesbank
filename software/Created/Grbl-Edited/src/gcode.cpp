#include "gcode.h"

gc_data blockData;
gc_modes state;

//TODO: return message

void executeGcodeLine(char *line, int length) {
  //printString("executegcodeline\n");
  uint8_t axisCommand = AXIS_COMMAND_NONE;
  memset(&blockData, 0, sizeof(gc_data));
  memcpy(&blockData.mode, &state, sizeof(gc_modes));
  blockData.feedrate = SYSTEM_MAX_SPEED;

  float value;
  uint8_t charIndex = 0;
  uint8_t intValue;
  uint16_t mantissa;

  while(charIndex < length - 1) {
    printString("index: "); printUint8Base10(charIndex); printString(", length: "); printUint8Base10(length); uartWrite('\n');

    if(line[charIndex] == 32) {
      charIndex++;
    }
    char letter = line[charIndex];
    printString("letter: "); uartWrite(letter); printString(", integer: ");
    
    if(letter < 'A' && letter > 'Z') {
      printString("Error: not a letter");
      return; //TODO: message letter expected
    }
    charIndex++;

    if(!readFloat(line, &charIndex, &value)) {
      printString("Error: not a number");

      return; //TODO message value expected
    }

    intValue = trunc(value);
    printInteger(value);
    printString("\n");
    mantissa = round(100*(value - intValue)); 
    switch (letter) {
    case 'G':
      printString("command: G\n");
    
      switch (intValue)
      {
      case 0: case 1: case 2: case 3:
        blockData.motion = intValue;



        break;
      default:
        //not suported codes
        break;
      }

      break;
    case 'M':
      
      switch (intValue) {
      case 0: blockData.mode.program_flow = 0; break;
      
      default:
        break;
      }

      break;
    default:
      switch (letter)
      { // M and G are reserverd for gcode commands.
      case 'A': if(N_AXIS > A_AXIS) { blockData.endPos[A_AXIS] = value; } else { break; /*not suported*/} break; 
      case 'B': if(N_AXIS > B_AXIS) { blockData.endPos[B_AXIS] = value; } else { break; /*not suported*/} break; 
      case 'C': break; //not suported
      case 'D': break; //not suported
      case 'E': break; //not suported
      case 'F': blockData.feedrate = value; break;
      case 'H': break; //not suported
      case 'I': blockData.offset[X_AXIS] = value; break; 
      case 'J': blockData.offset[Y_AXIS] = value; break;
      case 'K': blockData.offset[Z_AXIS] = value; break;
      case 'L': break; //not suported
      case 'N': break; //not suported
      case 'O': break; //not suported
      case 'P': break; //not suported
      case 'Q': break; //not suported
      case 'R': blockData.arcRadius = value; break;
      case 'S': break; //not suported
      case 'T': break; //not suported
      case 'U': break; //not suported
      case 'V': if(value > 255) { /* bigger than 25.5 cm - ERR */ break; } blockData.mode.tool_length = round(value); break; // length off tool (spindle cnc)
      case 'W': if(value > 255) { /* bigger than 25.5 cm - ERR */ break; } blockData.mode.tool_diameter = round(value); break; // diameter of tool (spindle cnc)
      case 'X': if(N_AXIS > X_AXIS) { blockData.endPos[X_AXIS] = value; } else { break; /*not suported*/} break; 
      case 'Y': if(N_AXIS > Y_AXIS) { blockData.endPos[Y_AXIS] = value; } else { break; /*not suported*/} break; 
      case 'Z': if(N_AXIS > Z_AXIS) { blockData.endPos[Z_AXIS] = value; } else { break; /*not suported*/} break; 
      default:
        break; // not suported
      }
      break;
    }
  }
  printString("executemovementline\n");
  executeMovementLine(&blockData);  

}

