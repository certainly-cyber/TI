/*
 * adc12.h
 *
 *  Created on: 2020Äê9ÔÂ2ÈÕ
 *      Author: DELL
 */

#ifndef AD12_AD12_H_
#define AD12_AD12_H_

#define FFT_N     32
#define designmax 3

unsigned int AD_capture(void);
void Init_AD(void);


extern struct complex h[128];
extern unsigned char fft_flag;
extern int flag1;


#endif /* AD12_AD12_H_ */
