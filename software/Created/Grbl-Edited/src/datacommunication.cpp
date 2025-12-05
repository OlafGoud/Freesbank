#include "datacommunication.h"

/******************************************************************************************************************************
 *          __                                   ___               ___
 *  ||     //  \\  \\    //\\    //    ||      ||      \\    //  ||      ||    
 *  ||    ||    ||  \\  //  \\  //     ||      ||---    \\  //   ||---   ||   
 *  ||___  \\__//    \\//    \\//      ||___   ||___     \\//    ||___   ||___ 
 *
 ******************************************************************************************************************************/

/** ring buffer for tx buffer. */
volatile uint8_t tx_buffer[SERIAL_BUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;

/** ring buffer for rx buffer. */
volatile uint8_t rx_buffer[SERIAL_BUFFER_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;


/**
 * Init function for uart connection to host pc over usb/serial cable.
 * @param baud baudrate over the cable. - max 9600 
 */
void uartInit(unsigned int baud) { 
  uint16_t ubrr = (F_CPU / 16 / baud) - 1;

  /** @todo add support for a baudrate of '115200' if needed */
	// Set baud rate
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)(ubrr);

	// Enable receiver, transmitter and RX interrupt
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

	// 8 data bits, no parity, 1 stop
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  return;
}

/**
 * Read one byte from the rx buffer. 
 * @note always non-blocking, returns a 0xff (default) if there is no data.
 * @returns the char in the front of the rx buffer. (0xff (default) when there is no data)
 */
unsigned char uartRead() {
  if(rx_head == rx_tail) return EMPTY_CHAR; /** there is no data on the buffer (EMPTY_CHAR = handle character without meaning in ASCII UTF-8) */

  char c = rx_buffer[rx_tail];
  rx_tail = (rx_tail + 1) % SERIAL_BUFFER_SIZE;
  return c;
}

/**
 * Add one byte in the tx buffer. 
 * @note only non blocking if non blocking is implemented. ISR can fire when blocked.
 * @todo Non blocking
 */
void uartWrite(char c) {
  uint8_t next = (tx_head + 1) % SERIAL_BUFFER_SIZE;
  while (next == tx_tail) {
    /** @todo add non blocking */
  }
  tx_buffer[tx_head] = c;
  tx_head = next;

	UCSR0B |= (1 << UDRIE0); /** enable interupt for TX */
  return;
}

/**
 * Interupts when data received via serial and writes to RX buffer.
 */
ISR(USART_RX_vect) {
	uint8_t data = UDR0;
	uint8_t next = (rx_head + 1) % SERIAL_BUFFER_SIZE;
	if (next != rx_tail) {
		rx_buffer[rx_head] = data;
		rx_head = next;
	}
}

/**
 * Interupt to write the data to the TX channel form the TX buffer.
 * activates when there is space on the buffer
 */
ISR(USART_UDRE_vect) {
	if (tx_head == tx_tail) {
		// Buffer empty, disable interrupt
		UCSR0B &= ~(1 << UDRIE0);
	} else {
		UDR0 = tx_buffer[tx_tail];
		tx_tail = (tx_tail + 1) % SERIAL_BUFFER_SIZE;
	}
}


/******************************************************************************************************************************
 * 
 * 
 * HIGH LEVEL
 * 
 * 
 ******************************************************************************************************************************/


/**
 * print a char*. 
 * @param str array of chars
 */
void print(char* str) {
  while (*str) uartWrite(*str++);
}

/**
 * print a char* with a new line.
 * @param str array of chars
 */
void println(char* str) {
  print(str);
  uartWrite('\n');
}

/**
 * print an int value in base 10. can be negative (signed).
 * @param n integer to print in base 10.
 */
void print(int n) {

  if(n == 0) {
    uartWrite('0');
    return;
  }

  if(n < 0) {
    uartWrite('-');
    n = -n;
  }

  uint32_t p = 1;

  // find highest power of 10 ≤ n
  while (n / p >= 10) {
      p *= 10;
  }

  while (p > 0) {
      uartWrite('0' + (n / p));
      n %= p;
      p /= 10;
  }


  return;
}

/**
 * print an int value in base 10 with a new line. can be negative (signed).
 * @param n integer to print in base 10.
 */
void println(int n) {
  print(n);
  uartWrite('\n');
}

/**
 * print a float value in base 10. can be negative.
 * @param n float to print in base 10.
 * @param d decimal places.
 */
void print(float f, uint8_t d) {

/** official grbl code ------------------------------------------------------------------
 * Convert float to string by immediately converting to a long integer, which contains
 * more digits than a float. Number of decimal places, which are tracked by a counter,
 * may be set by the user. The integer is then efficiently converted to a string.
 * @note: AVR '%' and '/' integer operations are very efficient. Bitshifting speed-up 
 * techniques are actually just slightly slower. Found this out the hard way.
 */

  if (f < 0) {
    uartWrite('-');
    f = -f;
  }

  uint8_t decimals = d;
  while (decimals >= 2) { // Quickly convert values expected to be E0 to E-4.
    f *= 100;
    decimals -= 2;
  }
  if(decimals) f *= 10;
  f += 0.5; // Add rounding factor. Ensures carryover through entire value.

  // Generate digits backwards and store in string.
  unsigned char buf[13];
  uint8_t i = 0;
  uint32_t a = (long)f;
  while(a > 0) {
    buf[i++] = (a % 10) + '0'; // Get digit
    a /= 10;
  }
  while (i < d) {
    buf[i++] = '0'; // Fill in zeros to decimal point for (n < 1)
  }
  if (i == d) { // Fill in leading zero, if needed.
    buf[i++] = '0';
  }

  // Print the generated string.
  for (; i > 0; i--) {
    if (i == d) { uartWrite('.'); } // Insert decimal point in right place.
    uartWrite(buf[i-1]);
  } 

}

/**
 * print a float value in base 10 with a new line. can be negative.
 * @param n float to print in base 10.
 * @param d decimal places.
 */
void println(float f, uint8_t d) {
  print(f, d);
  uartWrite('\n');
}

/**
 * print an uint8_t value in base 10. must be positive (unsigned).
 * @param n integer to print in base 10.
 */
void print(uint8_t n) {
  uint8_t digit_a = 0;
  uint8_t digit_b = 0;
  if (n >= 100) { // 100-255
    digit_a = '0' + n % 10;
    n /= 10;
  }
  if (n >= 10) { // 10-99
    digit_b = '0' + n % 10;
    n /= 10;
  }
  uartWrite('0' + n);
  if (digit_b) { uartWrite(digit_b); }
  if (digit_a) { uartWrite(digit_a); }
}

/**
 * print an uint8_t value in base 10 with a new line. must be positive (unsigned).
 * @param n integer to print in base 10.
 */
void println(uint8_t n) {
  print(n);
  uartWrite('\n');
}

/**
 * print a line with n amount characters. with the character '='.
 * @param n amount of characters.
 */
void printHline(uint8_t n) {
  for (int i = 0; i < n; i++) {
    uartWrite('=');
  }
  uartWrite('\n');
}

/**
 * print a line with n amount characters. with the character c.
 * @param n amount of characters.
 * @param c character.
 */
void printHline(uint8_t n, char c) {
  for (int i = 0; i < n; i++) {
    uartWrite(c);
  }
  uartWrite('\n');
}
