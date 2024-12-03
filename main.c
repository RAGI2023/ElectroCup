#include <msp430.h>
#include <stdint.h>
#include "lcd/lcd.h"
#include "button/button.h"
#include "Lissajous/Lissajous.h"
#include "msp430g2553.h"
#include "res/res.h"
#include "ctrl/ctrl.h"
#include "adc/adc.h"
#include "timer/timer.h"
#define PI 3.14159265358979323846

uint16_t adc_buffer[100] = {0};

extern uint16_t time_stamp;
extern unsigned long int val, val2;

void main(void)
{
//    SMCLK 50kHz
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
//    timer_init();
    ADC_init_intref_repeating(adc_buffer, 100);

    initial_lcd();

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



    while(1){
        lcd_deltaphi(0, 0, adc_buffer[50] /1023.0 * 2.5);
    }

}
