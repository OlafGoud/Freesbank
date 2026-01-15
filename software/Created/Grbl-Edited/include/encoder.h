#include <avr/interrupt.h>
#include "macros.h"
#include "datacommunication.h"

void getCurrentMMFromEncoders(float* encoderValues);
void setAccuracy();
void initEncoder();

static inline void readEncoderX(void);

ISR(PCINT2_vect);