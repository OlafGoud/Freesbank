#include <stdlib.h>   
#include <stdio.h>
#include "datacommunication.h"

#define MAX_LINE_SIZE 60

struct GCodeBlock {
  float position[3];  /** xyz position, float, 12 bytes */
  float feedrate;     /** feedrate, float, 4 bytes */
  float extrude;      /** E modifier, amount to extrude, speed of spindle, 0 = off, float, 4 bytes */
  float radius;       /** radius for circle, float, 4 bytes */
  float offset[2];       /** offset xy , float[2], 8 bytes */
};


struct GCodeSettings {
  bool absolute = true;   /** true = absolute (G91), false = relative (G90) */
  
};




void loadSegmentInStepperBuffer();

void readSerialLine();


void readGCodeLine(char* line, uint8_t size);