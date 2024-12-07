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

#define DELAY(X) for(delay_i_ = 0; delay_i_ < X; delay_i_++) //max 65535

uint16_t adc_buffer[100] = {0};

extern uint16_t T1, T2;
extern uint16_t t1[2], t2[2];
extern double f1, f2;
// extern uint16_t time_stamp;

uint16_t delay_i_;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P1DIR |= BIT0 | BIT3 | BIT4;
    timer_init();
    // ADC_init_intref_repeating(adc_buffer, 100);

    freq_init();
    initial_lcd();
    DELAY(635);
    // button_init();
//    P1OUT |= BIT0;
    // lcd_deltaphi(1, 0, 1);

    // P1OUT ^= BIT0;
    // button_init();
//    set_res(100);
//    set_freq(2);
//    set_volt(0);
    // display_graphic_5x8(0, 0, (uchar*)font5x7);
//    int i;
//    for (i =0; i < 10; i++){
    //    lcd_deltaphi(0, 0, amp/1023.0 * 2.5);
//        __delay_cycles(100000);
//        P1OUT |= BIT1;
//
//    }
    // clear_screen();
    // DisplayLissajous(1, 1, 1, 3,PI/2);
    // set_res(1000);
    // FREQ_OFF;
    // TA1CTL &= ~TAIE;
    lcd_uint16(3, 0, 65535);
    while(1){
        CalculateT();
        lcd_uint16(0, 0,T1);
        lcd_uint16(1, 0, T2);
        lcd_uint16(3, 0, get_time_stamp());
        DELAY(1535);
    }

}
