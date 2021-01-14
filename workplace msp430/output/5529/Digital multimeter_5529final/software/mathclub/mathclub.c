/*
 * mathclub.c
 *
 *  Created on: 2020年8月11日
 *      Author: certainly
 */
#include "mathclub.h"
#include "math.h"
//计算真有效值的公式
float urms(int n,float voltage[])
{
 float sum;
 int i=0;
 for(i=0;i<n;i++)
 {
     float a=voltage[i]*voltage[i];
     sum=sum+a;

 }
 sum=sum/n;
 sum=sqrt(sum);
 return sum;
}



