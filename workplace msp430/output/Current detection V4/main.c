#include <ad12.h>
#include <msp430.h> 
#include "capture.h"
#include "math.h"
#include <stdlib.h>
#include "fft.h"
#include "sys.h"
#include "ad12.h"


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
    volatile float dc[FFT_N]={0};                     //直流量
    volatile float fh = 0;                     //基波
    volatile float sh = 0;                     //二次谐波

     WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
     Init_20MHz_MCLK();                         /*------选择系统主时钟为20MHz-------*/
     Capture_Init();                           //初始化TA0的捕获模式由P1.2角捕获信号
     Init_AD();
     _EINT();
     while(1)
     {
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

         dc[0]=data[0]/FFT_N;
         for(c=1;c<FFT_N;c++)
         {
             dc[c]=data[c]/FFT_N * 2;
         }

         fft_flag=0;
         ADC12IE = BIT0;

         _EINT();


       }

     }
}
