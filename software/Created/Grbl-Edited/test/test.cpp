#include <Arduino.h>

#define INTERUPTPIN1 2
#define INTERUPTPIN2 3

volatile int encoderSteps = 0;
volatile short encoderState = 0;



void setup() {
  pinMode(INTERUPTPIN1, INPUT_PULLUP);
  pinMode(INTERUPTPIN2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERUPTPIN1), encoderStep, CHANGE);
  attachInterrupt(digitalPinToInterrupt(INTERUPTPIN2), encoderStep, CHANGE);
  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
}

void loop() {
  if(Serial.available()) {
    noInterrupts();
    if(Serial.read() == '?') {
      Serial.println(encoderSteps);
    }
    interrupts();
  }

  step();
}

void encoderStep() {
  bool interupt1 = digitalRead(INTERUPTPIN1);
  bool interupt2 = digitalRead(INTERUPTPIN2);

  uint8_t tempStep = interupt1 == 1 ? (interupt2 == 1 ? 2 : 3) : (interupt2 == 1 ? 1 : 0);
  if((encoderState + 1) % 4 == tempStep) {
    encoderState = ((encoderState + 1) % 4); //vooruit
    encoderSteps++;
  } else if((encoderState - 1) % 4 == tempStep) {
    encoderState = ((encoderState - 1) % 4); // achteruit
    encoderSteps--;
  } else {
    //ERR
  }
}

void step() {
  PINB = (1 << PB5);    
}