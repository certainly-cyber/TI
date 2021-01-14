/*
 * fft.h
 *
 *  Created on: 2020年8月30日
 *      Author: DELL
 */
#ifndef __FFT_H__

#define __FFT_H__



typedef struct complex //复数类型

{

  float real;       //实部

  float imag;       //虚部

}complex;

//#define FFT_N 128                   //定义福利叶变换的点数
#define PI 3.1415926535897932384626433832795028841971

//交换
#define SWAP(a,b)  tempr=(a);(a)=(b);(b)=tempr

///////////////////////////////////////////

void conjugate_complex(int n,complex in[],complex out[]);

void c_plus(complex a,complex b,complex *c);//复数加

void c_mul(complex a,complex b,complex *c) ;//复数乘

void c_sub(complex a,complex b,complex *c); //复数减法

void c_div(complex a,complex b,complex *c); //复数除法

void fft(int p,complex f[]);

void ifft(int p,complex f[]);  // 傅里叶逆变换

void c_abs(complex f[],float out[],int n);//复数数组取模

void reveal_bus(float a);

////////////////////////////////////////////

#endif
