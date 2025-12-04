#include <stdio.h>
#include <iostream>

volatile int32_t encoderSteps = 0;

void handleEncoderInterrupt(uint8_t pina, uint8_t pinb) {
  static uint8_t encoderState = 0;
  uint8_t curr = (pina ^ pinb) | (pinb << 1);
  printf("curr %i\n", curr);
  uint8_t diff = (curr - encoderState) & 3;
  int8_t delta = (diff == 1) - (diff == 3);

  encoderSteps += delta;
  encoderState = curr;
}

int main() {
  for(int i = 0; i < 10; i++) {
    handleEncoderInterrupt(0, 0);
    printf("encoderSteps %i\n", encoderSteps);
    handleEncoderInterrupt(1, 0);
    printf("encoderSteps %i\n", encoderSteps);
    handleEncoderInterrupt(1, 1);
    printf("encoderSteps %i\n", encoderSteps);
    handleEncoderInterrupt(0, 1);
    printf("encoderSteps %i\n", encoderSteps);
    handleEncoderInterrupt(1, 1);
    printf("encoderSteps %i\n", encoderSteps);
    handleEncoderInterrupt(0, 1);
    printf("encoderSteps %i\n", encoderSteps);
  }
}