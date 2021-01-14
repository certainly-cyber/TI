/*
 * mathc.h
 *
 *  Created on: 2020Äê9ÔÂ18ÈÕ
 *      Author: certainly
 */

#ifndef SOFTWARE_MATHC_H_
#define SOFTWARE_MATHC_H_
#define zero_max 4
float findmax(float fft_final[]);
float findmin(float fft_final[]);
int zeronumber(float fft_final[],float max);
float xielvcompose(float max,float min,float time);
float voltage_measure(int flag,int flagtan,float i,float max,float min,int number);




#endif /* SOFTWARE_MATHC_H_ */
