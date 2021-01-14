/*
 * fft.c
 *
 *  Created on: 2020年8月30日
 *      Author: DELL
 */



#include "math.h"

#include "fft.h"

//精度0.0001弧度


////求共轭复数
void conjugate_complex(int n,complex in[],complex out[])

{

  int i = 0;

  for(i=0;i<n;i++)

  {

    out[i].imag = -in[i].imag;

    out[i].real = in[i].real;

  }

}


//求绝对值
void c_abs(complex f[],float out[],int n)

{

  int i = 0;

  float t;

  for(i=0;i<n;i++)

  {

    t = f[i].real * f[i].real + f[i].imag * f[i].imag;

    out[i] = sqrt(t);

  }

}




//求和
void c_plus(complex a,complex b,complex *c)

{

  c->real = a.real + b.real;

  c->imag = a.imag + b.imag;

}


//相减
void c_sub(complex a,complex b,complex *c)

{

  c->real = a.real - b.real;

  c->imag = a.imag - b.imag;

}


//相乘
void c_mul(complex a,complex b,complex *c)

{

  c->real = a.real * b.real - a.imag * b.imag;

  c->imag = a.real * b.imag + a.imag * b.real;

}


//相除
void c_div(complex a,complex b,complex *c)

{

  c->real = (a.real * b.real + a.imag * b.imag)/(b.real * b.real +b.imag * b.imag);

  c->imag = (a.imag * b.real - a.real * b.imag)/(b.real * b.real +b.imag * b.imag);

}






void Wn_i(int n,int i,complex *Wn,char flag)

{

  Wn->real = cos(2*PI*i/n);

  if(flag == 1)

  Wn->imag = -sin(2*PI*i/n);

  else if(flag == 0)

  Wn->imag = -sin(2*PI*i/n);

}



//傅里叶变化
void fft(int p,complex f[])
{

  complex t,wn;//中间变量

  int i,j,k,m,n,l,r,M;

  int la,lb,lc;

  /*----计算分解的级数M=log2(N)----*/

  for(i=p,M=1;(i=i/2)!=1;M++);

  /*----按照倒位序重新排列原信号----*/

  for(i=1,j=p/2;i<=p-2;i++)

  {

    if(i<j)

    {

      t=f[j];

      f[j]=f[i];

      f[i]=t;

    }

    k=p/2;

    while(k<=j)

    {

      j=j-k;

      k=k/2;

    }

    j=j+k;

  }

  // -----FFT算法-----//

  for(m=1;m<=M;m++)

  {

    la=pow(2,m); //la=2^m代表第m级每个分组所含节点数

    lb=la/2;    //lb代表第m级每个分组所含碟形单元数

                 //同时它也表示每个碟形单元上下节点之间的距离

    /*----碟形运算----*/

    for(l=1;l<=lb;l++)

    {

      r=(l-1)*pow(2,M-m);

      for(n=l-1;n<p-1;n=n+la) //遍历每个分组，分组总数为N/la

      {

        lc=n+lb;  //n,lc分别代表一个碟形单元的上、下节点编号

        Wn_i(p,r,&wn,1);//wn=Wnr

        c_mul(f[lc],wn,&t);//t = f[lc] * wn复数运算

        c_sub(f[n],t,&(f[lc]));//f[lc] = f[n] - f[lc] * Wnr

        c_plus(f[n],t,&(f[n]));//f[n] = f[n] + f[lc] * Wnr

      }

    }

  }

}



//傅里叶逆变换

void ifft(int p,complex f[])

{

  int i=0;

  conjugate_complex(p,f,f);

  fft(p,f);

  conjugate_complex(p,f,f);

  for(i=0;i<p;i++)

  {

    f[i].imag = (f[i].imag)/p;

    f[i].real = (f[i].real)/p;

  }

}





void reveal_bus(float a)
{
     volatile unsigned char b[4];
     a=a*1.7;
     b[0]=(unsigned char) a/1000;
     b[1]=((unsigned char)a/100)%10;
     b[2]=((unsigned char)a/10)%10;
     b[3]=((unsigned char)a)%10;
     //上面拆分好了，可以用来放在显示屏上
}


