#include "system.h"


typedef struct {
	uint8_t state;
	int32_t position[N_AXIS]; /** measures in mircometer (1E-6 meter) @deprecated @todo @remove */

} SystemInfo;

volatile SystemInfo sys;

bool comment = false;
int length = 0;
char line[LINE_BUFFER];
float currentPos[3];


/**
 * @todo init
 * @todo mainloop
 * @todo deinit
 */
int mainLoop() {
  /*init*/
  cli();                        // disable interrupts to prevent system getting input when initilize
  /** @todo clear everything? */

  sys.state = STATE_CLEAR;

  uartInit(9600);               // setup UART (RX / TX)
  /** @todo stepper */

  sei();                        // enable interrupts 

  systemExecuteReadyLine();


  /*main loop*/
  while (!CHECK_BIT(sys.state, STATE_EXIT)) {

    if(CHECK_BIT(sys.state, STATE_ALARM)) {
      /* ALARM STATE so not doing anything and waiting */
      continue;
    }

    readSerialLine();
    prepareStepperBuffer();
    
    
    

    /** @todo get from plannerbuffer
     * 
    stepperLoadPlanning( @todo parameters);
    */
    
  }
  
  /**
   * @note End of program -> never reached
   * @todo power down everything. and activate led 13
   */
  return 1;
}


/**
 * read the serial buffer for input. it seperates it into lines and then sends it to the gcode reader and planner.
 */
void readSerialLine() {
	while (length < LINE_BUFFER) {
    uint8_t data = uartRead();

    if(data == SERIAL_NO_DATA) { 
      return;
    }

    if(comment == true) {
      if(data == ')') {
        comment = false;
      }
      continue;
    }

    // Line end: execute line; otherwise it fills the bufferline
    if(data == '\n' || data == '\0') {
      line[length] = '\n';
      length++;
      if(line[0] == '$' && length > 1) {
        if(line[1] == 'I') {
          //printString("[VER:1.1h.20190830:]\n[OPT:V,15,128]\nok\n"); // dummy
        } else if(line[1] == '$') {
          //printString("$0 = 10\nok\n"); // dummy
        } else if (line[1] == 'G') {
          //printString("[GC:G0 G54 G17 G21 G90 G94 M5 M9 T0 F0 S0]\nok");
        }
        //TODO: execute system command
      } else if (line[0] == '?') {
        printStatus();
      } else {
        executeGcodeLine(line, length);
        // TODO: Execute gc command
      }

      length = 0;
      

    } else {
      if(data == '(') {
        comment = true;
        continue;
      } else if (data == ';') {
        sys.state = 1;
      } else {
        if(data >= 'a' && data <= 'z') {
          data = data - 'a' + 'A';
        }
        line[length] = (char) data;
        length += 1;
      }
      
    }

	}

  // never reached or gcode is to big per line;
  uartWriteStringWithTerminator("Line to long: \0");
  printInteger(length);
  uartWrite('\n');
}




/**
 * @deprecated
 * Main loop. If this returns with a critical error the program stops. Otherwise the mainloop will restart.

 int mainLoop() {
    
  cli();             // disable interrupts
  uartInit(9600);    // setup UART
  sei();             // enable interrupts

  systemExecuteReadyLine();

  while (sys.state != 1) {
    readSerialLine();
      

    executeMovementLine(blockData);
  }


  return 0;
}
*/
