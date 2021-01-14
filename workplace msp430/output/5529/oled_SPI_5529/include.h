/*
 * include.h
 *
 *  Created on: 2020Äê8ÔÂ11ÈÕ
 *      Author: certainly
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_
#define uchar unsigned char
#define uint unsigned int


#define u8 unsigned char
#define u32 unsigned int

/*
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long int
*/

#include <intrinsics.h>
#define CPU_F ((double)1048576)//(32.768*1000*(31+1))
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))





#endif /* INCLUDE_H_ */
