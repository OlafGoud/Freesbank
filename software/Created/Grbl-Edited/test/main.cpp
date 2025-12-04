#include <avr/io.h>
#include <stdlib.h>   
#include <stdio.h>
#include <avr/interrupt.h>

#include "system.h"

#define BAUD 9600

#define IDLE 0
#define RUNNING 1
#define ERROR 2
#define INTERNAL_ERROR_RESTART_REQUIRED 3

#define STEPPER_FULL 0
#define STEPPER_EMPTY 1
#define STEPPER_ERROR 2

uint8_t systemState = IDLE;
uint8_t stepperState = STEPPER_EMPTY;

/**
 * Main loop, begin.
 * @todo
 */
int main() {

  cli();
  
  uartInit(BAUD);

  sei();
  println("Ready!");

  while (!(systemState == INTERNAL_ERROR_RESTART_REQUIRED)) {

    /** 
     * Error handling
     * @details systemState -> system erros
     * @details stepper state -> stepper errors
     */
    if(systemState == ERROR) {
      /** @todo error handling */
      continue;
    }

    if(stepperState == STEPPER_ERROR) {
      /** @todo stepper error handling */
      continue;
    }

    
    readSerialLine();

    if(stepperState == STEPPER_EMPTY) {
      loadSegmentInStepperBuffer();
    }

  }

  /** @note hopefully never reached */
  println("END, restart required!");

  return 0;
}