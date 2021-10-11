/*
 * fdc2214.h
 *
 *  Created on: 2021年5月4日
 *      Author: certainly
 */
#include "driverlib.h"
#include "delay.h"
#include "stdio.h"

#ifndef FDC2214_FDC2214_H_
#define FDC2214_FDC2214_H_


#define PORT_fdc2214   P2OUT   //总端口号
#define DIR_fdc2214    P2DIR   //端口方向

#define SDA_Value_fdc2214  (P2IN & BIT6)    //读取SDA引脚的电平值，读之前端口方向设为输入!!!!!!

#define SCK_I2C_OUT_fdc2214 DIR_fdc2214  |=BIT5
#define SCK_I2C_H_fdc2214   PORT_fdc2214  |= BIT5  // SCK
#define SCK_I2C_L_fdc2214   PORT_fdc2214  &= ~BIT5

#define SDA_DIR_OUT_fdc2214    DIR_fdc2214  |= BIT6     //数据端口方向设为输出   SDA
#define SDA_DIR_IN_fdc2214    DIR_fdc2214  &= ~BIT6    //数据端口方向设为输入
#define SDA_I2C_H_fdc2214     PORT_fdc2214  |= BIT6
#define SDA_I2C_L_fdc2214     PORT_fdc2214  &= ~BIT6

void IIC_Start_fdc2214();
void IIC_Stop_fdc2214();
u8 IIC_Wait_Ack_fdc2214();
void IIC_Ack_fdc2214(void);
void IIC_NAck_fdc2214(void);
void IIC_Send_Byte_fdc2214(u8 dat);
u8 IIC_Read_Byte_fdc2214(unsigned char ack);
#endif /* FDC2214_FDC2214_H_ */
