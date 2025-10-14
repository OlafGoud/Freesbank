#pragma once

#ifndef MILLING_UTILS_H
#define MILLING_UTILS_H
#include "system.h"
#include "config.h"

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



#endif