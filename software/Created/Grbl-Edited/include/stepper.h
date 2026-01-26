#include "macros.h"
#include "system.h"
#include "encoder.h"
#include <math.h>
#pragma once
extern uint8 stepperState;

struct StepperData {
  float formulaValues[3]{};
  CodeBlock* currentBlock;

  uint32 modifier;
  unsigned long timerValue;
  uint8 selectedPlane = 0;
  float prevY = 0;
  int8 signage = 1;
  uint8 xAxisNumber = 0;
  uint8 yAxisNumber = 1;
  float angleStep = 0;
  float deltaAngle = 0;
};

void initSteppers();
uint8 setHardwareCompareTimer(float targetms);

void loadNewSegment();
void setSegmentDone();
uint8 checkIfAxisNotDone(uint8 axisNumber, float currentPos);

void stepAxisFromStepVar(uint8 axisNumber, uint32 currentCycle, uint32 endCycle, volatile uint8* port, uint8 pin, volatile uint8* dirport, uint8 dirpin);
void stepAxisFromPos(int i, unsigned long currentcycle, unsigned long maxCycle, float currPos, uint8 port, uint8 pin);

float getYForLine(float x, float x1, float x2, float y1, float y2);
float getCircleYPos(uint32 currentTime, float startAngle);
float getCircleXPos(uint32 currentTime, float startAngle);
