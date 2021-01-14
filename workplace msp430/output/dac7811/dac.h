/*
 * dac.h
 *
 *  Created on: 2020Äê9ÔÂ18ÈÕ
 *      Author: DELL
 */

#ifndef DAC_H_
#define DAC_H_



#define dac_PORT          P6DIR
#define dac_OUT           P6OUT
#define dac_CLK           BIT2
#define dac_SYNC          BIT3
#define dac_DIN           BIT1
#define dac_CLK_HIGH      dac_OUT |= dac_CLK;
#define dac_CLK_LOW       dac_OUT &=~ dac_CLK;
#define dac_SYNC_HIGH     dac_OUT |= dac_SYNC;
#define dac_SYNC_LOW      dac_OUT &=~ dac_SYNC;
#define dac_DIN_HIGH      dac_OUT |= dac_DIN;
#define dac_DIN_LOW       dac_OUT &=~ dac_DIN;

void writedata(unsigned int a);
void adc_Init(void);


#endif /* DAC_H_ */
