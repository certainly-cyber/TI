#include <msp430.h> 
#include "driverlib.h"
#include "oled_i2c.h"
#include "tem_sensor.h"
#include "delay.h"
unsigned int Tem;
float finaltem;
unsigned int pwm;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    I2C_OLED_Init();
    OLED_All(0);//«Â∆¡
    OLED_P8x16Str(0,0,(unsigned char*)"temperature:");

    while(1)
    {
        Tem=memread();
        finaltem= transmit(Tem);
        delay_ms(200);
        decimal_show(0,2,finaltem,2);
    }
    return 0;
}
