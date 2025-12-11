
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


#define BAUD 9600

#define IDLE 0
#define RUNNING 1
#define ERROR 2
#define INTERNAL_ERROR_RESTART_REQUIRED 3

#define STEPPER_FULL 0
#define STEPPER_EMPTY 1
#define STEPPER_ERROR 2



#define X_STEP_PIN PD5
#define X_DIR_PIN PD6