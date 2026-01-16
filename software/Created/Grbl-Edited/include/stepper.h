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

};

void initSteppers();
void loadNewSegment();
float getYForLine(float x, float x1, float x2, float y1, float y2);
float getYForCircle(float x, float y);
void stepAxisFromPos(int i, unsigned long currentcycle, unsigned long maxCycle, float currPos, uint8 port, uint8 pin);

uint8 setHardwareCompareTimer(float targetms);
void setSegmentDone();
uint8 checkIfAxisNotDone(uint8 axisNumber, float currentPos);