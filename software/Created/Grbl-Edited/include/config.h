#ifndef MILLING_PINOUT_H
#define MILLING_PINOUT_H

#define X_LINIAR_MOTOR_PULSE_PIN
#define X_LINIAR_MOTOR_DIRECTION_PIN

#define Y_LINIAR_MOTOR_PULSE_PIN
#define Y_LINIAR_MOTOR_DIRECTION_PIN

#define Z_LINIAR_MOTOR_PULSE_PIN
#define Z_LINIAR_MOTOR_DIRECTION_PIN

#define X_ROTATION_MOTOR_PULSE_PIN
#define X_ROTATION_MOTOR_DIRECTION_PIN

#define Y_ROTATION_MOTOR_PULSE_PIN
#define Y_ROTATION_MOTOR_DIRECTION_PIN

#define MILLING_SPINDLE_PIN




/********************************************************************************************************************************************
 * Configurable
 *******************************************************************************************************************************************/
#define N_AXIS 3 //amount of axis
//all axis number in array's

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define A_AXIS 3
#define B_AXIS 4

#define AXIS_COMMAND_NONE 0
#define AXIS_COMMAND_MOTION 1



/**
 * Sizes of the Serial TX and RX Buffers
 */
#define TX_BUFFER_SIZE 64
#define RX_BUFFER_SIZE 64


#define SYSTEM_MAX_SPEED 3000
#define SYSTEM_MAX_ACCEL 200

#define MAX_INT_DIGITS 10


/********************************************************************************************************************************************
 * Not configurable
 *******************************************************************************************************************************************/

#define STATE_IDLE          0      // Must be zero. No flags.
#define STATE_CHECK_MODE    bit(1) // G-code check mode. Locks out planner and motion only.
#define STATE_HOMING        bit(2) // Performing homing cycle
#define STATE_CYCLE         bit(3) // Cycle is running or motions are being executed.
#define STATE_HOLD          bit(4) // Active feed hold
#define STATE_JOG           bit(5) // Jogging mode.
#define STATE_SAFETY_DOOR   bit(6) // Safety door is ajar. Feed holds and de-energizes system.
#define STATE_SLEEP         bit(7) // Sleep state.

#define STATE_CLEAR       0
#define STATE_BUSY        0
#define STATE_ALARM       1 
#define STATE_EXIT        7

#define PLANNER_BUFFER_SIZE 5
#define EPS_COS_STRAIGHT 0.999999f


#define SERIAL_NO_DATA 0xff
#define LINE_BUFFER 60

#define SYSTEM_INFO "<Idle|MPos:0.000,0.000,0.000|FS:0,0|WCO:0.000,0.000,0.000>\nok"

#endif