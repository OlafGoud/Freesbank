#ifndef MILLING_UTILS_H
#define MILLING_UTILS_H
#pragma once
#include "system.h"

/**
 * @note Macro definitions
 * @def CHECK_BIT -> checks specified bit in int
 * @def SET_BIT -> sets specified bit in int
 * @def CLEAR_BIT -> clears specified bit in int
 */

/**
 * Check for 1 bit in a integer.
 * @param var integer to check bit in
 * @param pos position of the bit to check
 * @return int (1 / 0 from that position)
 */
#define CHECK_BIT(var, pos) ((var) & (1 << (pos)))

/**
 * Sets 1 bit in a integer.
 * @param var integer to check bit in
 * @param pos position of the bit to set
 * @return int with bitshift
 */
#define SET_BIT(var, pos) ((var) | (1 << (pos)))

/**
 * Clears 1 bit in a integer.
 * @param var integer to check bit in
 * @param pos position of the bit to clear
 * @return int with bitshift
 */
#define CLEAR_BIT(var, pos) ((var) &~ (1 << (pos)))



/**
 * @note structs
 * @namespace data
 */


// NOTE: When this struct is zeroed, the above defines set the defaults for the system.
typedef struct {
  uint8_t tool_length;     // Vnnn mm
  uint8_t tool_diameter;   // Wnnn mm
  uint8_t program_flow;    // {M0,M1,M2,M30}
  uint8_t coolant;         // {M7,M8,M9}
  uint8_t spindle;         // {M3,M4,M5}
  uint8_t override;        // {M56}
} gc_modes;

typedef struct {
  uint8_t motion;
  float offset[N_AXIS];
  float endPos[3];
  float feedrate;
  float arcRadius;
  gc_modes mode;
} gc_data;

typedef struct {
  float distance;       // length of move (mm)              4
  float feedrate;       // mm/min                           4
  float dir[N_AXIS];    // unit vector                      4 * naxis = 4*3 = 12
  float nominal_speed;  // nominal speed                    4
  float entry_speed;        // velocity entry (mm/s)        4
  float exit_speed;         // velocity exit  (mm/s)        4
  float p0[N_AXIS];     // start pos                        4 * naxis = 4*3 = 12
  float p1[N_AXIS];     // end pos                          4 * naxis = 4*3 = 12
} planner_block_t;

typedef struct {
  planner_block_t block[PLANNER_BUFFER_SIZE];
  uint8_t head;
  uint8_t tail;
} planner_t;







/**
 * @note Functions from here 
 * @def read_float -> read float from a char*.
 */


/** - grbl function
 * Extracts a floating point value from a string. The following code is based loosely on
 * the avr-libc strtod() function by Michael Stumpf and Dmitry Xmelkov and many freely
 * available conversion method examples, but has been highly optimized for Grbl. For known
 * CNC applications, the typical decimal value is expected to be in the range of E0 to E-4.
 * Scientific notation is officially not supported by g-code, and the 'E' character may
 * be a g-code word on some CNC systems. So, 'E' notation will not be recognized.
 * NOTE: Thanks to Radu-Eosif Mihailescu for identifying the issues with using strtod().
 * 
 * @param line char*, char to get float from
 * @param char_counter uint8_t*, counter from where to read
 * @param float_ptr float*, pointer to the place for the returnvalue 
 * 
 * @returns 0 or 1, 1 if succesfull. 0 if not a number is detected
 */
uint8_t readFloat(char *line, uint8_t *char_counter, float *float_ptr);

float vecDot3(const float a[3], const float b[3]);

void vecSub3(float r[3], const float a[3], const float b[3]);

float vecNorm3(const float a[3]);

void vecNormalize3(float r[3]);

#endif