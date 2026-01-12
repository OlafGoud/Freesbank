
#define uchar unsigned char             /** 0 to 255 */

#define uint8 unsigned char             /** 0 to 255 */
#define int8 char                       /** -128 to 127 */

#define uint16 unsigned int             /** 0 to 65,535 */
#define int16 int                       /** -32,768 to 32,767 */

#define uint32 unsigned long int        /** 0 to 4,294,967,295 */
#define int32 long int                  /** -2,147,483,648 to 2,147,483,647 */

/** 
 * types: arduino uno
 * 
 * char     size: -128 to 127
 * uchar    size: 0 to 255
 * 
 * int8     size: -128 to 127
 * uint8    size: 0 to 255
 * 
 * int16    size: 0 to 65,535
 * uint16   size: -32,768 to 32,767
 * 
 * int32    size: -2,147,483,648 to 2,147,483,647
 * uint32   size: 0 to 4,294,967,295
 * 
 * double   size: 1.5 * 10^-45 to 3.4 * 10^38 (4 bytes)
 * float    size: 1.5 * 10^-45 to 3.4 * 10^38 (4 bytes)
 */


#define BAUD 9600 /** @note Only 9600 suported */
#define MAX_LINE_SIZE 60 /** Line size for serial input */
#define MAX_PLANNER_BUFFER_SIZE 10 /** Planner buffer size.  */
#define PLANNER_MAX_ITERATIONS 10 /** Amount of iterations the planner can execute */

#define OK_MESSAGE "OK." /** message if message is ok */

#define IDLE 0 /** System State */
#define RUNNING 1 /** System State */
#define ERROR 2 /** System State */
#define INTERNAL_ERROR_RESTART_REQUIRED 3 /** System State */


#define STEPPER_FULL 0 /** Stepper state */
#define STEPPER_EMPTY 1 /** Stepper state */
#define STEPPER_ERROR 2 /** Stepper state */

#define AXIS_PLANE_XY 0 /** Work axis plane */
#define AXIS_PLANE_XZ 1 /** Work axis plane */
#define AXIS_PLANE_YZ 2 /** Work axis plane */




#define EPS_COS_STRAIGHT 0.999999f
#define SYSTEM_MAX_ACCEL 300
#define SYSTEM_MAX_BRAKE 200
#define CORNER_DIVIDATION 0.2f



/** Encoders */

#define ENCODERS_AXIS 3

// #define ARDUINO_UNO
#define ARDUINO_MEGA
#ifdef ARDUINO_UNO
#define ENCODER_PIN_XA PD2 /** Interrupt pin for encoder A line */
#define ENCODER_PIN_XB PD3 /** Interrupt pin for encoder B line */
#define ENCODER_PORT_XY DDRD
#define STEPPER_STEP_PORT DDRD
#define STEPPER_DIR_PORT DDRB
#define X_STEP_PIN PD5 /** Pin definition */
#define X_DIR_PIN PD6 /** Pin definition */

#endif


#ifdef ARDUINO_MEGA

#define ENCODER_PIN_XA PD0 /** Interrupt pin for encoder A line */
#define ENCODER_PIN_XB PD0 /** Interrupt pin for encoder B line */
#define ENCODER_PIN_YA PD2
#define ENCODER_PIN_YB PD3
#define ENCODER_PIN_ZA PE4
#define ENCODER_PIN_ZB PE5
#define ENCODER_PORT_XY DDRD
#define ENCODER_PORT_Z DDRE


#define STEPPER_X_STEP_PIN PA1
#define STEPPER_Y_STEP_PIN PA3
#define STEPPER_Z_STEP_PIN PA5
#define STEPPER_X_DIR_PIN PA0
#define STEPPER_Y_DIR_PIN PA2
#define STEPPER_Z_DIR_PIN PA4
#define STEPPER_PORT DDRA
#endif
#define ENCODERPORT DDRD

/** Steppers */
#define STEPPER_ACCURACY 0.5f
#define TIMECONST 3


#define RESET_STEPBITS_US 800


