#include "msp.h"
#include "driverlib.h"
#include "delay.h"
#include "mpu_DMP.h"
#include "outputdata.h"





/**
 * main.c
 */
void main(void)
{
delay_init();
usart_init();
int i=0;
mpu6050_i2c_init();
for(i=0;i<5;i++)
{delay_ms(10);}
Init_MPU6050();

 while(1){

           int i4=0;
           for(i4=0;i4<50;i4++)
           {delay_ms(5);}  //这里的延时5ms注释掉是因为循环中的时间已经超过5ms,如果提高spi速度，串口速度和主频侧需要增加延时

           readdmp();
           getquaternion();//四元素
           getgyro();//陀螺仪
           getaccel();//加速度计
           getyawpitchroll();//根据四元素算出欧拉角
           OutData[0] = yprf[0];//欧拉角
           OutData[1] =gyrof[0]; //陀螺仪
           OutData[2] =  accelf[1]/200;//加速度计
           OutData[3] =  2000;  //用来检测输出是否正常

           OutPut_Data();
    }





}


