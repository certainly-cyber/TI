/*
 * main.c
 *
 *  Created on: 2020年10月5日
 *      Author: certainly
 */
#include "driverlib.h"
#include "delay.h"
#include "useri2c.h"
#include "lmt70.h"
#include "usart.h"

/**
 * main.c
 */
//通过串口1实现温度的输出，波特率：9600/8N1
float tem_show=0;
void main(void)
{
	delay_init();//关闭看门狗，升频
	lmt70_i2c_init();
 	lmt70_init();
 	usart_init();
	while(1)
	{
    tem_show=tem_get();
	printf("%f\r\n",tem_show );
	delay_ms(500);

	}

}
