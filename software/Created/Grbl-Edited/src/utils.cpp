#include "utils.h"

/**
 * 
 */
uint8_t readFloat(char *line, uint8_t *char_counter, float *float_ptr) {

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
