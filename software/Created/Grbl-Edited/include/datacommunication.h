#include <stdlib.h>   
#include <stdio.h>
#include <avr/interrupt.h>

#define EMPTY_CHAR 0xff
#define SERIAL_BUFFER_SIZE 60
/**
 * High level print and println functions with overloading.
 */

void println(char* str);
void println(int n);
void println(float f, uint8_t d);
void println(uint8_t n);

void print(char* str);
void print(int n);
void print(float f, uint8_t d);
void print(uint8_t n);

void printHline(uint8_t n);
void printHline(uint8_t n, char c);



/**
 * Low level RX and TX functions.
 */

void uartInit(unsigned int baud); // fix higher values
unsigned char uartRead();
void uartWrite(char data);



/**
 * ISR (interupts) for the RX and TX channel.
 */

ISR(USART_RX_vect);
ISR(USART_UDRE_vect);
