/*
 * tem_sensor.h
 *
 *  Created on: 2020年9月5日
 *      Author: certainly
 */
//************** 端口定义**************
#include "delay.h"

#ifndef TEM_SENSOR_TEM_SENSOR_H_
#define TEM_SENSOR_TEM_SENSOR_H_
#define TEM_SlaveAddress   0xB4   //tem ADDRESS

#define PORT_TEM   P2OUT   //总端口号
#define DIR_TEM    P2DIR   //端口方向

#define SDA_Value_TEM  (P2IN & BIT3)    //读取SDA引脚的电平值，读之前端口方向设为输入!!!!!!
#define SCK_I2C_OUT_TEM DIR_TEM  |=BIT2
#define SCK_I2C_H_TEM   PORT_TEM  |= BIT2   // SCK
#define SCK_I2C_L_TEM   PORT_TEM  &= ~BIT2

#define SDA_DIR_OUT_TEM    DIR_TEM  |= BIT3     //数据端口方向设为输出   SDA
#define SDA_DIR_IN_TEM     DIR_TEM  &= ~BIT3    //数据端口方向设为输入
#define SDA_I2C_H_TEM      PORT_TEM  |= BIT3
#define SDA_I2C_L_TEM      PORT_TEM  &= ~BIT3
float transmit(unsigned int a);
void I2C_start_TEM();
void I2C_Stop_TEM();
void I2C_SendACK_TEM(u8 ack);
u8 I2C_RecvACK_TEM();
void I2C_SendByte_TEM(u8 dat);
u8 I2C_RecvByte_TEM();
void Single_Write_TEM(u8 commend,u8 HighBIT,u8 LowBIT);
unsigned int memread(void);



#endif /* TEM_SENSOR_TEM_SENSOR_H_ */
