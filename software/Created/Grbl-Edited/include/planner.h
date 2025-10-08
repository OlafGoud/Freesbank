#pragma once
#ifndef MILLING_PLANNER_H
#define MILLING_PLANNER_H

#include "system.h"
#define PLANNER_BUFFER_SIZE 16
#define EPS_COS_STRAIGHT 0.999999f


typedef struct {
  float distance;       // length of move (mm)
  float feedrate;       // mm/min
  float dir[N_AXIS];    // unit vector
  float nominal_speed;  // nominal speed
  float accel;
  float entry_speed;        // velocity entry (mm/s)
  float exit_speed;         // velocity exit  (mm/s)
  float t_total;        // total time of motion
  float p0[N_AXIS];     // start pos
  float p1[N_AXIS];     // end pos
} planner_block_t;

typedef struct {
  planner_block_t block[PLANNER_BUFFER_SIZE];
  uint8_t head;
  uint8_t tail;
} planner_t;

extern planner_t planner;



#endif