/*
 * dac.c
 *
 *  Created on: 2020年9月18日
 *      Author: DELL
 */
#include "dac.h"
#include <msp430.h>
#include "delay.h"
#include "driverlib.h"



void adc_Init(void)
{
    dac_PORT |= dac_CLK + dac_SYNC + dac_DIN;

    dac_SYNC_HIGH;                      //先拉高
}


void writedata(unsigned int a)
{
    unsigned char Bit_counter;
    dac_SYNC_LOW;
    for(Bit_counter=16; Bit_counter; Bit_counter--)
     {
        dac_CLK_HIGH;

      if (a&0x8000)
      {
          dac_DIN_HIGH;
      }
       else
       {
           dac_DIN_LOW;
       }
      a<<=1;
    //  delay_us(1);
      dac_CLK_LOW;
   //   delay_us(1);
     }
    dac_SYNC_HIGH;
}
