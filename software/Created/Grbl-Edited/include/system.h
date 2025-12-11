#include <stdlib.h>   
#include <stdio.h>
#include <math.h>
#include "datacommunication.h"
#include "macros.h"

#define MAX_LINE_SIZE 60
#define AXIS_PLANE_XY 0
#define MAX_PLANNER_BUFFER_SIZE 10
#define EPS_COS_STRAIGHT 0.999999f
#define SYSTEM_MAX_ACCEL 300
#define SYSTEM_MAX_BRAKE 200
#define PLANNER_MAX_ITERATIONS 10

extern uint8 systemState;
extern uint8 stepperState;
extern volatile long int encoderSteps;
extern volatile long int targetStep;

struct GCodeBlock {
  float position[3];  /** xyz position, float, 12 bytes */
  float feedrate;     /** feedrate, float, 4 bytes */
  float extrude;      /** E modifier, amount to extrude, speed of spindle, 0 = off, float, 4 bytes */
  float radius;       /** radius for circle, float, 4 bytes */
  float offset[2];    /** offset xy , float[2], 8 bytes */
};


struct GCodeSettings {
  bool absolute = true;             /** true = absolute (G91), false = relative (G90) */
  uint8_t selectedPlane = AXIS_PLANE_XY;     /** 0 = xy, 1 = xz, 2 = yz */
};

struct StepperBlock {
  float exitPos[3];
  float exitSpeed;
  float feedrate; // maxspeed
};

struct PlannerBlock {
  float distance;
  float feedrate;
  float entryPoint[3];
  float exitPoint[3];
  float entrySpeed; 
  float exitSpeed;
  float unitVector[3];
};

struct PlannerBuffer {
  PlannerBlock block[MAX_PLANNER_BUFFER_SIZE];
  uint8_t tail;
  uint8_t head;
};

void loadSegmentInStepperBuffer();

void readSerialLine();

void readGCodeLine(char* line, uint8_t size);

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