/*
 * key.c
 *
 *  Created on: 2020年10月12日
 *      Author: 1715700508
 */
#include "driverlib.h"
#include "key.h"
#include "oled_i2c.h"
#include "delay.h"
int flag_final=0;//oled complete flag
int Travel_time=10;          //10~20S
int flag4=0;
void KEY_init()
{

         MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
         MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
         MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
         MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
         MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
         MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
         MAP_Interrupt_enableInterrupt(INT_PORT1);

}
void oled_app()
{

    I2C_OLED_Init();                     //硬件I2C初始化
    OLED_All(0);                         //清屏
    OLED_P8x16Str(1,0,(unsigned char*)"Please Set!");

}

void PORT1_IRQHandler(void)     //按键中断
{
    /*Key1_exit 用于退出长按设置模式
     *Key1_count用于计数是否进入长按设置模式
     *Key1_flag 用于在长按设置模式中设置参数
     *Key1_count、Key2_count用于Key1、2长按计数*/
    uint32_t status;                                                //按键状态位
    int i=0;                                                        //用于消抖for循环
    int Key1_count=0,Key2_count=0;
    static int Key1_flag=0,Key2_flag=0,Key1_exit=0;
    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);      //读按键状态位
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);              //清除标志位

/**************************************************按键1判断******************************************************************/
    if(status & GPIO_PIN1)                                          //按键1
   {
        /******************按键1长按判断********************************************/
        for(i=0;i<480000;i++);                                      //消抖48M下，0.01S消抖
        if(status & GPIO_PIN1)
        {
            while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0)                         //确认按键1没有松开
            {
                Key1_count+=1;                                                               //长按确认
                delay_ms(1);                        //用于确定长按时长
                                                    //按键2：长按(必须在满足时长以及不在按键1长按模式下才能进入)
                if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0 && Key1_count==1500 && (Key2_flag==0))
                {                                                                            //按键1：长按(大概1.5S)
                    Key1_flag=1;
                    Key1_exit+=1;
                    if(Key1_exit==2){                        //再次长按，退出速度设置模式
                           Key1_flag=0;
                           Key1_exit=0;
                           OLED_All(0);                         //清屏
                           delay_ms(10);
                           OLED_P8x16Str(0,1,(unsigned char*)"Successfully Set!");
                           delay_ms(10);
                           OLED_P8x16Str(0,3,(unsigned char*)"Time Now(S):");
                           delay_ms(10);
                           decimal_show(0,5,Travel_time,0);
                           delay_ms(10);
                    }
                    else
                    {OLED_All(0);                             //清屏
                    delay_ms(10);
                    OLED_P8x16Str(0,1,(unsigned char*)"Travel time(s):");
                    delay_ms(10);
                    decimal_show(0,3,Travel_time,0);
                    Travel_time-=1;                              //修正：会在下面的if中加一
                    }

                }
             }
        /******************按键1短按判断*********************************************/
            if((Key1_flag==1) && (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==1))//松开才更新，防止与长按冲突
            {                                          //按键1：短按
                Travel_time+=1;
                if(Travel_time>20)Travel_time=10;
                delay_ms(10);
                decimal_show(0,3,Travel_time,0);
            }
         }
    }
/*****************************************************************************************************************************/


/**************************************************按键2判断******************************************************************/
    if(status & GPIO_PIN4)
    {
        for(i=0;i<480000;i++);                                //消抖48M下，0.01S消抖
        if(status & GPIO_PIN4)
              {
            /******************按键2长按启动定时器********************************************/
                  while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0)                    //确认按键2没有松开
                  {
                      Key2_count+=1;
                      delay_ms(1);                    //用于确定长按时长
                                                      //按键2：长按(必须在满足时长以及不在按键1长按模式下才能进入)
                      if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0 && Key2_count==1500 && (Key1_flag==0))
                      {
                           OLED_All(0);                         //清屏
                           delay_ms(10);
                          OLED_P8x16Str(0,3,(unsigned char*)"OKOKOK!!!");
                          flag_final=1;
                          MAP_Interrupt_enableInterrupt(INT_T32_INT1);          //开启定时器中断，启动小车


                      }
                  }
            /*******************按键2短按判断*************************************************/
                  if(Key1_flag==1 && (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==1))//松开才更新，防止与长按冲突
                  {                                     //按键2：短按
                      Travel_time-=1;
                      if(Travel_time<10)Travel_time=20;
                      decimal_show(0,3,Travel_time,0);
                      delay_ms(10);
                  }
              }
    }
}
