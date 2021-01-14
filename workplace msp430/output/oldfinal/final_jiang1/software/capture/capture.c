/*
 * capture.c
 *
 *  Created on: 2020年8月29日
 *      Author: DELL
 */
#include <msp430.h>
#include "capture.h"
#include "ad12.h"

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
       flag1=1;
       break;


   case 10:
       break;

   }
}
