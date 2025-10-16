#include "movement.h"


/**
 * Executes line movement for a block.
 * @todo
 * @param data gc_data* data -> struct with gcode data
 */
void executeMovementLine(void* fdata) {
  gc_data &data = *(gc_data*)fdata;
  switch (data.motion)
  {
  case 0: case 1:
    planBufferLine(data.endPos, data.feedrate);
    break;
  case 2: case 3:
    // TODO arc
    break;
  default:
    break;
  }
}

/**
 * lines
 * @todo
 * @param data gc_data* data -> struct with gcode data
 */
void planLine(gc_data* data) {

}


/**
 * Calculates the arc. the arc is splits in multiple short straigt segments. And calls the line function @todo
 * 
 * @param data gc_data* data -> struct with gcode data
 */
void planArcLine(gc_data* data) {
  
}




/*********************************************************************************
 * dummy functions
 * @todo
 */

 //dummy function, not for end product
void updateCurrentLocation(int dirX, int dirY) {
  currentPos[0] += ((rand() % 10) * (currentPos[0] < dirX ? 1 : -1));
  currentPos[1] += ((rand() % 10) * (currentPos[1] < dirY ? 1 : -1));
  printString("\nRandom: ");
  printFloat(((rand() % 10) * 10), 4);
  uartWrite('\n');
}

 //dummy function, not for end product
void moveMotor(int dirX, int dirY) {
  updateCurrentLocation(dirX, dirY);
}


