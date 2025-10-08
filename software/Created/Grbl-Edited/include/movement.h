#ifndef MILLING_MOVEMENT_H
#define MILLING_MOVEMENT_H

/**
 * high level movement controller
 */


#include "system.h"
#pragma once
extern float currentPos[];


void executeMovementLine(int x, int y, int z);


void updateCurrentLocation(int dirX, int dirY);

void moveMotor(int dirX, int dirY);

#endif