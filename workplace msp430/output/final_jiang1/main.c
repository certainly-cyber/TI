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
#include "oled_i2c.h"
#include "zimo.h"
#include "key.h"
int all_number=0;
int   a = 0;
float b = 0;
int   c = 0;
int n=0;
int flag_key=0;
int flag_main;
int flag_over=0;
float i_main=0;
int i_main_1=0;
int i_main_2=0;
float return_number;
int return_number2;
int return_number3[200]={0};
int return_number4[200]={0};
int return_number5[200]={0};
float data[32]={0};
float fft_final[32]={0};
float max_main;
float min_main;
float temporary;
int zero_number;
int yuxia=0;
int pingheng=0;
float xielv_main=0;
int judge(float data[],float max,float min);

void main (void)
{
   volatile unsigned int tmp = 0;

   WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
   Init_20MHz_MCLK();
   GPIO_init();
   I2C_OLED_Init();
   adc_Init();
   OLED_All(0);//清屏
   OLED_P8x16Str(0,0,(unsigned char*)"start:");
   OLED_P8x16Str(0,2,(unsigned char*)"0");
     Init_AD();//开始采样
   Capture_Init();
   _EINT();
   while(1){


   if(all_number<3){  ADC12IE|= BIT0;
     while(fft_flag==0){}//等待采样完成
   fft_flag=0;
   _DINT();
   for(i_main_1=0;i_main_1<31;i_main_1++)
  {
      temporary =h[i_main_1].real/4095*3.3;//实际采样
     fft_final[i_main_1]=temporary;
   }
   for(n=0;n<FFT_N;n++)
          {
              h[n].real = (float)h[n].real*0.00080078125;
          }

          fft(FFT_N,h);

          for(a=0;a<FFT_N;a++)
          {
              b = h[a].real*h[a].real+h[a].imag*h[a].imag;
              data[a] = sqrtf(b);
          }

   max_main=findmax(fft_final);
   min_main=findmin(fft_final);
   zero_number=zeronumber(fft_final,max_main);
   flag_main=judge(data,max_main,min_main);
   if(flag_main==1){   OLED_P8x16Str(0,4,(unsigned char*)"cos");}
   if(flag_main==2){   OLED_P8x16Str(0,4,(unsigned char*)"tangle");}
   if(flag_main==3){   OLED_P8x16Str(0,4,(unsigned char*)"else");}
 //  flag_main=3;

 //           writedata(0x0000);


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
      if(all_number==0){
      return_number3[i_main_2]=(return_number2<<4);}
      if(all_number==1){
          return_number4[i_main_2]=(return_number2<<4);}
      if(all_number==2){
                return_number5[i_main_2]=(return_number2<<4);}
      }
      i_main=i_main+0.000001;
    }
     for(i_main_2=yuxia;i_main_2<yuxia+pingheng;i_main_2++)
         {
          return_number=max_main;
          return_number=(return_number/3.22)*256;
          return_number2=(int)return_number;
          if(all_number==0){
               return_number3[i_main_2]=(return_number2<<4);}
               if(all_number==1){
                   return_number4[i_main_2]=(return_number2<<4);}
               if(all_number==2){
                         return_number5[i_main_2]=(return_number2<<4);}
          i_main=i_main+0.000001;
        }
     i_main=0;
     for(i_main_2=yuxia+pingheng;i_main_2<2*yuxia+pingheng;i_main_2++)
         {
          return_number=max_main-i_main*xielv_main;
          return_number=(return_number/3.22)*256;
          return_number2=(int)return_number;
          if(all_number==0){
          return_number3[i_main_2]=(return_number2<<4);}
          if(all_number==1){
          return_number4[i_main_2]=(return_number2<<4);}
          if(all_number==2){
          return_number5[i_main_2]=(return_number2<<4);}

          i_main=i_main+0.000001;
        }
     for(i_main_2=2*yuxia+pingheng;i_main_2<200;i_main_2++)
             {
              return_number=min_main;
              return_number=(return_number/3.22)*256;
              return_number2=(int)return_number;
              if(all_number==0){
              return_number3[i_main_2]=(return_number2<<4); OLED_P8x16Str(0,2,(unsigned char*)"1");}
              if(all_number==1){
              return_number4[i_main_2]=(return_number2<<4); OLED_P8x16Str(0,2,(unsigned char*)"2");}
              if(all_number==2){
              return_number5[i_main_2]=(return_number2<<4); OLED_P8x16Str(0,2,(unsigned char*)"3");}
              i_main=i_main+0.000001;
            }
             i_main=0;


 if(flag_main!=3)
 {
for(i_main_2=0;i_main_2<100;i_main_2++)
{       if(flag_main==1)
        {return_number=2+voltage_measure(flag_main,0,i_main,max_main,min_main,zero_number);}
        if(flag_main==2)
        {return_number=voltage_measure(flag_main,0,i_main,max_main,min_main,zero_number);}

         return_number=(return_number/3.22)*256;
         return_number2=(int)return_number;
         if(all_number==0){
         return_number3[i_main_2]=(return_number2<<4); OLED_P8x16Str(0,2,(unsigned char*)"1");}
         if(all_number==1){
         return_number4[i_main_2]=(return_number2<<4); OLED_P8x16Str(0,2,(unsigned char*)"2");}
         if(all_number==2){
          return_number5[i_main_2]=(return_number2<<4); OLED_P8x16Str(0,2,(unsigned char*)"3");}
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
         if(all_number==0){
         return_number3[i_main_2]=(return_number2<<4); OLED_P8x16Str(0,2,(unsigned char*)"1");}
         if(all_number==1){
         return_number4[i_main_2]=(return_number2<<4); OLED_P8x16Str(0,2,(unsigned char*)"2");}
         if(all_number==2){
          return_number5[i_main_2]=(return_number2<<4); OLED_P8x16Str(0,2,(unsigned char*)"3");}
         i_main=i_main+0.000001;


}
i_main=0; _EINT();
}
all_number=all_number+1;}
while(all_number>2)//开始进入按键模式
{
    KEY_init();
    flag_key=keyscan();
    if(flag_key==1)
    {       for(i_main_2=0;i_main_2<200;i_main_2++)
    {
               writedata(return_number3[i_main_2]);
       //        delay_us(1);
    }
    }
    if(flag_key==2)
       {       for(i_main_2=0;i_main_2<200;i_main_2++)
       {
                  writedata(return_number4[i_main_2]);
          //        delay_us(1);
       }
       }
    if(flag_key==3)
       {       for(i_main_2=0;i_main_2<200;i_main_2++)
       {
                  writedata(return_number5[i_main_2]);
          //        delay_us(1);
       }
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

int judge(float data[],float max,float min)
        {
    if(zero_number!=0){return 3;}//是梯形波
    float a=(max-min)/2;
    if(14.0*a<data[1]&&data[1]<18.0*a){return 1;}//是正弦波
    if(10.0*a<data[1]&&data[1]<14.0*a){return 2;}//是三角波
    return 0;

        }
