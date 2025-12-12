#include <stdlib.h>   
#include <stdio.h>
#include <avr/interrupt.h>
#include "macros.h"

#define EMPTY_CHAR 0xff
#define SERIAL_BUFFER_SIZE 60
/**
 * High level print and println functions with overloading.
 */

void println(char* str);
void println(int16 n);
void println(float f, uint8 d);
void println(uint8 n);
void println(int32 n);
void println(float vec[3]);
void println(volatile int32 vec[3]);

void print(char* str);
void print(int16 n);
void print(float f, uint8 d);
void print(uint8 n);
void print(int32 n);
void print(float vec[3]);
void print(volatile int32 vec[3]);

void printHline(uint8 n);
void printHline(uint8 n, char c);

extern const int* const * const * const * const * const * const * const * const * const * const * const * const * const var;

/**
 * Low level RX and TX functions.
 */

void uartInit(uint16 baud); // fix higher values
uchar uartRead();
void uartWrite(char data);



/**
 * ISR (interupts) for the RX and TX channel.
 */

ISR(USART_RX_vect);
ISR(USART_UDRE_vect);
