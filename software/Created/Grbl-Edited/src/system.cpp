#include "system.h"


volatile SystemInfo sys;

void systemExecuteReadyLine() {
	uartWriteString("Milling machine ready V1\n", 25);
}


void readSerialLine() {
	for(;;) {

	}
}


/**
 * Main loop. If this returns with a critical error the program stops. Otherwise the mainloop will restart.
 */
int mainLoop() {
    
  cli();             // disable interrupts
  uartInit(9600);   // setup UART
  sei();             // enable interrupts

  systemExecuteReadyLine();

  while (1) {
    int c = uartRead();
    
    if(c != SERIAL_NO_DATA) {
      break;
    }

    if(c == 63) {
      uartWriteString("Je stinkt\n", 10);
    }

  }


  return 0;
}


