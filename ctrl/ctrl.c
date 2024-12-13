#include "msp430.h"
#include "msp430g2553.h"
#include <stdint.h>

// 0Ϊ���Ҳ���1Ϊ���ǲ�
uint8_t freq = 1, vpp = 1, type = 0;

#define F_CTRL0_BIT BIT1
#define F_CTRL1_BIT BIT2
#define F_CTRL2_BIT BIT3

#define V_CTRL0_BIT BIT4
#define V_CTRL1_BIT BIT5

#define TYPE_CTRL_BIT BIT0

#define F_CTRL0_LOW P1OUT &= ~F_CTRL0_BIT
#define F_CTRL0_HIGH P1OUT |= F_CTRL0_BIT
#define F_CTRL1_LOW P1OUT &= ~F_CTRL1_BIT
#define F_CTRL1_HIGH P1OUT |= F_CTRL1_BIT   
#define F_CTRL2_LOW P1OUT &= ~F_CTRL2_BIT
#define F_CTRL2_HIGH P1OUT |= F_CTRL2_BIT

#define V_CTRL0_LOW P1OUT &= ~V_CTRL0_BIT
#define V_CTRL0_HIGH P1OUT |= V_CTRL0_BIT
#define V_CTRL1_LOW P1OUT &= ~V_CTRL1_BIT
#define V_CTRL1_HIGH P1OUT |= V_CTRL1_BIT

#define TYPE_CTRL_LOW P2OUT &= ~TYPE_CTRL_BIT
#define TYPE_CTRL_HIGH P2OUT |= TYPE_CTRL_BIT

void ctrl_init(void)
{
    P1DIR |= F_CTRL0_BIT + F_CTRL1_BIT + F_CTRL2_BIT + V_CTRL0_BIT + V_CTRL1_BIT;
    P2DIR |= TYPE_CTRL_BIT;
    // Ƶ������Ϊ2
    F_CTRL0_LOW;
    F_CTRL1_HIGH;
    F_CTRL2_HIGH;
    // ��ѹ����Ϊ��
    V_CTRL0_LOW;
    V_CTRL1_LOW;
    // ��������Ϊ���ǲ�
    TYPE_CTRL_LOW;
}

/**
 * @brief ����Ƶ�ʣ�1-5��Ƶ
 * 
 * @param freq 1-5��Ƶ
 */
void set_freq(unsigned char freq)
{
    switch (freq) 
    {
        case 2:
            F_CTRL0_LOW;
            F_CTRL1_HIGH;
            F_CTRL2_HIGH;
            break;
        case 3:
            F_CTRL0_HIGH;
            F_CTRL1_LOW;
            F_CTRL2_HIGH;
            break;
        case 4:
            F_CTRL0_LOW;
            F_CTRL1_LOW;
            F_CTRL2_HIGH;
            break;
        case 5:
            F_CTRL0_HIGH;
            F_CTRL1_HIGH;
            F_CTRL2_LOW;
            break;
        case 6:
            F_CTRL0_LOW;
            F_CTRL1_HIGH;
            F_CTRL2_LOW;
            break;
        default:
            break;
    }
}

/**
 * @brief ���õ�ѹ����ѹ��Ϊ4����0��ߣ�4��ͣ������ѹ�����鿴��·ͼ
 * 
 * @param volt ��ѹ��λ
 */
void set_volt(unsigned char volt)
{
    switch (volt) 
    {
        case 1:
        // ch0
            V_CTRL0_LOW;
            V_CTRL1_LOW;
            break;
        case 2:
        // ch1
            V_CTRL0_HIGH;
            V_CTRL1_LOW;
            break;
        case 3:
        // ch2
            V_CTRL0_LOW;
            V_CTRL1_HIGH;
            break;
        case 4:
        // ch3
            V_CTRL0_HIGH;
            V_CTRL1_HIGH;
            break;
        default:
            break;
    }
}
//0:���ǲ� 1:���Ҳ�
void set_type(unsigned char type)
{
    if (type == 0)
    {
        // ���ǲ�
        TYPE_CTRL_LOW;
    }
    else
    {
        // ���Ҳ�
        TYPE_CTRL_HIGH;
    }
}
