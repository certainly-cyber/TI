/*
 * ADS1118.c
 *
 *  Created on: 2020年9月3日
 *      Author: lenovo
 */


#include <ADS1118.h>

static int msb;
//static unsigned int temp;
volatile unsigned int flag;
static uint8_t dummy1, dummy2, dummy3, dummy0 = 0;


void S_GPIO_INIT()
{
    ADS1118_CS_OUT;
    ADS1118_CLK_OUT;
    ADS1118_IN_OUT;
    ADS1118_OUT_IN;
    CLR_ADS1118_CS;
    _NOP();
    CLR_ADS1118_CLK;
    _NOP();
    CLR_ADS1118_IN;
    _NOP();
}


int S_ADS1118_ReadByte(unsigned int config)
{
    unsigned char i, temp, Din;
    temp = config;
    for (i = 0; i < 8; i++)
    {
        Din = Din << 1;
        if (0x80 & temp)
            SET_ADS1118_IN;
        else
            CLR_ADS1118_IN;
        delay_us(1);
        SET_ADS1118_CLK;
        delay_us(1);
        if (ADS1118_OUT_Val)
            Din |= 0x01;
        delay_us(1);
        CLR_ADS1118_CLK;
        delay_us(1);
        temp = (temp << 1);
    }
    return Din;
}


int S_ADS1118_Read(unsigned int config)
{
    unsigned int temp = 0;

    //发送MSB数据 接受测量值MSB数据
    temp = (config & 0xFF00) >> 8;
    dummy0 = S_ADS1118_ReadByte(temp);
    //发送LSB数据 接受测量值LSB数据
    temp = config & 0x00FF;
    dummy1 = S_ADS1118_ReadByte(temp);

    //重发数据配置 读回配置数据
    //发送MSB数据 接受MSB数据
    temp = (config & 0xFF00) >> 8;
    dummy2 = S_ADS1118_ReadByte(temp);
    //发送LSB数据 接受LSB数据
    temp = config & 0x00FF;
    dummy3 = S_ADS1118_ReadByte(temp);


    //接受配置信息 检验正确性
    temp = ((dummy2 << 8) | dummy3);
    temp |= 0x8000;
//    a = (config == temp);
    if(config == temp)
    {
        msb = (unsigned int)((dummy0<<8) | dummy1);
        P1OUT |= BIT0;
    }
    else
    {
        msb = 0x0; // error on the SPI Tx/Rx
    }
    return msb;
}

