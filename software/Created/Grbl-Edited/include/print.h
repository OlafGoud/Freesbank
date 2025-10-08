#ifndef MILLING_PRINT_H
#define MILLING_PRINT_H

/**
 * High level printing
 */

#include "system.h"


void printString(const char *s);

void print_uint8_base10(uint8_t n);

void print_uint32_base10(uint32_t n);

void printInteger(long n);

void printFloat(float n, uint8_t decimal_places);

void systemExecuteReadyLine();


#endif

