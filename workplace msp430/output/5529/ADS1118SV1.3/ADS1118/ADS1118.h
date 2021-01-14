
/*
 * ADS1118.h
 *
 *  Created on: 2020年9月3日
 *      Author: lenovo
 */

#ifndef ADS1118_DRIVER_ADS1118_H_
#define ADS1118_DRIVER_ADS1118_H_

#include "driverlib.h"
#include "delay.h"
#include <msp430f5529.h>
#include <stdint.h>


//SSPI
#define      ADS1118_CS         BIT0
#define      ADS1118_CLK        BIT1
#define      ADS1118_OUT        BIT3 //ADS1118数据输出口 MSP430数据输入口
#define      ADS1118_IN         BIT2 //ADS1118数据接受口 MSP430数据输出口


#define      ADS1118_Port_OUT   P6OUT
#define      ADS1118_Port_DIR   P6DIR
#define      ADS1118_Port_IN    P6IN

#define      ADS1118_CS_OUT     (ADS1118_Port_DIR|=ADS1118_CS)
#define      SET_ADS1118_CS     (ADS1118_Port_OUT|=ADS1118_CS)
#define      CLR_ADS1118_CS     (ADS1118_Port_OUT&=~ADS1118_CS)

#define      ADS1118_CLK_OUT    (ADS1118_Port_DIR|=ADS1118_CLK)
#define      SET_ADS1118_CLK    (ADS1118_Port_OUT|=ADS1118_CLK)
#define      CLR_ADS1118_CLK    (ADS1118_Port_OUT&=~ADS1118_CLK)

#define      ADS1118_OUT_IN     (ADS1118_Port_DIR&=~ADS1118_OUT)
#define      ADS1118_OUT_Val    (ADS1118_Port_IN&ADS1118_OUT)

#define      ADS1118_IN_OUT     (ADS1118_Port_DIR|=ADS1118_IN)
#define      SET_ADS1118_IN     (ADS1118_Port_OUT|=ADS1118_IN)
#define      CLR_ADS1118_IN     (ADS1118_Port_OUT&=~ADS1118_IN)



/********************************************************************
*
* This file supports assembler and C development for
* MSP430G2553 devices.
*
* ADS1118.h
*
* functions is defined for ADS1118 configuration and data capture.
*
* Texas Instruments, Version 1.0
*
* Rev. 1.0, Setup
*
*
*
*
*ADS1118
********************************************************************/
extern volatile unsigned int  flag;     //global flag.


#define ADS1118_OS             (0x8000)         //
#define ADS1118_CH0            (0x0000)         //差分输入端口:AINP is AIN0 and AINN is AIN1 (
#define ADS1118_CH1            (0x3000)         //差分输入端口:AINP is AIN2 and AINN is AIN3

#define ADS1118_SC0            (0x4000)         //差分输入端口:AINP is AIN2 and AINN is AIN3
#define ADS1118_SC1            (0x5000)         //差分输入端口:AINP is AIN2 and AINN is AIN3
#define ADS1118_SC2            (0x6000)         //差分输入端口:AINP is AIN2 and AINN is AIN3
#define ADS1118_SC3            (0x7000)         //差分输入端口:AINP is AIN2 and AINN is AIN3


#define ADS1118_GAIN0          (0x0000)         //6.144
#define ADS1118_GAIN1          (0x0200)         //4.096
#define ADS1118_GAIN2          (0x0400)         //2.048
#define ADS1118_GAIN4          (0x0600)         //1.024
#define ADS1118_GAIN8          (0x0800)         //0.512
#define ADS1118_GAIN16         (0x0A00)         //0.256
#define ADS1118_PWRDOWN        (0x0100)         //power_down

#define ADS1118_RATE8SPS       (0x0000)         //
#define ADS1118_RATE16SPS      (0x0020)         //
#define ADS1118_RATE32SPS      (0x0040)         //
#define ADS1118_RATE64SPS      (0x0060)         //
#define ADS1118_RATE128SPS     (0x0080)         //
#define ADS1118_RATE250SPS     (0x00A0)         //
#define ADS1118_RATE475SPS     (0x00C0)         //
#define ADS1118_RATE860SPS     (0x00E0)         //

#define ADS1118_TS             (0x0010)         //
#define ADS1118_PULLUP         (0x0008)         //
#define ADS1118_NOP            (0x0002)         //
#define ADS1118_CNVRDY         (0x0001)         //
//Set the configuration to AIN0/AIN1, FS=+/-0.256, SS, DR=128sps, PULLUP on DOUT
#define ADSCON_CH0      (0x8B8A)
//Set the configuration to AIN2/AIN3, FS=+/-0.256, SS, DR=128sps, PULLUP on DOUT
#define ADSCON_CH1      (0xBB8A)

//常用配置 =  4.096V
#define ADSCON_SC0OP      (ADS1118_SC0+ADS1118_OS+ADS1118_GAIN1+ADS1118_RATE8SPS+ADS1118_PULLUP+ADS1118_NOP)

//常用配置 =  2.048V
//#define ADSCON_SC0OP      (ADS1118_SC0+ADS1118_OS+ADS1118_GAIN2+ADS1118_RATE8SPS+ADS1118_PULLUP+ADS1118_NOP)



//常用配置 =  2.048V 差分设置
#define ADSCON_CH0OP      (ADS1118_CH0+ADS1118_OS+ADS1118_GAIN2+ADS1118_RATE8SPS+ADS1118_PULLUP+ADS1118_NOP)


//void initSPI();
//int WriteSPI(unsigned int config, int mode);
//int ReadConfig(unsigned int config, int mode);
//void ADS_Config(unsigned int mode);
//int ADS_Read(unsigned int mode);
//int ADC_code2temp(int code);            // transform ADC code for far-end to temperature.
//int local_compensation(int local_code); // transform from local sensor code to thermocouple's compensation code.
//void GPIO_INIT();
void S_GPIO_INIT();
int S_ADS1118_ReadByte(unsigned int config);
int S_ADS1118_Read(unsigned int config);

#endif /* ADS1118_DRIVER_ADS1118_H_ */
