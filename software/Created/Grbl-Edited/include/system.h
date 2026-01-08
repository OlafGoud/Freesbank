#include <stdlib.h>   
#include <stdio.h>
#include <math.h>

#include "datacommunication.h"
#include "macros.h"


extern uint8 systemState;
extern uint8 stepperState;
extern volatile int32 encoderSteps[3];
extern volatile int32 targetStep[3];

struct GCodeBlock {
  float position[3] {};  /** xyz position, float, 12 bytes */
  float feedrate {300};     /** feedrate, float, 4 bytes */
  float extrude {};      /** E modifier, amount to extrude, speed of spindle, 0 = off, float, 4 bytes */
  float radius {};       /** radius for circle, float, 4 bytes */
  float offset[2] {};    /** offset xy , float[2], 8 bytes */
};


struct GCodeSettings {
  bool absolute = true;             /** true = absolute (G91), false = relative (G90) */
  uint8_t selectedPlane = AXIS_PLANE_XY;     /** 0 = xy, 1 = xz, 2 = yz */
};

struct StepperBlock {
  float exitPos[3] {};
  float exitSpeed {};
  float feedrate {}; // maxspeed
};

struct PlannerBlock {
  float distance {};
  float feedrate {};
  float entryPoint[3] {};
  float exitPoint[3] {};
  float entrySpeed {}; 
  float exitSpeed {};
  float unitVector[3] {};
};

struct PlannerBuffer {
  PlannerBlock block[MAX_PLANNER_BUFFER_SIZE];
  uint8_t tail;
  uint8_t head;
};

void loadSegmentInStepperBuffer();

void readSerialLine();

bool readGCodeLine(char* line, uint8_t size);

bool readFloat(char *line, uint8_t* i, float* f_ptr);

void generateStraightSegementsFromCurve(GCodeBlock &block, uint8_t selectedPlane);

void addSegmentToPlanBuffer(GCodeBlock &block);
void plannerRecalculate(float J);
float junctionSpeedFromDeviation(const float u[3], const float v[3], float J, float a);
float vecDot3(const float a[3], const float b[3]);

void vecSub3(float r[3], const float a[3], const float b[3]);

float vecNorm3(const float a[3]);

void vecNormalize3(float r[3]);

void setStepperInterupts();
void setEncoderInterupts();
void setDirection();
char* getStatus(int s);


/** Data for CodeBlock */
struct Movement {
  float beginPos[ENCODERS_AXIS];
  float endPos[ENCODERS_AXIS];
  float E, F, H, I, J, R; /** all letters from Gcode in use */
};




struct CodeBlock {
  uchar letter{}; /** can be M or G */
  uint16 command{}; /** command number by G1 -> 1, M231 -> 231 */
  uchar modifier{};
  void* data; /** Pointer to a data struct. In use: Movement */
};

struct CodeBlockBuffer {
  CodeBlock block;
  
}