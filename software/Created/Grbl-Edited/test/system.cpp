#include "system.h"

char* line = new char[MAX_LINE_SIZE];
uint8_t size = 0;
bool lineComment = false;


/**
 * Function to load 1 segement into the buffer for the stepper motors. Also execute all functions for other componements for this segment (like spindle, printer etc)
 * Load 1 segment into buffer to use for the stepper motors
 * @todo load segement
 * @todo spindle functions
 * @todo ( printer functions )
 */
void loadSegmentInStepperBuffer() {

}

/**
 * Function to get data from the RX buffer and make a line from it. And send the right commands to the right functions.
 * @todo reading
 * @todo system commands '$'
 * @todo status commands '?'
 * @todo gcode commands [all other]
 */
void readSerialLine() {
  
  while(size < MAX_LINE_SIZE) {
    char c = uartRead();
    
    if(c == EMPTY_CHAR) return; /** No data on RX Line */

    if(lineComment == true) {
      if(c == ')') lineComment == false; /** comments off */
      continue; 
    }

    if(c == '\n' || c == '\0') {
      line[size++] = '\n';

      if(line[0] == '$' && size > 1) {
        /** @todo handle commands (I (info about system), G (print settings), $ (???)) */
        /** @fn funciton */
      } else if (line[0] == '?') {
        /** @todo print status */
      } else {
        readGCodeLine(line, size);
      }

      size = 0;
      line = new char[MAX_LINE_SIZE];
      continue;
    } else {
      if(c == '(') { /** comments on */
        lineComment = true;
        continue;
      } else if (c == ';') {
        /** ? */
      } else {
        if(c >= 'a' && c <= 'z') c = c - 'a' + 'A'; /** change lowercase letters into capital leters */

        line[size++] = c;
      }
    }
    
  }

  /** when this is reached, the input lines are or to big, or something is not correct. */
  println(size);
}


/**
 * function that processes gcode commands to be used by the rest.
 * @param line char* line of information
 * @param size size of the effective data.
 */
void readGCodeLine(char* line, uint8_t size) {
  
  
  
  return;
}