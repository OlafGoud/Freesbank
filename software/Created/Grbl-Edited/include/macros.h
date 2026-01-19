
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

/** Encoders */

#define ENCODERS_AXIS 3

// #define ARDUINO_UNO
#define ARDUINO_UNO
#ifdef ARDUINO_UNO
#define ENCODER_PIN_XA PD2 /** Interrupt pin for encoder A line */
#define ENCODER_PIN_XB PD3 /** Interrupt pin for encoder B line */
#define ENCODER_PORT_X PORTD
#define ENCODER_DDR_X DDRD
#define STEPPER_STEP_DDR DDRB
#define STEPPER_DIR_DDR DDRB
#define STEPPER_DIR_PORT PORTB
#define STEPPER_STEP_PORT PORTB
/*#define Y_STEP_PIN PD4
#define Y_DIR_PIN PD7
#define X_STEP_PIN PD5 /** Pin definition */
//#define X_DIR_PIN PD6 /** Pin definition */

#define X_STEP_PIN PB0
#define X_DIR_PIN PB1
#define Y_STEP_PIN PB2
#define Y_DIR_PIN PB3
#define Z_STEP_PIN PB4
#define Z_DIR_PIN PB5

#endif


#ifdef ARDUINO_MEGA

#define ENCODER_PIN_XA PD0 /** Interrupt pin for encoder A line */
#define ENCODER_PIN_XB PD1 /** Interrupt pin for encoder B line */
#define ENCODER_PIN_YA PD2
#define ENCODER_PIN_YB PD3
#define ENCODER_PIN_ZA PE4
#define ENCODER_PIN_ZB PE5
#define ENCODER_DDR_XY DDRD
#define ENCODER_PORT_XY PORTD
#define ENCODER_DDR_Z DDRE
#define ENCODER_PORT_Z PORTE


#define STEPPER_X_STEP_PIN PA1
#define STEPPER_Y_STEP_PIN PA3
#define STEPPER_Z_STEP_PIN PA5
#define STEPPER_X_DIR_PIN PA0
#define STEPPER_Y_DIR_PIN PA2
#define STEPPER_Z_DIR_PIN PA4
#define STEPPER_PORT PORTA
#define STEPPER_DDR DDRA
#endif
//#define ENCODERPORT DDRD

/** Steppers */
#define STEPPER_ACCURACY 0.015f

#define STEPPER_ISR_MS 2.0f
#define RESET_STEPBITS_US 1000

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
// hij pakt niet goed de oude

// 10x 6y
// 10.1 32.2
// 12.7 45.6
// 2.6  13