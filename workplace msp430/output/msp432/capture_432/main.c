#include "msp.h"

/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 * MSP432 GPIO - Toggle Output High/Low
 *
 * Description: In this very simple example, the LED on P1.0 is configured as
 * an output using DriverLib's GPIO APIs. An infinite loop is then started
 * which will continuously toggle the GPIO and effectively blink the LED.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P1.0  |---> P1.0 LED
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 ******************************************************************************/
/* DriverLib Includes */
#include <driverlib.h>
#include <msp.h>
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
int time0=0;
int time1=0;
int a=0;
void capture_init()
{
     TA0CTL=TASSEL_0+MC_2+TAIE+CM_3;
     TA1CTL=TASSEL_0+MC_2+TAIE+CM_3;
     P7DIR&=~BIT1;
     P7SEL1|=BIT1;
     P7DIR&=~BIT2;
     P7SEL1|=BIT2;
     MAP_Interrupt_enableSleepOnIsrExit();
     MAP_Interrupt_enableInterrupt(INT_TA0_N);
     MAP_Interrupt_enableInterrupt(INT_TA1_N);
     MAP_Interrupt_enableMaster();
}
int read_number(int number)//0代表P7.1，1代表P7.1
{
    int all_number=0;;
    if(number==0){all_number=TA0R+time0*65536;   TA0CTL|=TACLR;}
    if(number==1){all_number=TA1R+time1*65536;   TA1CTL|=TACLR;}

    return all_number;
}
void main()
{
  capture_init();
  while(1){
  //a= read_number(0);
  }


}
//中断处理函数
void TA0_N_IRQHandler(void)
{
   time0=time0+1;
   TA0CTL|=TACLR;
}
void TA1_N_IRQHandler(void)
{
   time1=time1+1;
   TA1CTL|=TACLR;
}

