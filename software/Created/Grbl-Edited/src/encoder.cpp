#include "encoder.h"



static volatile int32 encoderSteps[ENCODERS_AXIS]{};
static float accuracy[ENCODERS_AXIS]{};

void getCurrentMMFromEncoders(float* encoderValues) {
  for(int i = 0; i < ENCODERS_AXIS; i++) {
    encoderValues[i] = encoderSteps[i] * accuracy[i];
  }
}

void setAccuracy() {
  accuracy[0] = 0.016f;
  accuracy[1] = 0.016f;
  accuracy[2] = 0.016f;
}

/****************************************************************************************************************************
 *  
 * X Axis
 * 
 ***************************************************************************************************************************/

/**
 * Encoder X interrupt function.
 * not in ISR because it needs to be called to initilize.
 */
static inline void readEncoderX(void) {
  static uint8 encoderStateX = 0;

  /** Get value of encoder pin b and masks everything but the lowest bit*/
  uint8 b = (PIND >> ENCODER_PIN_XB) & 1;
  //PORTB ^= (1 << PB5); 
  //println(encoderSteps[0]);
  /** 
   * Read and mask value of encoder pin A. and convert it to 'binary graycode' (only 1 changing bit each time)
   * 0 & 0 = 0
   * 1 & 0 = 1
   * 1 & 1 = 2
   * 0 & 1 = 3
   */
  uint8 curr = (((PIND >> ENCODER_PIN_XA) & 1) ^ b) | (b << 1); 

  /** 
   * Calculate movement direction using the the current - previous. (&3 is a bitwise modulo (%4) to avoid dividing)
   * possible values: 0 - 3
   * 0 = no movement;
   * 1 = step forward;
   * 2 = error (invalid or skiped step); (ignored)
   * 3 = step backwards;
   */
  uint8 diff = (curr - encoderStateX) & 3;

  /** if forward add 1, if backward subtract 1. 
   * if diff = 1 the first part is true and it adds 1. second part is false so does not do anything
   * if diff = 3 the second part is true and it adds -1. first part is false so does not do anything
   */
  encoderSteps[Z_AXIS] += (diff == 1) - (diff == 3); 
  
  /** Save current state */
  encoderStateX = curr;
}


/**
 * Encoder interrupts for X axis. 
 * @note that PCINT2_vect activates when pin A or B activates.
 */
ISR(PCINT2_vect) {
  readEncoderX();
}


void initEncoder() {

  /**
   * set the accuracy for the encoders
   * @todo calibrate (side to side?)
   */
  setAccuracy();

  #ifdef ARDUINO_UNO
  /** other system than on mega. this uses PCINT and mega uses INTX (both are available on both) */
  /** X AXIS */
  //ENCODER_DDR_X |= (1 << PB5);
  /** Encoder interrupt setup */
  /** Set pins for encoder to input */
  ENCODER_DDR_X &= ~((1 << ENCODER_PIN_XA) | (1 << ENCODER_PIN_XB)); 
  
  /** Set pullup resistors */
  ENCODER_PORT_X |= ((1 << ENCODER_PIN_XA) | (1 << ENCODER_PIN_XB));

  /** enable interrupt on PCINT2 GROUP for when either of A or B changes */
  PCICR |= (1 << PCIE2);

  /** Set interupt on A and B pins */
  PCMSK2 |= (1 << ENCODER_PIN_XA) | (1 << ENCODER_PIN_XB);
  #endif

  #ifdef ARDUINO_MEGA

  /** Set inputs */
  ENCODER_DDR_XY &= ~((1 << ENCODER_PIN_XA) | (1 << ENCODER_PIN_XB) | (1 << ENCODER_PIN_YA) | (1 << ENCODER_PIN_YB));
  ENCODER_DDR_Z &= ~((1 << ENCODER_PIN_ZA) | (1 << ENCODER_PIN_ZB));
  
  /** set pullup resistors */
  ENCODER_PORT_XY |= (1 << ENCODER_PIN_XA) | (1 << ENCODER_PIN_XB) | (1 << ENCODER_PIN_YA) | (1 << ENCODER_PIN_YB);
  ENCODER_PORT_Z |= (1 << ENCODER_PIN_ZA) | (1 << ENCODER_PIN_ZB);

  /** Setup rising and falling edge (any change) for PD0, PD1, PD2, PD3 (Pin 21 - 18) */
  EICRA |= (1 << ISC00) | (1 << ISC10) | (1 << ISC20) << (1 << ISC30);
  EICRA &= ~((1 << ISC01) | (1 << ISC11) | (1 << ISC21) | (1 << ISC31));

  /** Setup rising and falling edge (any change) for D2, D3 (Pin 2 - 3) */
  EICRB |= (1 << ISC40) | (1 << ISC50);
  EICRB &= ~((1 << ISC41) | (1 << ISC51));

  EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2) | (1 << INT3) | (1 << INT4) | (1 << INT5);


  EICRA = (1 << PCIE0);



  #endif
  
  readEncoderX();
}
