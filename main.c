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
#define  SAMPLE_CNT 50
#define  CAL_NO 1
#define BAR_NUM 4 //偶数！

uint16_t adc_buffer_[SAMPLE_CNT];
uint16_t adc_buffer[SAMPLE_CNT];
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

    // 测试时钟频率
    // P1DIR |= BIT0 + BIT4; //设置P1.0 1.4为输出
    // P1SEL |= BIT0 + BIT4; //设置P1.0输出ACLK，P1.4输出SMCLK

    P1DIR |= BIT1;                  // Set P1.0 to output direction
    //第一个采样点真没法用
    ADC_init_intref_repeating(adc_buffer_, SAMPLE_CNT);
    __delay_cycles(1000);
    
    LissajousInit();
    initial_lcd();

    const uint8_t analyse_start = 2; //反三角第一个计算点
    int i;
    
    for (i = 0; i < SAMPLE_CNT; i++){
        adc_buffer[i] = adc_buffer_[i]; //复制过来
        lcd_deltaphi(0, 0, adc_buffer[i]/1023.0 * 2.5);
        lcd_deltaphi(1, 0, i);
    }
    uint16_t amp1 = adc_buffer[1], amp2 = adc_buffer[1];
    
    
    for(i = 1; i < SAMPLE_CNT; i++){
        if(adc_buffer[i] > amp1){
            amp1 = adc_buffer[i];
        }
        if (adc_buffer[i] < amp2){
            amp2 = adc_buffer[i];
        }
        
    }
    float amp = (amp1 - amp2)/1023.0 * 2.5 /2;
    float theta_for_mean[BAR_NUM];
    float theta_bar = 0;
    for(i = 0; i < BAR_NUM; i++){
        theta_for_mean[i] = (adc_buffer[i+analyse_start] / 1023.0 * 2.5 - 1.0) / 1.0;
        if (theta_for_mean[i] > 1){
            theta_for_mean[i] = 1;
        }else if (theta_for_mean[i] < -1){
            theta_for_mean[i] = -1;
        }
        lcd_deltaphi(0, 0, theta_for_mean[i]);
        lcd_deltaphi(1, 0, i);
        theta_for_mean[i] = asinf(theta_for_mean[i]);
        if (theta_for_mean[i] < 0){
            theta_for_mean[i] += PI;
        }
    }
    for(i = 0; i < BAR_NUM / 2; i++){
        theta_bar += theta_for_mean[i+BAR_NUM/2] - theta_for_mean[i];
    }
    theta_bar /= BAR_NUM / 2.0 / PI * 180;

    
    float theta = asinf((adc_buffer[analyse_start] / 1023.0 * 2.5 - 1.0) / 1.0) / PI * 180;
    theta -= theta_bar * analyse_start; //消除采样点之前的相位延迟
    lcd_deltaphi(2, 0, amp1/ 1023.0 * 2.5);
    // __delay_cycles(100000);
    lcd_deltaphi(3, 0, amp2/ 1023.0 * 2.5);
    // __delay_cycles(100000);
    lcd_deltaphi(4, 0, theta);
    // uint16_t adcdata = getADC();
    lcd_deltaphi(5, 0, theta_bar * 100);
    lcd_deltaphi(6, 0, adc_buffer[analyse_start] / 1023.0 * 2.5);
   
   
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
