#ifndef WProgram_h
#define WProgram_h

#ifdef __ICC430__
#define F_CPU     16000000L
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
#define _BV(x) (1 << x)
/*hide progmem*/
#define PROGMEM 
#define pgm_read_byte(x) (*(uint8_t*)(x))
#define pgm_read_word(x) (*(uint16_t*)(x))
#include <msp430.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <math.h>


#ifndef __ICC430__
#include <avr/interrupt.h>
#endif

#include "wiring.h"

#ifdef __cplusplus
#include "HardwareSerial.h"

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);
#endif

#endif
