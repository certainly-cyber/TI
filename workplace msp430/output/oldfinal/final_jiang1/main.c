#include <msp430.h> 
#include "driverlib.h"
#include "math.h"
#include "mathc.h"
#include "sys.h"
#include "fft.h"
#include "dac.h"
#include "ad12.h"
#include "capture.h"
#include "delay.h"
int flag_main;
int flag_over=0;
float i_main=0;
int i_main_1=0;
int i_main_2=0;
float return_number;
int return_number2;
int return_number3[200]={0};
float fft_final[32]={0};
float max_main;
float min_main;
float temporary;
int zero_number;
int yuxia=0;
int pingheng=0;
float xielv_main=0;
#define spot 32
volatile float data[32]={0};
int judge(float max,float min,float ftt_final[]);
void main (void)
{
   volatile unsigned int tmp = 0;

   WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
   Init_20MHz_MCLK();
   GPIO_init();
     Init_AD();//开始采样
   Capture_Init();
   _EINT();
   while(fft_flag==0){}//等待采样完成

   for(i_main_1=0;i_main_1<31;i_main_1++)
  {
      temporary =h[i_main_1].real/4095*3.3;//实际采样
     fft_final[i_main_1]=temporary;
   }

   max_main=findmax(fft_final);
   min_main=findmin(fft_final);
   zero_number=zeronumber(fft_final,max_main);

   while(1)
   {         flag_main=judge(max_main,min_main,fft_final);
   flag_main=1;
            adc_Init();
 //           writedata(0x0000);

               _DINT();
               while(1){



if(flag_over==0){
    if(flag_main==3)

    {return_number=voltage_measure(flag_main,0,i_main,max_main,min_main,zero_number);
     pingheng=(int)return_number;
     yuxia=(200-pingheng*2)/2;
     xielv_main=(max_main-min_main)/yuxia*1000000;
     for(i_main_2=0;i_main_2<yuxia;i_main_2++)
     {
      return_number=min_main+xielv_main*i_main;
      return_number=(return_number/3.22)*256;
      return_number2=(int)return_number;
      return_number3[i_main_2]=(return_number2<<4);
      i_main=i_main+0.000001;
    }
     for(i_main_2=yuxia;i_main_2<yuxia+pingheng;i_main_2++)
         {
          return_number=max_main;
          return_number=(return_number/3.22)*256;
          return_number2=(int)return_number;
          return_number3[i_main_2]=(return_number2<<4);
          i_main=i_main+0.000001;
        }
     i_main=0;
     for(i_main_2=yuxia+pingheng;i_main_2<2*yuxia+pingheng;i_main_2++)
         {
          return_number=max_main-i_main*xielv_main;
          return_number=(return_number/3.22)*256;
          return_number2=(int)return_number;
          return_number3[i_main_2]=(return_number2<<4);
          i_main=i_main+0.000001;
        }
     for(i_main_2=2*yuxia+pingheng;i_main_2<200;i_main_2++)
             {
              return_number=min_main;
              return_number=(return_number/3.22)*256;
              return_number2=(int)return_number;
              return_number3[i_main_2]=(return_number2<<4);
              i_main=i_main+0.000001;
            }
    }
 if(flag_main!=3)
 {
for(i_main_2=0;i_main_2<100;i_main_2++)
{       if(flag_main==1)
        {return_number=2+voltage_measure(flag_main,0,i_main,max_main,min_main,zero_number);}
        if(flag_main==2)
        {return_number=voltage_measure(flag_main,0,i_main,max_main,min_main,zero_number);}

         return_number=(return_number/3.22)*256;
         return_number2=(int)return_number;
         return_number3[i_main_2]=(return_number2<<4);
         i_main=i_main+0.000001;

}
for(i_main_2=100;i_main_2<200;i_main_2++)
{
      if(flag_main==1)
      {return_number=2+voltage_measure(flag_main,0,i_main,max_main,min_main,zero_number);}
       if(flag_main==2)
      {return_number=voltage_measure(flag_main,1,i_main-0.0001,max_main,min_main,zero_number);}

         return_number=(return_number/3.22)*256;
         return_number2=(int)return_number;
         return_number3[i_main_2]=(return_number2<<4);
         i_main=i_main+0.000001;

}
}
}
flag_over=1;

for(i_main_2=0;i_main_2<200;i_main_2++)
{
           writedata(return_number3[i_main_2]);
           delay_us(1);
}
               }
 //         writedata(0x0ff0);
 //         writedata(0x0000);
//          writedata(0x0340);
//          unsigned int x=0x0ff0;
//          writedata(x);


 //        if(i_main==0.0002){i_main=0;}

   }
   }

int judge(float max,float min,float ftt_final[])
        {
    if(zero_number!=0){return 3;}//是梯形波
    float a=spot*(max-min)/4;
    if(10<a<14){return 1;}//是正弦波
    if(14<a<16){return 2;}//是三角波
    return 0;

        }
