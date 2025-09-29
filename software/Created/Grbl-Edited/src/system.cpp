#include "system.h"

volatile SystemInfo sys;
int length = 0;
char line[LINE_BUFFER];

void systemExecuteReadyLine() {
	uartWriteString("Milling machine ready V1\n", 25);
}


void readSerialLine() {
  bool comment = false;
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
      if(line[0] == '$') {
        //TODO: execute system command
      } else {
        executeGcodeLine(line, length);
        // TODO: Execute gc command
      }
      uartWrite('&');
      uartWriteString(line, length);

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
  uartWriteStringWithTerminator("Line is to long. default is 60.\n");

}


/**
 * Main loop. If this returns with a critical error the program stops. Otherwise the mainloop will restart.
 */
int mainLoop() {
    
  cli();             // disable interrupts
  uartInit(9600);    // setup UART
  sei();             // enable interrupts

  systemExecuteReadyLine();

  while (sys.state != 1) {
    readSerialLine();
  }


  return 0;
}


