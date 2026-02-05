#include <stdlib.h>   
#include <stdio.h>
#include <math.h>

#include "datacommunication.h"
#include "macros.h"
#include "encoder.h"
#pragma once

extern SystemState systemState;
extern StepperState stepperState;

void readSerialLine();

bool readGCodeLine(char* line, uint8_t size);

bool readFloat(char *line, uint8_t* i, float* f_ptr);

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


struct CodeBlockBuffer {
  CodeBlock block[CODEBLOCKBUFFERSIZE];
  uint8 tail = 0;
  uint8 head = 1;
  uint8 size = 0;
};

extern CodeBlockBuffer codeBlockBuffer;
