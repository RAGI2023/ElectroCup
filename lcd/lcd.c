#include <msp430g2553.h>
#include <stdint.h>
#include "lcd.h"
#include "lcd/lcd.h"

#define RESET_LOW       P2OUT = P2OUT & (~BIT4)     // �������
#define RESET_HIGH      P2OUT = P2OUT | BIT4        // �������
#define SI_LOW          P2OUT = P2OUT & (~BIT2)     // �������������
#define SI_HIGH         P2OUT = P2OUT | BIT2        // �������������
#define SCK_LOW         P2OUT = P2OUT & (~BIT1)     // ����ʱ�������
#define SCK_HIGH        P2OUT = P2OUT | BIT1        // ����ʱ�������
#define CS_LOW          P2OUT = P2OUT & (~BIT3)     // Ƭѡ��
#define CS_HIGH         P2OUT = P2OUT | BIT3        // Ƭѡ��
#define A0_LOW          P2OUT = P2OUT & (~BIT5)     // A0��
#define A0_HIGH         P2OUT = P2OUT | BIT5        // A0��

#define LCD_PORT P2DIR

void delay(uint time)
{
	uint i, j;
	for (i = 0; i<time; i++)
	{
		for (j = 0; j<10; j++);
	}
}

const unsigned char num[10][5] = {
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // ���� '0'
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // ���� '1'
    {0x42, 0x61, 0x51, 0x49, 0x46}, // ���� '2'
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // ���� '3'
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // ���� '4'
    {0x27, 0x45, 0x45, 0x45, 0x39}, // ���� '5'
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // ���� '6'
    {0x01, 0x71, 0x09, 0x05, 0x03}, // ���� '7'
    {0x36, 0x49, 0x49, 0x49, 0x36}, // ���� '8'
    {0x06, 0x49, 0x49, 0x29, 0x1E}  // ���� '9'
};

// const unsigned char delta[5] = {0x20, 0x50, 0x88, 0x88, 0xF8};
// const unsigned char phi[5] = {0x70, 0x88, 0x88, 0x78, 0x88};
const unsigned char decimal_point[5] = {
    0x00, // �� 1 ��
    0x00, // �� 2 ��
    0x80, // �� 3 ��
    0x00, // �� 4 ��
    0x00  // �� 5 ��
};
const unsigned char minus_sign[5] = {
    0x10, // �� 1 ��
    0x10, // �� 2 ��
    0x10, // �� 3 �У������ߣ�
    0x10, // �� 4 ��
    0x10  // �� 5 ��
};



// дָ� LCD ģ�� 
void transfer_command_lcd(int data1)
{
	char i;
	// lcd_cs1 = 0;
    CS_LOW;
	// lcd_rs = 0;
    A0_LOW;
	for (i = 0; i < 8; i++)
	{
		// lcd_sclk = 0;    
		// delay(10);  //��������ʱ   
        SCK_LOW;
		if (data1 & 0x80){
            // lcd_sid = 1;
            SI_HIGH;
        }
		else{
            // lcd_sid = 0;
            SI_LOW;
        }
		// lcd_sclk = 1;   
		// delay(10);  //��������ʱ    
        SCK_HIGH;
		data1 = data1 << 1;
	}
	// lcd_cs1 = 1;
    // CS_HIGH;
	SCK_LOW;
}


//д���ݵ� LCD ģ�� 
void transfer_data_lcd(int data1)
{
	uchar i;
	// lcd_cs1 = 0;
    CS_LOW;
	// lcd_rs = 1;
    A0_HIGH;
	for (i = 0; i < 8; i++)
	{
		// lcd_sclk = 0;
        SCK_LOW;
		// delay(10);  //��������ʱ
		if (data1 & 0x80){
            // lcd_sid = 1;
            SI_HIGH;
        }
		else{
            // lcd_sid = 0;
            SI_LOW;
        }
		// lcd_sclk = 1;
		// delay(10);  //��������ʱ
        SCK_HIGH;
		data1 = data1 << 1;
	}
	// lcd_cs1 = 1;
    // CS_HIGH;
	SCK_LOW;
}

void initial_lcd()
{
	LCD_PORT |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
	
	// lcd_reset = 0;          //�͵�ƽ��λ  
    // RESET_LOW;
	// delay(100);
	// lcd_reset = 1;       //��λ���
    RESET_HIGH;  
	// delay(100);
	// transfer_command_lcd(0xe2);  //��λ  
	// delay(5);
	// transfer_command_lcd(0x2c);  //��ѹ���� 1  
	// delay(50);
	// transfer_command_lcd(0x2e);  //��ѹ���� 2  
	// delay(50);
	// transfer_command_lcd(0x2f);  //��ѹ���� 3 
	// delay(5);
	// transfer_command_lcd(0x23);  //�ֵ��Աȶȣ������÷�Χ 0x20��0x27  
	// transfer_command_lcd(0x81);  //΢���Աȶ�  
	// transfer_command_lcd(0x28);  //΢���Աȶȵ�ֵ�������÷�Χ 0x00��0x3f  
	// transfer_command_lcd(0xa2);  //1/9 ƫѹ�ȣ�bias��  
	// transfer_command_lcd(0xc8);  //��ɨ��˳�򣺴��ϵ���  
	transfer_command_lcd(0xa1);  //��ɨ��˳�򣺴�����  
	// transfer_command_lcd(0x40);  //��ʼ�У���һ�п�ʼ  
	transfer_command_lcd(0xaf);  //����ʾ 
}


void lcd_address(uint page, uint column)
{
	column = column;
	
	transfer_command_lcd(0x10 + ((column >> 4 )& 0x0f)); //�����е�ַ�ĸ� 4 λ  
	transfer_command_lcd(column & 0x0f); //�����е�ַ�ĵ� 4 λ  
	transfer_command_lcd(0xb0 + page);   //����ҳ��ַ��ÿ 8 ��Ϊһҳ��ȫ���� 64 �У����ֳ� 8 ҳ 
}


void clear_screen()
{
	unsigned char i, j;
	for (i = 0; i<9; i++)
	{
		transfer_command_lcd(0xb0 + i);
		transfer_command_lcd(0x10);
		transfer_command_lcd(0x00);
		for (j = 0; j<132; j++)
		{
			transfer_data_lcd(0x00);
		}
	}
}


//��ʾ 128x64 ����ͼ�� 
void display_128x64(uchar *dp)
{
	uchar i, j;
	for (j = 0; j<8; j++)
	{
		lcd_address(j + 1, 1);
		for (i = 0; i<128; i++)
		{
			transfer_data_lcd(*dp);     //д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1    
			dp++;
		}
	}
}

//��ʾ 16x16 ����ͼ�񡢺��֡���Ƨ�ֻ� 16x16 ���������ͼ�� 
void display_graphic_16x16(uint8_t page, uint8_t column, uint8_t *dp)
{
	uint8_t i, j;
	for (j = 0; j<2; j++)
	{
		lcd_address(page + j, column);
		for (i = 0; i<16; i++)
		{
			transfer_data_lcd(*dp);     //д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1    
			dp++;
		}
	}
}

//��ʾ 8x16 ����ͼ��ASCII, �� 8x16 ����������ַ�������ͼ�� 
void display_graphic_8x16(uchar page, uchar column, uchar *dp)
{
	uint i, j;
	for (j = 0; j<2; j++)
	{
		lcd_address(page + j, column);
		for (i = 0; i<8; i++)
		{
			transfer_data_lcd(*dp);     //д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1   
			dp++;
		}
	}
}

//��ʾ 64x32 ����ͼ��
void display_graphic_64x32(uchar page, uchar column, uchar *dp)
{
	uint i, j;
	for (j = 0; j<4; j++)
	{
		lcd_address(page + j, column);
		for (i = 0; i<64; i++)
		{
			transfer_data_lcd(*dp);     //д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1   
			dp++;
		}
	}
}


//��ʾ 5X8 ����ͼ��ASCII, �� 5x8 ����������ַ�������ͼ�� 
void display_graphic_5x8(uchar page, uchar column, uchar *dp)
{
	uint i;
	lcd_address(page, column);
	for (i = 0; i<5; i++)
	{
		transfer_data_lcd(*dp);
		dp++;
	}
}

void display_graphic_1X8(uchar page, uchar column, uchar *dp)
{
	lcd_address(page, column);
	transfer_data_lcd(*dp);

}

void lcd_deltaphi(uchar page, uchar column, float deltaphi)
{
	uchar single_col = 5;
	uchar *dp = (uchar*)num[(int)deltaphi];
	uint i = 0;
	uchar num_col, num_hundreds, num_tens, num_units, num_pointones;
	num_col = column;
	if (deltaphi < 0)
	{
		display_graphic_5x8(page, column, (uchar *)minus_sign);
		deltaphi = -deltaphi;
		num_col += single_col;
	}
	num_hundreds = (int)deltaphi / 100;
	deltaphi = deltaphi - num_hundreds * 100;
	num_tens = (int)deltaphi / 10;
	num_units = (int)deltaphi % 10;
	num_pointones = (int)(deltaphi * 10) % 10;
	display_graphic_5x8(page, num_col + single_col, (uchar *)num[num_hundreds]);
	display_graphic_5x8(page, num_col + single_col * 2, (uchar *)num[num_tens]);
	display_graphic_5x8(page, num_col + single_col * 3, (uchar *)num[num_units]);
	display_graphic_5x8(page, num_col + single_col * 4, (uchar *)decimal_point);
	display_graphic_5x8(page, num_col + single_col * 5, (uchar *)num[num_pointones]);


}


void lcd_uint16(uchar page, uchar column, uint16_t data)
{
	uchar single_col = 5;
	uint16_t i = 0;
	uchar num_col;
	num_col = column;

	for(i = 10000; i > 0; i /= 10)
	{
		display_graphic_5x8(page, num_col, (uchar *)num[(data / i)%10]);
		num_col += single_col;
	}
}