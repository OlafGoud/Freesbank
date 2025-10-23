#include "movement.h"
typedef struct {                                                          
  float endPosition[N_AXIS];                                              // 4 * N_AXIS = 4 * 3 = 12 (4 * 5 = 20) bytes 
  uint8_t spindle; //rpm * 10 250 = 2500 rpm, 1 = 10 rpm, 0 = off         // 1 byte
  float exitVelocity;                                                     // 4 bytes
  float maxVelocity;                                                      // 4 bytes

} StepperBufferBlock;                                                     // total -> 12 + 1 + 4 + 4 = 21

typedef struct {
  StepperBufferBlock block[STEPPER_BUFFER_SIZE];                          // 21 * STEPPER_BUFFER = 21 * 4 = 84
  uint8_t tail;                                                           // 1 byte
  uint8_t head;                                                           // 1 byte
} StepperBuffer;                                                          // total = 84 + 1 + 1 = 86 bytes

StepperBuffer stepperBuffer;


void prepare() {

  if(stepperBuffer.head == stepperBuffer.tail) {
    return;
  }

  planner_block_t* planner = (planner_block_t*)getBufferBlock();

  StepperBufferBlock *stepBlockP = &stepperBuffer.block[stepperBuffer.head];
  memcpy(stepBlockP->endPosition, planner->p1, sizeof(float) * N_AXIS);
  stepBlockP->endPosition[1] = planner->p1[1];
  stepBlockP->exitVelocity = planner->exit_speed;
  stepBlockP->maxVelocity = planner->nominal_speed;



  stepperBuffer.head = (stepperBuffer.head + 1) % STEPPER_BUFFER_SIZE;
}


void isr__() {
  /**
   * fix pinnen
   */


  
}




//#define DEBUGGER 
/**
 * lines
 * @todo
 * @param data gc_data* data -> struct with gcode data
 */
void planLine(gc_data* data) {
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
  printHLine('=');
  planner_t* planner = (planner_t*)getPlan();
  int index = planner->tail;
  while(planner->head != index) {
    planner_block_t* plannerBlock = &planner->block[index];
    printString("distance: "); printFloat(plannerBlock->distance, 5); uartWrite('\n');
    printString("speed entry: "); printFloat(plannerBlock->entry_speed, 2); printString(", exit: "); printFloat(plannerBlock->exit_speed, 2); uartWrite('\n');
    printString("speed modal: "); printFloat(plannerBlock->nominal_speed, 2); uartWrite('\n');
    printString("begin X: "); printFloat(plannerBlock->p0[0], 3); printString(", Y: "); printFloat(plannerBlock->p0[1], 3); uartWrite('\n');
    printString("end X: "); printFloat(plannerBlock->p1[0], 3); printString(", Y: "); printFloat(plannerBlock->p1[1], 3); uartWrite('\n');

    index = (index + 1) % PLANNER_BUFFER_SIZE;
    printHLine('-');
  }
  printHLine('=');

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
  uartWrite('\n');
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
  /*printString("\nRandom: ");
  printFloat(((rand() % 10) * 10), 4);
  uartWrite('\n');*/
}

 //dummy function, not for end product
void moveMotor(int dirX, int dirY) {
  updateCurrentLocation(dirX, dirY);
}


