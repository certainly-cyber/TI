/*
 * 12864.h
 *
 *  Created on: 2020年8月16日
 *      Author: certainly
 */
#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long
void LCDPort_Init();
void LCD_Write_cmd(uchar cmd);
void Write_cmd(uchar cmd);
void LCD_Write_Byte(uchar byte);
void Write_data(uchar dat);
void Write_char(uchar ch);
void LCD_Init();
void LCD_pos(uchar x,uchar y);
void Write_pos(uchar x,uchar y);
void Set_pos(uchar x,uchar y);
void LCD_flash(uchar times);
void LCD_OFF();
void drawdot_12864(unsigned char y,unsigned char x,unsigned char type);
unsigned char reddat_12864(void);
void clnGDR_12864(void);//清屏函数
void CheckBusy( void ) ;
void axis();
void lcdshow_fhigh(int a);
void lcdshow_flow(int a);
void LCD_ShowChar(char x, char y, char ch1, char ch2);
void LCD_ShowCharCH(char x, char y, char* CH);
void LCD_ShowStrCH(char x, char y, char* ch, char num);
void LCD_ShowStr(char x, char y, char* ch);
void Str2IN1(char a1, char a2);
void LCD_ClearPic(void);//图像专用清屏
void Disp_HZ(uchar addr, const uchar* pt, uchar num);
void ShowIMG(const uchar* ptr, uchar h, uchar w);
#define List0   0x80
#define List1   0x90
#define List2   0x88
#define List3   0x98






