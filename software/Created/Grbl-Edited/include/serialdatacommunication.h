#pragma once
#ifndef MILLING_SERIAL_H
#define MILLING_SERIAL_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "system.h"


/**
 * Init serial in and output
 * @param baudrate amount of bits per second
 */
void uartInit(unsigned long baud);

/**
 * Reads RX buffer and process the data
 * Called by main loop.
 */
int uartRead();


/**
 * Writes data to the TX buffer.
 * @param data 1 byte / char
 */
void uartWrite(uint8_t data);

/**
 * Writes a String to the TX buffer.
 * 
 */
void uartWriteString(char data[], int length);

/**
 * Interupts when data received via serial and writes to RX buffer.
 */
ISR(USART_RX_vect);


/**
 * Interupt to write the data to the TX channel form the TX buffer.
 * activates when there is space on the buffer
 */
ISR(USART_UDRE_vect);
#endif