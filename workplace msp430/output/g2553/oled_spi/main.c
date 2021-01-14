
#include"MSP430G2553.h"
#include"12864.h"
//#include"adc.h"
#define IDLE 0
#define BUSY 1

void main(void)
{

 WDTCTL = WDTPW + WDTHOLD;//Õ£÷πø¥√≈π∑
  P2DIR |=BIT0+BIT1+BIT2+BIT3;

  LCD_Init();
  unsigned char *str1= "2.45555";
  float a=433.4444;

  LCD_CLS();//«Â∆¡
  decimal_show(0,2,a,3,4);
  //LCD_P8x16Str(0,0,str1);
  //LCD_P8x16Str(0,0,(unsigned char*)"forward");


       while(1)
      {

      }
}
