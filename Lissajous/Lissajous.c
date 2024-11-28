#include "Lissajous.h"
#include "lcd/lcd.h"
#include "cos.h"
#include "math.h"

#define POINTS_NUM 300
#define PI 3.1415926535

#define ERR (1e-1 * 5)

void LissajousInit()
{

}



// 计算最大公约数（GCD）函数，使用欧几里得算法 
// 魔改了一下，接收float类型的参数，返回1位小数
float gcd(float a, float b) {
    int a_cal = (int)(a * 10);
    int b_cal = (int)(b * 10);
    while (b_cal != 0) {
        int temp = b_cal;
        b_cal = a_cal % b_cal;
        a_cal = temp;
    }
    return a / 10.0;
}

// 输入sin函数
void GenerateLissajous(float amp1, float amp2, float omega1, float omega2, float delta_phi)
{
    float t = 0;
    // float omega_ = gcd(omega1, omega2); //覆盖低频信号的完整周期
    float omega_ = omega1; 
    float dt = 2*PI  / omega_ / POINTS_NUM;
    float scale_x = 128 / (2 * amp1);
    float scale_y = 64 / (2 * amp2);
    const uchar O_col = 64, O_page = 4;  
    uchar col_write, page_write;
    uchar data = 0x01;
    float x_phi, y_phi;
    
    uchar data_prev;
    uchar page_prev, col_prev;
    // 选择较大的分度值?
    // scale_x = scale_x > scale_y ? scale_x : scale_y;
    // scale_y = scale_x > scale_y ? scale_x : scale_y;
    scale_x = scale_y;
    // scale_x /= 2;
    // scale_y /= 2;
    int i, x, y;

        t += dt;
        x_phi = COS(PI / 2 - omega1 * t);
        y_phi = COS(PI / 2 - omega2 * t - delta_phi);
        x = amp1 * x_phi * scale_x;
        y = amp2 * y_phi * scale_y;
        col_write = x + O_col;
        if (fabsf(amp1 * x_phi * scale_x - x) > ERR){
            if (x >= 0)
                col_write++;
            else
             col_write--;
        }
        // if (x_phi >0 && fabsf(x_phi) > ERR){
        //     y++;
        // }else if (x_phi< 0 && fabsf(x_phi) > ERR) {
        //     y--;
        // }
        if (y > 0 || fabsf(amp2 * y_phi * scale_y) < ERR){
            page_write = -(y / 8) + O_page - 1;
        }else {
            page_write = -(y / 8) + O_page;
        }
        
        data = 0x01;
        if (y >= 0){
            // data  <<= y % 8;
            data <<= (7 - (y % 8));
        }
        else {
            // data = 0x01 <<(7 - ((0-y) % 8));
            data  <<= -y % 8;
        }
        col_prev = col_write;
        page_prev = page_write;
        data_prev = data;


    for (i = 1; i < POINTS_NUM; i++) {
        t += dt;
        x_phi = COS(PI / 2 - omega1 * t);
        y_phi = COS(PI / 2 - omega2 * t - delta_phi);
        x = amp1 * x_phi * scale_x;
        y = amp2 * y_phi * scale_y;
        col_write = x + O_col;
        if (fabsf(amp1 * x_phi * scale_x - x) > ERR){
            if (x >= 0)
                col_write++;
            else
             col_write--;
        }
        // if (x_phi >0 && fabsf(x_phi) > ERR){
        //     y++;
        // }else if (x_phi< 0 && fabsf(x_phi) > ERR) {
        //     y--;
        // }
        // page_write = -(y / 8) + O_page;
        if (y > 0 || fabsf(amp2 * y_phi * scale_y) < ERR){
            page_write = -(y / 8) + O_page - 1;
        }else {
            page_write = -(y / 8) + O_page;
        }
        data = 0x01;
        if (y >= 0){
            // data  <<= y % 8;
            data <<= (7 - (y % 8));
        }
        else {
            // data = 0x01 <<(7 - ((0-y) % 8));
            data  <<= -y % 8;
        }

        if (col_write == col_prev && page_write == page_prev){
            data |= data_prev;
            data_prev = data;
        }else {
            col_prev = col_write;
            page_prev = page_write;
            data_prev = data;
        }
        // lcd_deltaphi(0, 0, col_write * 1.0);
        // lcd_deltaphi(4, 0, y * 1.0);
        // lcd_deltaphi(5, 0, data * 1.0);
        // lcd_deltaphi(7, 0, page_write * 1.0);
        // delay(50);
        display_graphic_1X8(page_write, col_write, &data);
        // delay(50);
        // data = 0x01;
        // display_graphic_1X8(4, 107, &data);
        // break;




    }
}

/**
 * @brief 显示李萨如图形
 * 
 * @param amp1 x轴振幅
 * @param amp2 y轴振幅
 * @param omega1 x信号角频率
 * @param omega2 y信号角频率
 * @param delta_phi 相位差（弧度）
 * @return uchar 
 */
uchar DisplayLissajous(float amp1, float amp2, float omega1, float omega2, float delta_phi)
{
    // int i;

    GenerateLissajous(amp1, amp2, omega1, omega2, delta_phi);
    // // display_128x64((uchar *)image);
    // for (i = 0; i < i1; i++){
    //     SetValue(points1[i].y, points1[i].x);
    // }
    // SetValue(2, 2);
    // *(subimage1+1) = 0xFF;
    // display_graphic_64x32(0, 0, (uchar *)subimage);
    // display_graphic_64x32(0, 64, subimage2);
    // display_graphic_64x32(4, 0, subimage3);
    // display_graphic_64x32(4, 64, subimage4);
    return 1;
}