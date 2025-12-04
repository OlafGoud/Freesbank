#include "serialdatacommunication.h"

/**
 * Buffers for sending and getting
 */

volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;

volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;


/**
 * Init serial in and output
 * @param baudrate amount of bits per second
 */
void uartInit(unsigned long baud) {
	uint16_t ubrr = (F_CPU / 16 / baud) - 1;

	// Set baud rate
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)(ubrr);

	// Enable receiver, transmitter and RX interrupt
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

	// 8 data bits, no parity, 1 stop
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/**
 * Reads RX buffer and process the data
 * Called by main loop.
 */
uint8_t uartRead() {
	if (rx_head == rx_tail) {
		return 0xff; // no data
	}
	uint8_t data = rx_buffer[rx_tail];
	rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
	return data;
}


/**
 * Writes data to the TX buffer.
 * @param data 1 byte / char
 */
void uartWrite(uint8_t data) {
	uint8_t next = (tx_head + 1) % TX_BUFFER_SIZE;
	while (next == tx_tail) {
		// wait if buffer is full
		// TODO: interuptflag for escaping
	}
	tx_buffer[tx_head] = data;
	tx_head = next;

	// Enable Data Register Empty interrupt
	UCSR0B |= (1 << UDRIE0);
}

/**
 * Writes a String to the TX buffer.
 * @param str pointer to array
 * @param len Integer
 */
void uartWriteString(const char *str, int len) {
	if(len > TX_BUFFER_SIZE) {
		return;
	}
	for(int i = 0; i < len; i++) {
		uartWrite((uint8_t) str[i]);
	}
}

/**
 * Writes a string to the TX buffer until it is bigger than the buffersize or a terminator (\0, \n) is reached.
 * @param str const char *str (need to end wiht terminator).
 */
void uartWriteStringWithTerminator(const char *str) {
	for(int i = 0; i < TX_BUFFER_SIZE; i++) {
		if(str[i] == '\n' || str[i] == '\0') {
			return;
		}

		uartWrite(str[i]);
	}
}

/**
 * Interupts when data received via serial and writes to RX buffer.
 */
ISR(USART_RX_vect) {
	uint8_t data = UDR0;
	uint8_t next = (rx_head + 1) % RX_BUFFER_SIZE;
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
		tx_tail = (tx_tail + 1) % TX_BUFFER_SIZE;
	}
}