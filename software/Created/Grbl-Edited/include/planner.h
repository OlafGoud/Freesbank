#ifndef MILLING_PLANNER_H
#define MILLING_PLANNER_H

#include "system.h"



void planBufferLine(float targetPos[N_AXIS], float feedrate);

void plannerRecalculate(float J);

float junctionSpeedFromDeviation(const float u[3], const float v[3], float J, float a);

void* getBufferBlock();

void* getPlan();
#endif