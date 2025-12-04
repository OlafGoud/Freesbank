#ifndef MILLING_PRINT_H
#define MILLING_PRINT_H

/**
 * High level printing
 */

#include "system.h"


extern float currentPosition[N_AXIS];

void printString(const char *s);

void printUint8Base10(uint8_t n);

void printUint32Base10(uint32_t n);

void printInteger(long n);

void printFloat(float n, uint8_t decimal_places);

void systemExecuteReadyLine();

void printHLine(char c);

void printStatus();

#endif

