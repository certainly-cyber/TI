/*
 * delay.h
 *
 *  Created on: 2016年5月17日
 *      Author: xiaomo
 */

#ifndef DELAY_H_
#define DELAY_H_

#include"msp430g2553.h"
#ifndef u8
#define u8 unsigned char
#endif
#ifndef u16
#define u16 unsigned int
#endif

#ifndef u32
#define u32 unsigned long
#endif

#define DCO_16M                           //DCO为16M  MCLK=SMCLK
//#define DCO_12M                       //DCO为12M  MCLK=SMCLK
//#define DCO_8M                        //DCO为8M  MCLK=SMCLK
//#define DCO_1M                      //DCO为1M  MCLK=SMCLK
#ifdef DCO_16M
#define CPU_F ((double)16000000)//cpu 16000000HZ


#elif defined DCO_12M
#define CPU_F ((double)12000000)//cpu 12000000HZ
#define system_clock()  { WDTCTL = WDTPW | WDTHOLD;BCSCTL1=CALBC1_12MHZ;DCOCTL =CALDCO_12MHZ;}

#elif defined DCO_8M
#define CPU_F ((double)8000000)//cpu 8000000HZ
#define system_clock()  { WDTCTL = WDTPW | WDTHOLD;BCSCTL1=CALBC1_8MHZ;DCOCTL =CALDCO_8MHZ;}


#elif defined DCO_1M
#define CPU_F ((double)1000000)//cpu 1000000HZ


#endif

#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))




#endif /* DELAY_H_ */
