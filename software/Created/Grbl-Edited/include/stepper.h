#include "macros.h"
#include "system.h"
#include "encoder.h"
#include <math.h>

extern uint8 stepperState;

struct StepperData {
  float formulaValues[3]{};
  CodeBlock* currentBlock;

  uint16 modifier;
  uint16 timerValue;
  uint8 selectedPlane = 0;
  float prevY = 0;
};

void initSteppers();
