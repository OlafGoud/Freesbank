#ifndef MILLING_MAIN_H
#define MILLING_MAIN_H


#include <avr/io.h>
#include <stdlib.h>   
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>


#include "serialdatacommunication.h"
#include "movement.h"
#include "gcode.h"
#include "print.h"
#include "system.h"
#include "utility.h"
#include "config.h"




typedef struct {
	uint8_t state;
	int32_t position[N_AXIS]; // measures in mircometer (1E-6 meter)

} SystemInfo ;

volatile SystemInfo sys;


/**
 * @todo init
 * @todo mainloop
 * @todo deinit
 */
int main() {

  /*init*/
  sys.state = STATE_CLEAR;
  

  /*main loop*/
  while (!CHECK_BIT(sys.state, STATE_EXIT)) {

    if(CHECK_BIT(sys.state, STATE_ALARM)) {
      /* ALARM STATE not doing anything */
      continue;
    }
    
    

    /** @todo get from plannerbuffer
     * 
    stepperLoadPlanning( @todo parameters );
    */
     






  }
  
  /**
   * @note End of program -> never reached
   * @todo power down everything. and activate led 13
   */
  return 1;
}


#endif