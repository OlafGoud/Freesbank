#ifndef MILLING_GCODE_H
#define MILLING_GCODE_H

#pragma once

#include "system.h"
#define MAX_INT_DIGITS 8 // Maximum number of digits in int32 (and float)
#define AXIS_COMMAND_NONE 0
#define AXIS_COMMAND_MOTION 1
#ifndef N_AXIS
#define N_AXIS 3
#endif

// NOTE: When this struct is zeroed, the above defines set the defaults for the system.
typedef struct {
  uint8_t tool_length;     // Vnnn mm
  uint8_t tool_diameter;   // Wnnn mm
  uint8_t program_flow;    // {M0,M1,M2,M30}
  uint8_t coolant;         // {M7,M8,M9}
  uint8_t spindle;         // {M3,M4,M5}
  uint8_t override;        // {M56}
} gc_modes;



typedef struct {
  uint8_t motion;
  float offset[N_AXIS];
  float endPos[3];
  float feedrate;
  float arcRadius;
  gc_modes mode;

} gc_data;




void executeGcodeLine(const char *line, int length);
uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr);


#endif