#pragma once
#ifndef MILLING_STATUS_H
#define MILLING_STATUS_H


#include <avr/io.h>
#include <stdlib.h>   
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>


#include "serialdatacommunication.h"
#include "config.h"
#include "gcode.h"



/********************************************************************************************************************************************
 * Configurable
 *******************************************************************************************************************************************/

#define N_AXIS 5 //amount of axis
//all axis number in array's

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define A_AXIS 3
#define B_AXIS 4




/********************************************************************************************************************************************
 * Not configurable
 *******************************************************************************************************************************************/

#define STATE_IDLE          0      // Must be zero. No flags.
#define STATE_ALARM         bit(0) // In alarm state. Locks out all g-code processes. Allows settings access.
#define STATE_CHECK_MODE    bit(1) // G-code check mode. Locks out planner and motion only.
#define STATE_HOMING        bit(2) // Performing homing cycle
#define STATE_CYCLE         bit(3) // Cycle is running or motions are being executed.
#define STATE_HOLD          bit(4) // Active feed hold
#define STATE_JOG           bit(5) // Jogging mode.
#define STATE_SAFETY_DOOR   bit(6) // Safety door is ajar. Feed holds and de-energizes system.
#define STATE_SLEEP         bit(7) // Sleep state.

#define SERIAL_NO_DATA 0xff
#define LINE_BUFFER 60

#define SYSTEM_INFO "<Idle|MPos:0.000,0.000,0.000|FS:0,0|WCO:0.000,0.000,0.000>\nok"




typedef struct {
	uint8_t state;
	int32_t position[N_AXIS]; // measures in mircometer (1E-6 meter)

	
} SystemInfo ;
extern volatile SystemInfo sys;


/**
 * write ready message for fork.
 */
void systemExecuteReadyLine();

void systemExecuteLine();

int mainLoop();

#endif