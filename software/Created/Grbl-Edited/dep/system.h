#ifndef MILLING_MAIN_H
#define MILLING_MAIN_H

#include <avr/io.h>
#include <stdlib.h>   
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#include "config.h"
#include "utils.h"
#include "serialdatacommunication.h"
#include "print.h"
#include "planner.h"
#include "movement.h"
#include "gcode.h"


/**
 * Main class.
 */

int mainLoop();

void systemExecuteLine();


#endif