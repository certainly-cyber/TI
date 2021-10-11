#include "msp.h"
#include "delay.h"
#include "driverlib.h"
#include "oled_i2c.h"
int aclk,mclk,smclk,hsmclk,bclk=0;



/**
 * main.c
 */
void main(void)
{
delay_init();
MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
I2C_OLED_Init();
OLED_All(0);//«Â∆¡
OLED_P8x16Str(0,0,(unsigned char*)"Voltage:");
OLED_P8x16Str(70,0,(unsigned char*)"0.0001");
OLED_P8x16Str(0,2,(unsigned char*)"Current:");
OLED_P8x16Str(78,2,(unsigned char*)"0.000");
OLED_P8x16Str(0,4,(unsigned char*)"Resistor:");
OLED_P8x16Str(78,4,(unsigned char*)"0.000");
while(1){}

}
