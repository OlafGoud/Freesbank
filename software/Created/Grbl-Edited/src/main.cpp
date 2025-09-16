#include <Arduino.h>
#include "serialdatacommunication.h"

/**
 * Main loop. If this returns with a critical error the program stops. Otherwise the mainloop will restart.
 */

int mainLoop() {
    
    
    for(;;) {
        listenToSerialLine();
    }




    return 0;
}


/**
 * Begin of program.
 */
int main() {

    Serial.begin(115200);

    for(;;) {
        int state = mainLoop();
    
        if(state == 1) break;
    
    
    }
}