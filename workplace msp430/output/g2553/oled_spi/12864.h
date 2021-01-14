#ifndef _12864_H_
#define _12864_H_
#include"stdio.h"
//管教定义；
#define byte  unsigned char
#define word  unsigned int
#define dword unsigned long

#define DC_CLR P2OUT &=~BIT0
#define DC_SET P2OUT |=BIT0

#define RST_CLR P2OUT &=~BIT1//复位按键
#define RST_SET P2OUT |=BIT1

#define SDA_CLR P2OUT &=~BIT2
#define SDA_SET P2OUT |=BIT2

#define SCL_CLR P2OUT &=~BIT3
#define SCL_SET P2OUT |=BIT3

#define X_WIDTH 128
#define Y_WIDTH 64
//函数声明


void delay(int ms);//延时
 void LCD_WrCmd(unsigned char cmd);
 void LCD_WrDat(unsigned char dat);
 void LCD_Init(void);
 void LCD_CLS(void);
 void OLED_P6x8Str();
 void LCD_P6x8num(unsigned char x,unsigned char y, unsigned char ch[]);
 void LCD_P8x16Str(byte x,byte y,byte *ch);
 void LCD_P16x16Ch(byte x,byte y,byte count);
 void LCD_Fill(byte dat);
 void Draw_LibLogo(void);
 void Draw_BMP(byte x0,byte y0,byte x1,byte y1);
 /******功能描述：显示小数，m为整数部分位数，n为小数部分位数*********************/
 int decimal(float a,int n);
 void decimal_show(unsigned char x,unsigned char y,float a,int m,int n);
#endif



