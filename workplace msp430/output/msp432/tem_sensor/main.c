#include <useri2c.h>
#include "msp.h"
#include "driverlib.h"
#include "delay.h"
#include "oled_i2c.h"
#include "tem_sensor.h"
unsigned int Tem_wuti;
float finaltem_wuti;
unsigned int Tem_huanjing;
float finaltem_huanjing;
/**
 * main.c
 */
void main(void)
{
    delay_init();
    oled_i2c_init();
    tem_i2c_init();
    I2C_OLED_Init();
    OLED_All(0);//«Â∆¡
    OLED_P8x16Str(0,0,(unsigned char*)"temwuti:");
//    OLED_P8x16Str(70,0,(unsigned char*)"0.0000");
    OLED_P8x16Str(0,4,(unsigned char*)"temhuan:");
//    OLED_P8x16Str(70,4,(unsigned char*)"0.0000");
    while(1)
    {
        Tem_wuti=memread_wuti();
        finaltem_wuti= transmit(Tem_wuti);
        delay_ms(200);
        decimal_show(0,2,finaltem_wuti,2);
        Tem_huanjing=memread_huanjing();
        finaltem_huanjing= transmit(Tem_huanjing);
        delay_ms(200);
        decimal_show(0,6,finaltem_huanjing,2);


    }

}
