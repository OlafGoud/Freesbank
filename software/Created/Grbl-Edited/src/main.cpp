#include <avr/io.h>
#include <stdlib.h>   
#include <stdio.h>
#include <avr/interrupt.h>

#include "system.h"
#include "macros.h"
#include "stepper.h"


uint8 systemState = IDLE;


/**
 * Main loop, begin.
 * @todo
 */
int main() {
  cli();
  uartInit(BAUD);
  initSteppers();
  //initEncoder();
  sei();
  println("Ready!");
  while (uartRead() != EMPTY_CHAR) {
    println("clearing");
  } 
  while (!(systemState == INTERNAL_ERROR_RESTART_REQUIRED)) {
    /** 
     * Error handling
     * @details systemState -> system erros
     * @details stepper state -> stepper errors
     */
    if(systemState == ERROR) {
      /** @todo error handling */
      println("err");
      /** shutdown stepper */

    }

    if(stepperState == STEPPER_ERROR) {
      /** @todo stepper error handling */
      println("err");

      continue;
    }


    readSerialLine();
    if(stepperState == STEPPER_EMPTY) {
      loadNewSegment();
    }
  }


  /** @note hopefully never reached */
  println("END, restart required!");

  return 0;
}