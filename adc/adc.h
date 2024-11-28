#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>

typedef struct{
    uint8_t amp;
    float omega;
    float delta_phi;
}sin_Data;

void ADC_init_intref_repeating(uint16_t *adc,uint16_t samples_cnt); // Init ADC single channel repeating with internal reference 2.5V

void ADC_init_intref(void); // Init ADC single channel oneshot with internal reference 2.5V
uint16_t getADC(void);

sin_Data getLissaData(uint16_t *buffer, uint16_t samples_cnt);

#endif
