#include <msp430g2553.h>
#include <stdint.h>
#include "lcd.h"
#include "lcd/lcd.h"

#define RESET_LOW       P2OUT = P2OUT & (~BIT4)     // 重置设低
#define RESET_HIGH      P2OUT = P2OUT | BIT4        // 重置设高
#define SI_LOW          P2OUT = P2OUT & (~BIT2)     // 串行数据输入低
#define SI_HIGH         P2OUT = P2OUT | BIT2        // 串行数据输入高
#define SCK_LOW         P2OUT = P2OUT & (~BIT1)     // 串行时钟输入低
#define SCK_HIGH        P2OUT = P2OUT | BIT1        // 串行时钟输入高
#define CS_LOW          P2OUT = P2OUT & (~BIT3)     // 片选低
#define CS_HIGH         P2OUT = P2OUT | BIT3        // 片选高
#define A0_LOW          P2OUT = P2OUT & (~BIT5)     // A0低
#define A0_HIGH         P2OUT = P2OUT | BIT5        // A0高

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
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 数字 '0'
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 数字 '1'
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 数字 '2'
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 数字 '3'
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 数字 '4'
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 数字 '5'
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 数字 '6'
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 数字 '7'
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 数字 '8'
    {0x06, 0x49, 0x49, 0x29, 0x1E}  // 数字 '9'
};

// const unsigned char delta[5] = {0x20, 0x50, 0x88, 0x88, 0xF8};
// const unsigned char phi[5] = {0x70, 0x88, 0x88, 0x78, 0x88};
const unsigned char decimal_point[5] = {
    0x00, // 第 1 列
    0x00, // 第 2 列
    0x80, // 第 3 列
    0x00, // 第 4 列
    0x00  // 第 5 列
};
const unsigned char minus_sign[5] = {
    0x10, // 第 1 列
    0x10, // 第 2 列
    0x10, // 第 3 列（负号线）
    0x10, // 第 4 列
    0x10  // 第 5 列
};



// 写指令到 LCD 模块 
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
		// delay(10);  //加少量延时   
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
		// delay(10);  //加少量延时    
        SCK_HIGH;
		data1 = data1 << 1;
	}
	// lcd_cs1 = 1;
    // CS_HIGH;
	SCK_LOW;
}


//写数据到 LCD 模块 
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
		// delay(10);  //加少量延时
		if (data1 & 0x80){
            // lcd_sid = 1;
            SI_HIGH;
        }
		else{
            // lcd_sid = 0;
            SI_LOW;
        }
		// lcd_sclk = 1;
		// delay(10);  //加少量延时
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
	
	// lcd_reset = 0;          //低电平复位  
    // RESET_LOW;
	// delay(100);
	// lcd_reset = 1;       //复位完毕
    RESET_HIGH;  
	// delay(100);
	// transfer_command_lcd(0xe2);  //软复位  
	// delay(5);
	// transfer_command_lcd(0x2c);  //升压步聚 1  
	// delay(50);
	// transfer_command_lcd(0x2e);  //升压步聚 2  
	// delay(50);
	// transfer_command_lcd(0x2f);  //升压步聚 3 
	// delay(5);
	// transfer_command_lcd(0x23);  //粗调对比度，可设置范围 0x20～0x27  
	// transfer_command_lcd(0x81);  //微调对比度  
	// transfer_command_lcd(0x28);  //微调对比度的值，可设置范围 0x00～0x3f  
	// transfer_command_lcd(0xa2);  //1/9 偏压比（bias）  
	// transfer_command_lcd(0xc8);  //行扫描顺序：从上到下  
	transfer_command_lcd(0xa1);  //列扫描顺序：从左到右  
	// transfer_command_lcd(0x40);  //起始行：第一行开始  
	transfer_command_lcd(0xaf);  //开显示 
}


void lcd_address(uint page, uint column)
{
	column = column;
	
	transfer_command_lcd(0x10 + ((column >> 4 )& 0x0f)); //设置列地址的高 4 位  
	transfer_command_lcd(column & 0x0f); //设置列地址的低 4 位  
	transfer_command_lcd(0xb0 + page);   //设置页地址，每 8 行为一页，全屏共 64 行，被分成 8 页 
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


//显示 128x64 点阵图像 
void display_128x64(uchar *dp)
{
	uchar i, j;
	for (j = 0; j<8; j++)
	{
		lcd_address(j + 1, 1);
		for (i = 0; i<128; i++)
		{
			transfer_data_lcd(*dp);     //写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1    
			dp++;
		}
	}
}

//显示 16x16 点阵图像、汉字、生僻字或 16x16 点阵的其他图标 
void display_graphic_16x16(uint8_t page, uint8_t column, uint8_t *dp)
{
	uint8_t i, j;
	for (j = 0; j<2; j++)
	{
		lcd_address(page + j, column);
		for (i = 0; i<16; i++)
		{
			transfer_data_lcd(*dp);     //写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1    
			dp++;
		}
	}
}

//显示 8x16 点阵图像、ASCII, 或 8x16 点阵的自造字符、其他图标 
void display_graphic_8x16(uchar page, uchar column, uchar *dp)
{
	uint i, j;
	for (j = 0; j<2; j++)
	{
		lcd_address(page + j, column);
		for (i = 0; i<8; i++)
		{
			transfer_data_lcd(*dp);     //写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1   
			dp++;
		}
	}
}

//显示 64x32 点阵图像
void display_graphic_64x32(uchar page, uchar column, uchar *dp)
{
	uint i, j;
	for (j = 0; j<4; j++)
	{
		lcd_address(page + j, column);
		for (i = 0; i<64; i++)
		{
			transfer_data_lcd(*dp);     //写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1   
			dp++;
		}
	}
}


//显示 5X8 点阵图像、ASCII, 或 5x8 点阵的自造字符、其他图标 
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