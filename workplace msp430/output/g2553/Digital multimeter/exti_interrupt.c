/*
 * exti_interrupt.c
 *
 *  Created on: 2020年8月1日
 *      Author: certainly
 */
#include <msp430.h>
#include "exti_interrupt.h"
void exti_interrupt_init()
{
      P1IE |=  BIT2+BIT3+BIT4+BIT5;                            // interrupt enabled
      P1IES |= BIT2+BIT3+BIT4+BIT5;                             // Hi/lo edge
      P1REN |= BIT2+BIT3+BIT4+BIT5;                            // Enable Pull Up on SW2
//      P1OUT |= BIT2+BIT3+BIT4+BIT5;                            //这里使得配置生效
      P1IFG &= ~(BIT2+BIT3+BIT4+BIT5);                         // IFG cleared
      P2IE |=  BIT0+BIT1;                           // interrupt enabled
      P2IES |= BIT0+BIT1;                              // Hi/lo edge
      P2REN |= BIT0+BIT1;                            // Enable Pull Up on SW2
      P2OUT |= BIT0+BIT1;                            //这里使得配置生效
      P2IFG &= ~(BIT0+BIT1);                         // IFG cleared

}


