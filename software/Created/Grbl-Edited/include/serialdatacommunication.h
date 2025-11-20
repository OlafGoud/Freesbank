#ifndef MILLING_SERIAL_H
#define MILLING_SERIAL_H

#include "system.h"



void uartInit(unsigned long baud);


uint8_t uartRead();

void readSerialLine();

void uartWrite(uint8_t data);


void uartWriteString(const char *str, int len);

void uartWriteStringWithTerminator(const char *str);


ISR(USART_RX_vect);

ISR(USART_UDRE_vect);
#endif