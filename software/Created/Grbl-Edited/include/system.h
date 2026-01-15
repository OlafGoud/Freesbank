#include <stdlib.h>   
#include <stdio.h>
#include <math.h>

#include "datacommunication.h"
#include "macros.h"
#include "encoder.h"
#pragma once

extern uint8 systemState;
extern uint8 stepperState;

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

void readSerialLine();

bool readGCodeLine(char* line, uint8_t size);

bool readFloat(char *line, uint8_t* i, float* f_ptr);
float vecDot3(const float a[3], const float b[3]);
void vecSub3(float r[3], const float a[3], const float b[3]);
float vecNorm3(const float a[3]);
void vecNormalize3(float r[3]);

void setDirection();
char* getStatus(int s);


struct CodeBlock {
  uchar letter{}; /** can be M or G */
  uint16 command{}; /** command number by G1 -> 1, M231 -> 231 */
  uint8 subCommand{};
  float beginPos[ENCODERS_AXIS]{};
  float endPos[ENCODERS_AXIS]{};
  float E{}, F{}, H{}, I{}, J{}, R{}; /** all letters from Gcode in use */
};

#define CODEBLOCKBUFFERSIZE 16
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

struct CodeBlockBuffer {
  CodeBlock block[CODEBLOCKBUFFERSIZE];
  uint8 tail = 0;
  uint8 head = 1;
  uint8 size = 0;
};

extern CodeBlockBuffer codeBlockBuffer;
