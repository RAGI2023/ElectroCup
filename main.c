#include <msp430.h>
#include <stdint.h>
#include "intrinsics.h"
#include "lcd/lcd.h"
#include "button/button.h"
#include "Lissajous/Lissajous.h"
#include "msp430g2553.h"
#include "res/res.h"
#include "ctrl/ctrl.h"
#include "adc/adc.h"
#include "timer/timer.h"
#define PI 3.14159265358979323846

const uchar char_v[5] = {0x0F, 0x30, 0x60, 0x60, 0x0F};

uint8_t res_chage = 0;
uint16_t res = 1000;

uint8_t freq_change = 0;
uint8_t freq_times = 2;

uint16_t adc_buffer[100] = {0};

uint16_t amp = 1;

// const float phi[5] = {0, 125.9474 / 180 * PI, 16.9200 / 180 * PI, 57.1433 / 180 * PI, 7.9579 / 180 * PI};
const float phi[5] = {0, 125.9474, 16.9200, 57.1433, 7.9579};
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= BIT0;
    // stop watchdog timer
    // timer_init();
    // ADC_init_intref_repeating(adc_buffer, 100);

    // freq_init(396);
    initial_lcd();
   button_init();
     res_init();
    ctrl_init();
    // set_freq(2);

    set_res(1000);
    P1OUT |= BIT0;

    // lcd_uint8(0, 0, 1);
    while(1){
        
        lcd_uint8(0, 0, freq_times);
        lcd_uint8(1, 0, amp);
        display_graphic_5x8(1, 15, (uchar *)char_v);
        lcd_deltaphi_nozero(2, 0, phi[freq_times - 1]);
        
        // clear_screen();
        DisplayLissajous(1, 1, 1, freq_times, phi[freq_times - 1] / 180.0 * PI);
        // clear_screen();
    }
}
