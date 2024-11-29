#include <math.h>
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
#define  SAMPLE_CNT 100
#define  CAL_NO 1
#define BAR_NUM 10

uint16_t adc_buffer[SAMPLE_CNT] = {0};
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
    P1DIR |= BIT1;                  // Set P1.0 to output direction
    //第一个采样点真没法用
    ADC_init_intref_repeating(adc_buffer, SAMPLE_CNT);
    uint16_t amp1 = adc_buffer[1], amp2 = adc_buffer[1];
    LissajousInit();
    initial_lcd();
    int i;
    for(i = 1; i < SAMPLE_CNT; i++){
        if(adc_buffer[i] > amp1){
            amp1 = adc_buffer[i];
        }
        if (adc_buffer[i] < amp2){
            amp2 = adc_buffer[i];
        }
        lcd_deltaphi(0, 0, adc_buffer[i]/1023.0 * 2.5);
        lcd_deltaphi(1, 0, i);
        // P1OUT ^= BIT1;
    }
    float amp = (amp1 - amp2)/1023.0 * 2.5 /2;
    float theta_bar = 0.0;
    for(i = 1; i <= BAR_NUM; i++){
        theta_bar += asinf(adc_buffer[i] / 1023.0 * 2.5 / 1.0);
    }
    theta_bar /= BAR_NUM;

    float theta = asinf(adc_buffer[1] / 1023.0 * 2.5 / 1.0);
    theta -= theta_bar; //消除第2个采样点带来的相位延迟
    lcd_deltaphi(2, 0, amp1/ 1023.0 * 2.5);
    // __delay_cycles(100000);
    lcd_deltaphi(3, 0, amp2/ 1023.0 * 2.5);
    // __delay_cycles(100000);
    lcd_deltaphi(4, 0, theta / PI * 180);
    // uint16_t adcdata = getADC();
   
   
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
    //    lcd_deltaphi(0, 0, amp/1023.0 * 2.5);
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
