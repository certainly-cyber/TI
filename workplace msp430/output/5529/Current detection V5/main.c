#include <ad12.h>
#include <msp430.h> 
#include "capture.h"
#include "math.h"
#include <stdlib.h>
#include "fft.h"
#include "sys.h"
#include "ad12.h"
#include "bluetooth.h"
#include "driverlib.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
int blueflag=1;//蓝牙是否连接标志位
float aw=44.944;
uint8_t receivedData = 0x00;
uint8_t transmitData = 0x00;
/**
 * main.c
 */

void main(void)
{
    volatile int   a = 0;
    volatile float b = 0;
    volatile int   c = 0;
    volatile int n=0;
    volatile float data[FFT_N]={0};
    volatile float dc [FFT_N] ={0};            //0是直流量
    volatile float fh = 0;                     //基波
    volatile float sh = 0;                     //二次谐波

     WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
     Init_20MHz_MCLK();                         /*------选择系统主时钟为20MHz-------*/
     Capture_Init();                           //初始化TA0的捕获模式由P1.2角捕获信号
     Init_AD();
     GPIO_init();
     USART_init();
     _EINT();
     while(1)
     {
    //     __bis_SR_register(LPM3_bits + GIE);//低功耗模式，可根据需求选择是否进入
       if(fft_flag==1)
       {

         _DINT();

         for(n=0;n<FFT_N;n++)
         {
             h[n].real = (float)h[n].real*0.00080078125;
         }

         fft(FFT_N,h);

         for(a=0;a<FFT_N;a++)
         {
             b = h[a].real*h[a].real+h[a].imag*h[a].imag;
             data[a] = sqrtf(b);
         }

         printf("%f\r\n", dc[0]);

         for(c=1;c<FFT_N;c++)
         {

             printf("%f\r\n", dc[c]);
         }



         fft_flag=0;
         ADC12CTL0 &=~ ADC12SC;

         _EINT();


       }

     }
}
int fputc(int _c, register FILE *_fp)
{
  USCI_A_UART_transmitData(USCI_A0_BASE,_c);


  return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++)
  {
    USCI_A_UART_transmitData(USCI_A0_BASE,_ptr[i]);
  }

  return len;
}
//******************************************************************************
//
//This is the PORT1_VECTOR interrupt vector service routine
//
//******************************************************************************

#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void)
{
    unsigned int temp1;
    int i1;
    for(i1=0;i1<12000;i1++);//延时消抖
    if((P1IN&0xff)!=0xff)
    {

     temp1=P1IFG;
     P1IFG=0x00;
     if(temp1==0x02)
     {
         LPM3_EXIT;
         ADC12IE = BIT0;
         ADC12CTL0 |= ADC12SC;

     }//P1.1被按下
    }

}
//******************************************************************************
//
//This is the PORT2_VECTOR interrupt vector service routine
//
//******************************************************************************
#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void)
{
    unsigned int temp2;
    int i2;
    int i4;
    for(i2=0;i2<12000;i2++);//延时消抖
    if((P2IN&0xff)!=0xff)
    {temp2=P2IFG;
     P2IFG=0x00;
     if(temp2==0x02)
     {   for(i4=0;i4<3;i4++)
     {
         UartPutchar('1');
     }
     blueflag=1;
     }//P2.1被按下
    }

}
//  Echo back RXed character
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR (void)
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);

    switch (__even_in_range(UCA0IV,4)){

        //Vector 2 - RXIFG
        case 2:
            receivedData = USCI_A_UART_receiveData(USCI_A0_BASE);
            if(receivedData=='6'){GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);}
            if(receivedData=='7'){GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);}
            USCI_A_UART_transmitData(USCI_A0_BASE,receivedData);


            break;
        default: break;
    }
}
