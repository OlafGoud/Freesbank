#ifndef MILLING_MOVEMENT_H
#define MILLING_MOVEMENT_H

/**
 * high level movement controller
 */
#include "system.h"


extern float currentPos[];

/**
 * Excecutes movement lines.
 * @note void* -> gc_data. conflict errors in header functions make it not posible to user the gc_data struct.
 * @param fdata (void*) only gc_data*
 */
void executeMovementLine(void* fdata);

void updateCurrentLocation(int dirX, int dirY);

void moveMotor(int dirX, int dirY);

#endif