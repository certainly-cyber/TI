#include "driverlib.h"
#include "key.h"
#include "pwm.h"
#include "watchdog.h"
#include "oled_i2c.h"
#include "zimo.h"
#include "loop.h"
#include "fdc2214_interface.h"
unsigned long *Ptr1=(unsigned long*)0x1900;
unsigned long *Ptr2=(unsigned long*)0x1980;
int i=0;
void main()
{
    TIME_init();
    WATCHDOG_init();
    KEY_init();
    PWM_init();
    fdc2214_init();
    I2C_OLED_Init();
    OLED_All(0);//清屏
    //这里是读取存储的程序
//    for(i=0;i<32;i++){
//        CAP0_BUFF[i]=Ptr1[i];
//     }
//    i=0;
//    for(i=0;i<32;i++){
//        CAP1_BUFF[i]=Ptr2[i];
//     }

    __enable_interrupt();
    while(1)
    {
        Loop_1000ms_Task();
        Loop_50ms_Task();
        Loop_10ms_Task();
        decimal_show(75,4,High_Aim,1,1);
        decimal_show(75,6,High_Now,1,1);
    }
}


//Watchdog Timer interrupt service routine
//2ms会进入
#pragma vector=WDT_VECTOR
__interrupt void WDT_A_ISR (void)
{
        sys_time++;
        loop_5ms++;
        loop_10ms++;
        loop_20ms++;
        loop_50ms++;
        loop_100ms++;
        loop_500ms++;
        loop_1000ms++;
}
