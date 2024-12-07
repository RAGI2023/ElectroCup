#ifndef _TIMER_H_
#define _TIMER_H_

#include "msp430g2553.h"
#include "stdint.h"

#define FREQ_OFF P1IE &= ~(BIT1 | BIT2) // interrupt off
#define FREQ_ON P1IE |= BIT1 | BIT2 // interrupt on

void timer_init(void);
uint16_t get_time_stamp(void);
void CalculateT(void);
void CalculateF(void);
void freq_init(void);
uint16_t get_time_stamp(void);

#endif
