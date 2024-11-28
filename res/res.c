#include <msp430.h>
#include "i2c.h"
#include "msp430g2553.h"
#include "res.h"

#define SCL_LOW P1OUT &= ~BIT6
#define SCL_HIGH P1OUT |= BIT6
#define SDA_LOW P1OUT &= ~BIT7
#define SDA_HIGH P1OUT |= BIT7

#define STEPS 127
#define TOTAL_RES 10000
#define RW 100
// const uchar mcp4018_add = (0x2F << 1); //‘0101111’

extern unsigned char WHO_AM_I;
extern int TXByteCtr;

// void I2CDelay()
// {
//     int i;
//     for(i = 0; i < 50; i++);
// }

void res_init()
{
    // P1DIR |= (BIT6 | BIT7);
    // SCL_HIGH;
    // SDA_HIGH;

    // I2CDelay();
    // // 发送起始信号
    // SDA_LOW;
    // SCL_LOW;
    // I2C_init(mcp4018_add);
    init_I2C();
}

// void I2CStop()
// {
//     SCL_LOW;
//     SDA_LOW;
//     I2CDelay();
//     SCL_HIGH;
//     I2CDelay();
//     SDA_HIGH;
//     I2CDelay();
// }

// void send_data(uint dat)
// {
//     uchar mask;

//     for(mask = 0x80; mask != 0; mask >>= 1)  //mask用于探测字节内某一位值的掩码变量，从高到低进行
//     {
//         if((mask & dat) == 0)                //dat传入的字节，该位的值输出到SDA上
//             SDA_LOW;
//         else
//             SDA_HIGH;
//         I2CDelay();                          //延时
//         SCL_HIGH;                         //拉高SCL，等待该位的值传输
//         I2CDelay();                          //延时
//         SCL_LOW;                         //拉低SCL，完成一个位周期
//     }
//     SDA_HIGH;
    
// }

void set_res(uint value)
{
    // uchar data = value * STEPS / TOTAL_RES;
    uint8_t data = (((STEPS * value) / TOTAL_RES) + (value / 1000));
    if (data > 127)
    {
        data = 127;
    }
    TXByteCtr = 1;
    WHO_AM_I = data;

    Transmit();
}