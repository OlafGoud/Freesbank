#include <avr/io.h>
#include <avr/interrupt.h>
#include "serialdatacommunication.h"

/**
 * Main loop. If this returns with a critical error the program stops. Otherwise the mainloop will restart.
 */
int mainLoop() {
    
  while (true) {
    int c = uart_read();
    if (c != -1) {
      if(c == 63) {
          uart_writeln("<Idle|MPos:0.000,0.000,0.000|FS:0,0|WCO:0.000,0.000,0.000>\nok", 61);
      }
      // Echo back received characters
      //uart_write((uint8_t)c);
    }
  }


    return 0;
}

/**
 * Begin of program.
 */
int main() {
  cli();             // disable interrupts
  uart_init(9600);   // setup UART
  sei();             // enable interrupts

  uart_write('H');
  uart_write('i');
  uart_write('\n');


  //Serial.begin(9600);
  while (true) {
    int state = mainLoop();

    if(state == 1) break; /* The state is critical */


  }


  return 0;
}