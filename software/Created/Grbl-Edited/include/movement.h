#ifndef MILLING_MOVEMENT_H
#define MILLING_MOVEMENT_H

/**
 * high level movement controller
 */


#include "system.h"
#include "planner.h"
#pragma once


extern float currentPos[];


void executeMovementLine();

void updateCurrentLocation(int dirX, int dirY);

void moveMotor(int dirX, int dirY);

#endif