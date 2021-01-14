/*
 * capture.c
 *
 *  Created on: 2020年8月29日
 *      Author: DELL
 */
#include <msp430.h>
#include "capture.h"

volatile int flag;
volatile unsigned int flag1;
volatile unsigned int count;
volatile unsigned int count_end;
volatile unsigned int count_difference;
volatile unsigned long count_average;
volatile float count_unit;
volatile float count_last;
volatile float frequency;
float count_back[10]={0};
unsigned int overflow;
unsigned int i;

void Capture_Init(void)
{

  P1SEL = BIT2;                             // P1.2是TA0的CCI1A

  TA0CCTL1 = CAP + CCIE + CM_1 + SCS;       // 开启捕获模式， CCR1 interrupt enabled
                                            // 上升沿捕获，CCI1A，同步采集
  TA0CTL = TASSEL_2 + MC_2;                 // SMCLK, Continuous up

}


// Timer0 A0 interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
   switch(TA0IV)
   {
   case 2:                                  // CCR1 used


        if(flag%2==0)
        {
          count = TA0CCR1;


        }
        if(flag%2 == 1)
                 {
                   count_end = TA0CCR1;
                   count_difference=count_end-count+overflow*65536;
                   count_back[i]=count_difference;

                   flag1=flag1+1;
                   i=i+1;

                 }
        if(flag1==10)
        {
            for(i=0;i<10;i++)
            {count_average+=count_back[i];}
            count_unit=count_average%10;
            count_last=count_average/10;
            count_last=count_last+count_unit/10;
            i=0;
            frequency=(1/count_last)*7.995392*1000000;
            flag1=0;
            flag=-1;
            count_average=0;
            overflow=0;


        }

        flag=flag+1;

        break;

   case 10:

       overflow++;
       break;

   }
}
