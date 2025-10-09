#pragma once
#ifndef MILLING_STATUS_H
#define MILLING_STATUS_H


#include <avr/io.h>
#include <stdlib.h>   
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>


#include "serialdatacommunication.h"
#include "config.h"
#include "movement.h"
#include "gcode.h"
#include "print.h"










typedef struct {
	uint8_t state;
	int32_t position[N_AXIS]; // measures in mircometer (1E-6 meter)

	
} SystemInfo ;
extern volatile SystemInfo sys;



void systemExecuteLine();

int mainLoop();

#endif