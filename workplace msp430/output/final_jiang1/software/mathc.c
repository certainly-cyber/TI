/*
 * mathc.c
 *
 *  Created on: 2020年9月18日
 *      Author: certainly
 */
#include "math.h"
#include "mathc.h"

#define pai 3.1415926535859
float b_final=0;
float findmax(float fft_final[])
{   int i=0;
    float a=fft_final[0];
    for(i=1;i<32;i++)
    {

    if(fft_final[i]>a){a=fft_final[i];}
    }
    return a;
}
float findmin(float fft_final[])
{   int i=0;
    float a=fft_final[0];
    for(i=1;i<31;i++)
    {
    if(fft_final[i]<a){a=fft_final[i];}
    }
    return a;
}
int zeronumber(float fft_final[],float max)
{
       int i=0;
       int number=0;
       for(i=1;i<32;i++)
       {
       if(max-fft_final[i]<0.03){number=number+1;}
       }
       if(number>zero_max){;}
       else{number=0;}
       return number;
}
float xielvcompose(float max,float min,float time)
{
float cha=max-min;
float xielv=cha/time;
return xielv;
}
float voltage_measure(int flag,int flagtan,float i,float max,float min,int number)
{   float a=0;
    if(flag==1)//余弦波
    {a=(max-min)*1/2*sin(10000*pai*i);}
    if(flag==2)//三角波
    {float xielv_tan=xielvcompose(max,min,0.0001);
    if(flagtan==0)
    {
    a=min+xielv_tan*i;
    }
    if(flagtan==1)
    {
    a=max-xielv_tan*i;
    }
    }
    if(flag==3)
    {
        b_final=((float)number)/32;
       a=b_final*200;
    }
    return a;
    }


