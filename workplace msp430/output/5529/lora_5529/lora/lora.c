/*
 * lora.c
 *
 *  Created on: 2020年8月21日
 *      Author: certainly
 */
#include "driverlib.h"
#include "lora.h"
void lora_init()//普通模式
{
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
}



