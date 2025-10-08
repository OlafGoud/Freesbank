#include "print.h"

void printString(const char *s) {
  while (*s) uartWrite(*s++);
}

// Prints an uint8 variable in base 10.
void print_uint8_base10(uint8_t n) {
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


void print_uint32_base10(uint32_t n) {
  if (n == 0) {
    uartWrite('0');
    return;
  }

  unsigned char buf[10];
  uint8_t i = 0;

  while (n > 0) {
    buf[i++] = n % 10;
    n /= 10;
  }

  for (; i > 0; i--)
    uartWrite('0' + buf[i-1]);
}


void printInteger(long n) {
  if (n < 0) {
    uartWrite('-');
    print_uint32_base10(-n);
  } else {
    print_uint32_base10(n);
  }
}

// Convert float to string by immediately converting to a long integer, which contains
// more digits than a float. Number of decimal places, which are tracked by a counter,
// may be set by the user. The integer is then efficiently converted to a string.
// NOTE: AVR '%' and '/' integer operations are very efficient. Bitshifting speed-up
// techniques are actually just slightly slower. Found this out the hard way.
void printFloat(float n, uint8_t decimal_places) {
  if (n < 0) {
    uartWrite('-');
    n = -n;
  }

  uint8_t decimals = decimal_places;
  while (decimals >= 2) { // Quickly convert values expected to be E0 to E-4.
    n *= 100;
    decimals -= 2;
  }
  if (decimals) { n *= 10; }
  n += 0.5; // Add rounding factor. Ensures carryover through entire value.

  // Generate digits backwards and store in string.
  unsigned char buf[13];
  uint8_t i = 0;
  uint32_t a = (long)n;
  while(a > 0) {
    buf[i++] = (a % 10) + '0'; // Get digit
    a /= 10;
  }
  while (i < decimal_places) {
     buf[i++] = '0'; // Fill in zeros to decimal point for (n < 1)
  }
  if (i == decimal_places) { // Fill in leading zero, if needed.
    buf[i++] = '0';
  }

  // Print the generated string.
  for (; i > 0; i--) {
    if (i == decimal_places) { uartWrite('.'); } // Insert decimal point in right place.
    uartWrite(buf[i-1]);
  }
}

void systemExecuteReadyLine() {
	printString("Milling machine ready V1\n");
}