#include "macros.h"
#include "system.h"
#include "encoder.h"

extern uint8 stepperState;

struct StepperBlock {
  float exitPos[3] {};
  float exitSpeed {};
  float feedrate {}; // maxspeed

  uint8 stepBits;
  uint8 dirBits;

  float beginPosition[3];
  uint32 time;
};

void initSteppers();
