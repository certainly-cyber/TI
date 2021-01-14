#include <msp430.h> 
#include "oled_i2c.h"
#include "zimo.h"
#include "watchdog.h"
#include "exti_interrupt.h"
#include "mathclub.h"
/**
 * main.c
 */
int i=0;
int flag=0;
//   float a[2]={2.1,3.1};
//float a=1.999;
int main(void)
{
watchdog_init();
InitSystemClock();
exti_interrupt_init();
I2C_OLED_Init();
OLED_All(0);//清屏
OLED_P8x16Str(0,0,(unsigned char*)"Voltage:");
OLED_P8x16Str(70,0,(unsigned char*)"0.0000");
OLED_P8x16Str(0,2,(unsigned char*)"Current:");
OLED_P8x16Str(78,2,(unsigned char*)"0.000");
OLED_P8x16Str(0,4,(unsigned char*)"Resistor:");
OLED_P8x16Str(78,4,(unsigned char*)"0.000");
//decimal_show(70,0,a,1,3);
//float d=urms(2,a);
__enable_interrupt();
while(1)
{
 while(flag==1)
 { __bis_SR_register(LPM3_bits + GIE);       }// Enter LPM3 w/ interrupt
 if(flag==0)
 {
     OLED_P8x16Str(0,6,(unsigned char*)"Nor-power-mode");
 }

}
	
	return 0;
}






// Watchdog Timer interrupt service routine
//1 min会进入LPM3
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{

i=i+1;
if(i==60)
{
  i=0;
  flag=1;
  OLED_P8x16Str(0,6,(unsigned char*)"Low-power-mode");//显示进入低功耗

}
}
// exti_interrupt service routine(port1)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
   {    unsigned int num1;
        unsigned char Pushkey1;
        Pushkey1=P1IFG&(BIT2+BIT3+BIT4+BIT5);
        for(num1=0;num1<1000;num1++){};
        unsigned char state1=P1IN&Pushkey1;
        if(state1==Pushkey1)//注意这里一定要分开去写，不要连等
        {P1IFG=0;return;}
        if(P1IFG&BIT2)//按键被按下，写在这里
        {}
        if(P1IFG&BIT3)
        {}
        if(P1IFG&BIT4)
        {}
        if(P1IFG&BIT5)
        {}
        P1IFG=0;
        if(flag==1)
        { LPM3_EXIT;}

        flag=0;
        i=0;
        return;

}
// exti_interrupt service routine(port2)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
   {
        unsigned int num2;
        unsigned char Pushkey2;
        Pushkey2=P2IFG&(BIT0+BIT1);
        for(num2=0;num2<1000;num2++){};
        unsigned char state2=P2IN&Pushkey2;
        if(state2==Pushkey2)
        {P2IFG=0;return;}
        if(P2IFG&BIT0)//按键被按下，写在这里
        {}
        if(P2IFG&BIT1)
        {}
        P2IFG=0;
        if(flag==1)
        { LPM3_EXIT;}

        flag=0;
        i=0;
        return;

}
