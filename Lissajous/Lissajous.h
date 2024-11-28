#ifndef _LISSAJOUS_H_
#define _LISSAJOUS_H_

#include "lcd/lcd.h"
#include <stdint.h>

typedef struct{
    int8_t x, y;
} Point;

typedef struct{
    float amp1, amp2, omega1, omega2, delta_phi;
}LissaData;

uchar DisplayLissajous(float amp1, float amp2, float omega1, float omega2, float delta_phi);
void LissajousInit();

#endif
