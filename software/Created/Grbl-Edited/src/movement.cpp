#include "movement.h"
///#define DEBUGGER 
/**
 * lines
 * @todo
 * @param data gc_data* data -> struct with gcode data
 */
void planLine(gc_data* data) {
  printString("gcode: 1, 0, lijn \n");
  planBufferLine(data->endPos, data->feedrate);
}


/**
 * Calculates the arc. the arc is splits in multiple short straigt segments. And calls the line function @todo
 * 
 * @param data gc_data* data -> struct with gcode data
 */
void planArcLine(gc_data* data) {
  planLine(data);
}


void prepareStepperBuffer() {

  #ifdef DEBUGGER
  planner_block_t* planner = (planner_block_t*)getBufferBlock();
  printFloat(planner->distance, 2); printString(" -> distance\n");
  printString("entry, exit: ");
  printFloat(planner->entry_speed, 2); uartWrite(' ');
  printFloat(planner->exit_speed, 2); uartWrite('\n');
  printString("begin X: "); printFloat(planner->p0[0], 3); printString(", Y: "); printFloat(planner->p0[1], 3); uartWrite('\n');
  printString("end X: "); printFloat(planner->p1[0], 3); printString(", Y: "); printFloat(planner->p1[1], 3); uartWrite('\n');
  #endif
  printString("full planning\n");
  printString("======================================================\n");
  planner_t* planner = (planner_t*)getPlan();
  int index = planner->tail;
  while(planner->head != index) {
    planner_block_t* plannerBlock = &planner->block[index];
    printString("distance: "); printFloat(plannerBlock->distance, 5); uartWrite('\n');
    printString("entry speed: "); printFloat(plannerBlock->entry_speed, 2); printString(", exit speed: "); printFloat(plannerBlock->exit_speed, 2); uartWrite('\n');
    printString("modal speed: "); printFloat(plannerBlock->nominal_speed, 2); uartWrite('\n');
    printString("begin X: "); printFloat(plannerBlock->p0[0], 3); printString(", Y: "); printFloat(plannerBlock->p0[1], 3); uartWrite('\n');
    printString("end X: "); printFloat(plannerBlock->p1[0], 3); printString(", Y: "); printFloat(plannerBlock->p1[1], 3); uartWrite('\n');

    index = (index + 1) % PLANNER_BUFFER_SIZE;
    printString("----------------------------------------------------\n");
  }
  printString("======================================================\n");


}

/*

test sequence gcode

G1 X3 Y4 F400
G1 X8 Y10
G1 X3 Y10
G1 X199 Y400
G1 X30 Y20




*/



/**
 * Executes line movement for a block.
 * @todo
 * @param data gc_data* data -> struct with gcode data
 */
void executeMovementLine(void* fdata) {
  gc_data &data = *(gc_data*)fdata;
  printString("gcode: ");
  printInteger(data.motion);
  printString("\n");
  switch (data.motion)
  {
  case 0: case 1:
    planLine(&data);
    break;
  case 2: case 3:
    // TODO arc
    break;
  default:
    break;
  }
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


