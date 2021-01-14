/* DriverLib Includes */
#include <lib/control_task.h>
#include <lib/System.h>
#include "driverlib.h"
#include "msp.h"
#include "adc14_multi_channal.h"
#include "key.h"
#include "oled_i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "delay.h"



int Thread(float vol);

float res[5];
/*编码器定时器捕获
*    P7.7    P7.6
*   TA1.1   TA1.2
*/
/* 控制前进方向
 * P7.7 P7.6 接左右

 */
/*编码器接错引脚*/
/*蓝牙要延时 会阻塞*/
void main(void)
{
//    delay_init();
//
//      KEY_init();
//      I2C_OLED_Init();                     //硬件I2C初始化
//      OLED_All(0);                         //清屏
//      OLED_P8x16Str(1,0,(unsigned char*)"Please Set!");
//      MAP_Interrupt_enableMaster();

    //    WDT_A_holdTimer();
    System_init();



    while(flag_final==0);
    while (1)
    {
        getvol (res);
        threshold = 2*Thread(res[4])+Thread(res[2])-Thread(res[1])-2*Thread(res[3]);
        if(Thread(res[4])+Thread(res[2])+Thread(res[1])+Thread(res[3]) >= 2)
         {
            Remoter(0);
            TA3CCR3 =0;
            TA3CCR4 =0;
         }

        if(cap_flag)
        {
        set_PID_diff(threshold);
        }
        if(threshold!=0)
        {
            cap_flag=0;
        }
        Printf();
    }
}


int Thread(float vol){
    if(vol - 2.0 >= 0.001)
        return 1;
    else //if(vol - 0.8 <= 0.001)
        return 0;

}
