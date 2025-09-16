#ifndef CNCSniffer_H
#define CNCSniffer_H
#include <Arduino.h>


/**
 * @param 
 */
void listenToSerialLine() {
    Serial.println(Serial.read());
}

#endif
