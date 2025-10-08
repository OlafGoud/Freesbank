#include "movement.h"


void executeMovementLine() {
 
  for(int i = 0; i < 3; i++) {
    printFloat(currentPos[i], 4);
    printString(", ");
  } 
  printString("\n \n \n");
  
  moveMotor(x, y);


}


void updateCurrentLocation(int dirX, int dirY) {
  currentPos[0] += ((rand() % 10) * (currentPos[0] < dirX ? 1 : -1));
  currentPos[1] += ((rand() % 10) * (currentPos[1] < dirY ? 1 : -1));
  printString("\nRandom: ");
  printFloat(((rand() % 10) * 10), 4);
  uartWrite('\n');
}


void moveMotor(int dirX, int dirY) {
  updateCurrentLocation(dirX, dirY);
}


