#include "msp430g2553.h"
#include "oled_i2c.H"
#include "zimo.h"
int main(void)
{ unsigned char *str1= "2.45555";
   float a=433.4444;
   system_clock();
   I2C_OLED_Init();
   OLED_All(0);//清屏
   decimal_show(0,2,a,3,4);
   OLED_P8x16Str(0,4,str1);
   OLED_P8x16Str(0,0,(unsigned char*)"forward");
   while(1)
   {

   }
}
