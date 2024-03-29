/*
 * oled.h
 *
 *  Created on: 2020年8月11日
 *      Author: certainly
 */

#ifndef HARDWARE_OLED_OLED_I2C_H_
#define HARDWARE_OLED_OLED_I2C_H_
#include "driverlib.h"


/*************************I2C_OLED指令集(x到127，y0~7）***********************************
 **********************slave address 0X78*********************************
 *写数据指令0x40 *****顺序***     0x78, 0x40,  data
 *写命令指令0x00 *****顺序***     0x78, 0x00,  com
 *命令：设置对比度  0x81，com****顺序****0x78,0x00,0x81    0x78,0x00,com       com越大越亮
 *命令：关/开显示 0XAE/0XAF ****顺序***0X78,0X00,0XAE/0XAF           AE 关  AF 开
 *命令：电荷泵开启  8D *********顺序***0x78,0x00,0x8D 0x78,0x00,0X14          显示时必须开启
 *命令：设置页地址 0XB0+y********顺序*****0x78,0x00,0xb0+y            y对应0到7页（第0到7行）
 *命令：设置列地址高4位((x&0xf0)>>4)|0x10)****顺序*****0X78,0X00,((x&0xf0)>>4)|0x10)    X对应列地址
 *命令：设置列地址低4位((x&0x0f)|0x01)     ****顺序*****0X78,0X00,((x&0x0f)|0x01)       X对应列地址                          X对应列地址
 *命令：设置列地址高4位0x10+x1        设置列地址低4位0x00+x0                X1、x2分别为X的高4位及低4位
 */
#define SlaveAddress   0x78   //oled ADDRESS
typedef unsigned char u8;
typedef unsigned int u16;
#define PORT_OLED   P1OUT   //总端口号
#define DIR_OLED    P1DIR   //端口方向

#define SDA_Value_OLED  (P1IN & BIT6)    //读取SDA引脚的电平值，读之前端口方向设为输入!!!!!!

#define SCK_I2C_OUT_OLED DIR_OLED  |=BIT5
#define SCK_I2C_H_OLED   PORT_OLED  |= BIT5  // SCK
#define SCK_I2C_L_OLED   PORT_OLED  &= ~BIT5

#define SDA_DIR_OUT_OLED    DIR_OLED  |= BIT6     //数据端口方向设为输出   SDA
#define SDA_DIR_IN_OLED     DIR_OLED  &= ~BIT6    //数据端口方向设为输入
#define SDA_I2C_H_OLED      PORT_OLED  |= BIT6
#define SDA_I2C_L_OLED      PORT_OLED  &= ~BIT6

extern u8 OLED_GRAM[128][8];     //显存

void Single_Write_OLED(u8 REG_Address,u8 REG_data); //ADDRESS WRITE DATA

void I2C_OLED_Init(void);                                   //OLED初始化

void OLED_WRITE_COM(u8 com);                                //写命令
void OLED_WRITE_dat(u8 data);                               //写数据
void OLED_Set_Pos(u8 x,u8 y);                               //设置OLED坐标
void OLED_All(u8 bmp_dat);                                  //OLED全屏写同一数据
void OLED_CLC(void);                                        //清屏
void OLED_P8x16Str(u8 x,u8 y, u8 ch[]);                     //显示8*16一组标准ASCII字符串
void OLED_P6x8Str(u8 x,u8 y, u8 ch[]);                      //显示6*8一组标准ASCII字符串

/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
/*****************细节描述:只显示codetab.h文件中F16x16[]数组内点阵，num为显示字模在数组中的位置**********/
void OLED_P16x16Ch(u8 x,u8 y,u8 num);

/******功能描述：显示显示BMP图片128×64   全屏显示*********************/
void Draw_BMP(const u8 *BMP);
/******功能描述：显示小数，m为整数部分位数，n为小数部分位数*********************/
int decimal(float a,int n);
void decimal_show(u8 x,u8 y,float a,int n);
/*************注意！以下函数不可用！G2533 RAM太小********************/
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);             //画对角线
void OLED_DrawPoint(u8 x,u8 y,u8 t);                        //画点
void OLED_Refresh_Gram(void);                               //更新显存到LCD






#endif /* HARDWARE_OLED_OLED_I2C_H_ */
