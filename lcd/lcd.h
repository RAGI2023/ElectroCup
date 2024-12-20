#ifndef _MSP_LCD_H_
#define _MSP_LCD_H_

#include <stdint.h>

typedef uint16_t uint;
typedef uint8_t uchar;
typedef uint32_t ulong;

void delay(uint time);
void initial_lcd();
void clear_screen();
void display_128x64(uchar *dp);
void display_graphic_16x16(uchar page, uchar column, uchar *dp);
void display_graphic_5x8(uchar page, uchar column, uchar *dp);
void display_graphic_64x32(uchar page, uchar column, uchar *dp);
void transfer_command_lcd(int data1);
void lcd_address(uint page, uint column);
void transfer_data_lcd(int data1);
void display_graphic_1X8(uchar page, uchar column, uchar *dp);
void lcd_deltaphi(uchar page, uchar column, float deltaphi);
void lcd_uint16(uchar page, uchar column, uint16_t data);
void lcd_uint8(uchar page, uchar column, uint8_t data);
void lcd_deltaphi_nozero(uchar page, uchar column, float deltaphi);


#endif
