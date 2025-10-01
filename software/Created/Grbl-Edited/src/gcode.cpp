#include "gcode.h"
#include "string.h"

gc_data blockData;
gc_modes state;


//TODO: return message

void executeGcodeLine(char *line, int length) {
  uint8_t axisCommand = AXIS_COMMAND_NONE;
  memset(&blockData, 0, sizeof(gc_data));
  memcpy(&blockData.mode, &state, sizeof(gc_modes));


  float value;
  uint8_t charIndex = 0;
  uint8_t intValue;
  uint16_t mantissa;

  while(charIndex < length) {
    char letter = line[charIndex];
    if(letter < 'A' && letter > 'Z') {
      return; //TODO: message letter expected
    }
    charIndex++;

    if(!read_float(line, &charIndex, &value)) {
      return; //TODO message value expected
    }

    intValue = trunc(value);
    mantissa = round(100*(value - intValue)); 
    switch (letter) {
    case 'G':
      
      switch (intValue)
      {
      case 0: case 1: case 2: case 3:
        blockData.mode.motion = intValue;



        break;
      default:
        //not suported codes
        break;
      }

      break;
    case 'M':
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
      case 'V': if(value > 255) { /*bigger than 25.5 cm - ERR*/ break; } blockData.mode.tool_length = round(value); break; // length off tool (spindle cnc)
      case 'W': if(value > 255) { /*bigger than 25.5 cm - ERR*/ break; } blockData.mode.tool_diameter = round(value); break; // diameter of tool (spindle cnc)
      case 'X': if(N_AXIS > X_AXIS) { blockData.endPos[X_AXIS] = value; } else { break; /*not suported*/} break; 
      case 'Y': if(N_AXIS > Y_AXIS) { blockData.endPos[Y_AXIS] = value; } else { break; /*not suported*/} break; 
      case 'Z': if(N_AXIS > Z_AXIS) { blockData.endPos[Z_AXIS] = value; } else { break; /*not suported*/} break; 
      default:
        break; // not suported
      }
      break;
    }



  }


}













// Extracts a floating point value from a string. The following code is based loosely on
// the avr-libc strtod() function by Michael Stumpf and Dmitry Xmelkov and many freely
// available conversion method examples, but has been highly optimized for Grbl. For known
// CNC applications, the typical decimal value is expected to be in the range of E0 to E-4.
// Scientific notation is officially not supported by g-code, and the 'E' character may
// be a g-code word on some CNC systems. So, 'E' notation will not be recognized.
// NOTE: Thanks to Radu-Eosif Mihailescu for identifying the issues with using strtod().
uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr) {

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
  uint32_t intval = 0;
  int8_t exp = 0;
  uint8_t ndigit = 0;
  bool isdecimal = false;
  while (1) {
    c -= '0';
    if (c <= 9) {
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS) {
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

