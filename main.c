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
#define PI 3.14159265358979323846
uint16_t adc_buffer[10];
// const uchar font5x8[5] = {
//     0x7E, // 01111110 -> 第一列
//     0x09, // 00001001 -> 第二列
//     0x09, // 00001001 -> 第三列
//     0x09, // 00001001 -> 第四列
//     0x7E  // 01111110 -> 第五列
// };

void main(void)
{
//    SMCLK 50kHz
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    __delay_cycles(1000);
    ADC_init_intref_repeating(adc_buffer, 16);
    // __delay_cycles(1000);
    // uint16_t adcdata = getADC();
   initial_lcd();
   LissajousInit();
   sin_Data y_sin = getLissaData(adc_buffer, 16);
//
//    res_init();
//    ctrl_init();

    // button_init();
//    set_res(100);
//    set_freq(2);
//    set_volt(0);
    // display_graphic_5x8(0, 0, (uchar*)font5x7);
//    int i;
//    for (i =0; i < 10; i++){
//        lcd_deltaphi(0, 0, adc_buffer[i]/1023.0 * 2.5);
//        __delay_cycles(100000);
//        P1OUT |= BIT1;
//
//    }

    
    
    // delay(50);
    // clear_screen();
    // DisplayLissajous(1, 1, 1, 3,PI/2);
    // display_graphic_1X8(4, 107, &data);
    // if (DisplayLissajous(1, 2, 0))
    // {
    //  P1OUT = 0x01;
    // }else{
    //  P1OUT ^= 0x01;
    // delay(5000);
    // P1OUT |= BIT0;
    // }

    // set_res(1000);
    // Transmit();


    while(1);

}
