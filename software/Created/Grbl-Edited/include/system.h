#ifndef MILLING_MAIN_H
#define MILLING_MAIN_H

#include <avr/io.h>
#include <stdlib.h>   
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>


#include "serialdatacommunication.h"
#include "movement.h"
#include "gcode.h"
#include "print.h"
#include "system.h"
#include "utility.h"
#include "config.h"


/**
 * Main class.
 */

int mainLoop();

void systemExecuteLine();


#endif