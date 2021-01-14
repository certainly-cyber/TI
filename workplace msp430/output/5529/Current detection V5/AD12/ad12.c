/*
 * ad12.c
 *
 *  Created on: 2020年9月2日
 *      Author: DELL
 */

#include <msp430.h>
#include "fft.h"
#include "ad12.h"

unsigned char fft_flag=0;
struct complex h[FFT_N];

unsigned int AD_capture(void)
{
    while (!(0x01 & ADC12IFG)); //SCF：转换序列完成标志位。当一次转换序列完成后，
                        //标志位置1等待AD转换完成

    return( ADC12MEM0);          //返回转换结果变量AD_data
}


void Init_AD(void)
{
  P6SEL |= BIT0;                            // Enable A/D channel A0
  ADC12CTL0 = ADC12ON + ADC12SHT0_4  + ADC12MSC ;        // 开启ADC12, 设置样本时间，在第一个上升沿后自动转换。
  ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2 ;       // Use sampling timer, 重复单通道转换
//  ADC12IE = BIT0;                           // Enable ADC12IFG.0
  ADC12CTL0 |= ADC12ENC;                      // Enable conversions
//  ADC12CTL0 |= ADC12SC;                       // Start conversion

}



#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
    static unsigned char i;
 // volatile  float s;
  switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                           // Vector  0:  No interrupt
  case  2: break;                           // Vector  2:  ADC overflow
  case  4: break;                           // Vector  4:  ADC timing overflow
  case  6:
//     s = (float) ADC12MEM0*0.00080078125;      // Vector  6:  ADC12IFG0
    h[i].real=AD_capture();
    h[i++].imag=0;
     if(i==FFT_N)
     {
       fft_flag=1;
       ADC12IE &=~ BIT0;
     }
  case  8: break;                         // Vector  8:  ADC12IFG1
  case 10: break;                           // Vector 10:  ADC12IFG2
  case 12: break;                           // Vector 12:  ADC12IFG3
  case 14: break;                           // Vector 14:  ADC12IFG4
  case 16: break;                           // Vector 16:  ADC12IFG5
  case 18: break;                           // Vector 18:  ADC12IFG6
  case 20: break;                           // Vector 20:  ADC12IFG7
  case 22: break;                           // Vector 22:  ADC12IFG8
  case 24: break;                           // Vector 24:  ADC12IFG9
  case 26: break;                           // Vector 26:  ADC12IFG10
  case 28: break;                           // Vector 28:  ADC12IFG11
  case 30: break;                           // Vector 30:  ADC12IFG12
  case 32: break;                           // Vector 32:  ADC12IFG13
  case 34: break;                           // Vector 34:  ADC12IFG14
  default: break;
  }
}
