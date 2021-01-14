/*
 * loop.c
 *
 *  Created on: 2020年8月13日
 *      Author: certainly
 */
#include "loop.h"
#include "driverlib.h"
#include "pwm.h"
#include "key.h"
#include "delay.h"
#include "stdio.h"
#include "fdc2214_interface.h"
#define INFOB_START   (0x1900)
#define INFOA_START   (0x1980)
//float a=11.95;
int keyscan1=0;
u16 loop_5ms,loop_10ms,loop_20ms,loop_50ms,loop_100ms,loop_500ms,loop_1000ms;
u32 sys_time=0;
char mode1[2];
char start1[2];
char number1[2];
//每增加1cm，CAP对应值
int8_t cap_step=31;
uint32_t CAP0_BUFF[32];
uint32_t CAP1_BUFF[32];
uint32_t ch0_original,ch1_original;
float ch0_cap,ch1_cap;
int start=0;
int mode=0;
int number=0;
float High_Now=5.5,High0_Now=0,High1_Now=0,High_Aim=5.5;//水位高度
float Kp=0.0,Ki=0.0,Kd=0.0;//PID参数
float Error,Last_Error;//误差
float Integral;//积分
float Diff;//微分
float PID;
uint16_t status;

float Cap0_Filter_Buff[5];
float Cap1_Filter_Buff[5];
float Cap2_Filter_Buff[5];
float cap0_filter;
float cap1_filter;
float cap2_filter;

void Loop_1000ms_Task(void)
{
    if(loop_1000ms>=1000)
    {
        loop_1000ms=0;
        //任务处理
        GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);//提示系统正在运行



    }
}

void Loop_50ms_Task(void)
{
    if(loop_50ms>=50)
    {
        loop_50ms=0;
        //任务处理
        if(start==1)//开始题目控制
        {
            if(mode==0)
            {
                setcompare(750);
            }
            if(mode==1 || mode==6)
            {
                Kp=2500.0f;
                Ki=5.0f;
                Kd=0.0f;
                //PID参数

                //PID计算
                Error=High_Aim-High_Now;
                Integral+=Error;
                if(Integral>10)Integral=10;
                if(Integral<-10)Integral=-10;
                Diff=Error-Last_Error;
                Last_Error=Error;

                PID=Kp*Error + Ki*Integral + Kd*Diff;
                if(PID<50)PID=50;
                if(PID>990)PID=990;

                setcompare(500);
            }

            if(mode==2)//不开瓶口，10s内，50mm到90mm
            {
                High_Aim=9.0f;
                Kp=2500.0f;
                Ki=5.0f;
                Kd=0.0f;
                //PID参数

                //PID计算
                Error=High_Aim-High_Now;
                Integral+=Error;
                if(Integral>10)Integral=10;
                if(Integral<-10)Integral=-10;
                Diff=Error-Last_Error;
                Last_Error=Error;

                PID=Kp*Error + Ki*Integral + Kd*Diff;
                if(PID<1)PID=1;
                if(PID>990)PID=990;

                setcompare(100);
            }

            if(mode==3)//开瓶口，10s内，控制到90mm，稳住10s以上
            {
                High_Aim=9.0f;
                Kp=2500.0f;
                Ki=5.0f;
                Kd=0.0f;
                //PID参数

                //PID计算
                Error=High_Aim-High_Now;
                Integral+=Error;
                if(Integral>10)Integral=10;
                if(Integral<-10)Integral=-10;
                Diff=Error-Last_Error;
                Last_Error=Error;

                PID=Kp*Error + Ki*Integral + Kd*Diff;
                if(PID<100)PID=100;
                if(PID>990)PID=990;

                setcompare(200);
            }

            if(mode==4)//30s内，90mm到110mm，稳住10s以上
            {
                High_Aim=11.0f;
                Kp=2500.0f;
                Ki=5.0f;
                Kd=0.0f;
                //PID参数

                //PID计算
                Error=High_Aim-High_Now;
                Integral+=Error;
                if(Integral>10)Integral=10;
                if(Integral<-10)Integral=-10;
                Diff=Error-Last_Error;
                Last_Error=Error;

                PID=Kp*Error + Ki*Integral + Kd*Diff;
                if(PID<100)PID=100;
                if(PID>990)PID=990;

                setcompare(300);
            }

            if(mode==5)//完成4后，打开扰动水箱，稳住在110mm
            {
                High_Aim=9.0f;
                Kp=2500.0f;
                Ki=5.0f;
                Kd=0.0f;
                //PID参数

                //PID计算
                Error=High_Aim-High_Now;
                Integral+=Error;
                if(Integral>10)Integral=10;
                if(Integral<-10)Integral=-10;
                Diff=Error-Last_Error;
                Last_Error=Error;

                PID=Kp*Error + Ki*Integral + Kd*Diff;
                if(PID<50)PID=50;
                if(PID>990)PID=990;

                setcompare(400);
            }
        }
        else
        {
            setcompare(0);
        }



    }
}


void Loop_10ms_Task(void)
{
    static u16 water_t=0;
    float K=0;
    if(loop_10ms>=10)
    {
        loop_10ms=0;
        //任务处理
        if(fdc2214_get_channel_data(CHANNEL_0))
        {
            ch0_original=fdc2214_get_channel_data(CHANNEL_0);
            ch0_cap=(uint32_t)fdc2214_calc_capatiance(ch0_original);
        }

        if(fdc2214_get_channel_data(CHANNEL_1))
        {
            ch1_original=fdc2214_get_channel_data(CHANNEL_1);
            ch1_cap=(uint32_t)fdc2214_calc_capatiance(ch1_original);
        }


        if(start==0)//开始之前，选择题目
        { keyscan1=keyscan();
            if(keyscan1==2)
            {

                mode++;
                keyscan1=0;
            }
            if(keyscan1==3)
            {

                start=1;//开始题目
                keyscan1=0;
            }
        }
        else
        {
            switch(mode)
            {
                case 0://debug,标定模式
                    keyscan1=keyscan();
                    if(keyscan1==2)
                    {
                        keyscan1=0;
                        number=number+1;
                        CAP0_BUFF[cap_step]=ch0_cap;
                        CAP1_BUFF[cap_step]=ch1_cap;
                        cap_step--;
                        if(cap_step<0)
                        {
                            mode=1;
                                //Write calibration data to INFOB

                                do
                                {
                                    //Erase INFOB
                                    FlashCtl_eraseSegment((uint8_t *)INFOB_START);
                                    status = FlashCtl_performEraseCheck((uint8_t *)INFOB_START,
                                        128
                                        );
                                } while (status == STATUS_FAIL);

                                //Write calibration data to INFOA

                                FlashCtl_write32(CAP0_BUFF,
                                    (uint32_t *)(INFOB_START),
                                    32
                                    );
                                //Erase INFOA
                                do
                                {
                                FlashCtl_eraseSegment((uint8_t *)INFOA_START);
                                status = FlashCtl_performEraseCheck((uint8_t *)INFOA_START,
                                    128
                                    );
                            } while (status == STATUS_FAIL);

                            //Write calibration data to INFOB

                            FlashCtl_write32(CAP1_BUFF,
                                (uint32_t *)(INFOA_START),
                                32
                                );

                            //flash
//                            Flash_Erase_Segment(MAX_SEGMENT);
//                            Flash_Write_buf (MAX_SEGMENT,0,124,(uint8_t*)(CAP0_BUFF));
//                            Flash_Write_buf (MAX_SEGMENT,128,124,(uint8_t*)(CAP1_BUFF));
                        }
                    }

                    break;

                case 1://可控注水，显示水位
                    keyscan1=keyscan();
                    if(keyscan1==1)
                    {   keyscan1=0;
                        High_Aim-=0.1;
                    }
                    if(keyscan1==2)
                    {
                        keyscan1=0;
                        High_Aim+=0.1;
                    }
                    if(keyscan1==3)
                    {
                        keyscan1=0;
                        mode=2;
                    }
                    break;

                case 2://不开瓶口，10s内，50mm到90mm
                    keyscan1=keyscan();
                    if(keyscan1==3)
                    {
                        keyscan1=0;
                        mode=3;
                    }
                    break;

                case 3://开瓶口，10s内，控制到90mm，稳住10s以上
                    keyscan1=keyscan();
                    if(keyscan1==3)
                    {
                        keyscan1=0;
                        mode=4;
                    }
                    break;

                case 4://30s内，90mm到110mm，稳住10s以上
                    keyscan1=keyscan();
                    if(keyscan1==3)
                    {
                        keyscan1=0;
                        mode=5;
                    }
                    break;

                case 5://完成4后，打开扰动水箱，稳住在110mm
                    keyscan1=keyscan();
                    if(keyscan1==3)
                    {
                        keyscan1=0;
                        mode=6;
                    }
                    break;

                case 6://完成4后，打开扰动水箱，稳住在110mm
                    keyscan1=keyscan();
                    if(keyscan1==3)
                    {
                        keyscan1=0;
                        mode=1;
                    }

                    if(keyscan1==1)
                    {
                        keyscan1=0;
                        water_t++;
                    }
                    if(keyscan1==2)
                    {
                        keyscan1=0;
                        if(29<water_t && water_t<=38)High_Aim=(((float)water_t-29.0f)/9.0f) + 0.0f;
                        if(38<water_t && water_t<=46)High_Aim=(((float)water_t-38.0f)/8.0f) + 1.0f;
                        if(46<water_t && water_t<=55)High_Aim=(((float)water_t-46.0f)/9.0f) + 2.0f;
                        if(55<water_t && water_t<=63)High_Aim=(((float)water_t-55.0f)/8.0f) + 3.0f;

                        if(63<water_t && water_t<=71)High_Aim=(((float)water_t-63.0f)/8.0f) + 4.0f;
                        if(71<water_t && water_t<=79)High_Aim=(((float)water_t-71.0f)/8.0f) + 5.0f;
                        if(79<water_t && water_t<=87)High_Aim=(((float)water_t-79.0f)/8.0f) + 6.0f;
                        if(87<water_t && water_t<=94)High_Aim=(((float)water_t-87.0f)/7.0f) + 7.0f;
                        if(94<water_t && water_t<=102)High_Aim=(((float)water_t-94.0f)/8.0f) + 8.0f;
                        if(102<water_t && water_t<=109)High_Aim=(((float)water_t-102.0f)/7.0f) + 9.0f;
                        if(109<water_t && water_t<=116)High_Aim=(((float)water_t-109.0f)/7.0f) + 10.0f;
                        if(116<water_t && water_t<=123)High_Aim=(((float)water_t-116.0f)/7.0f) + 11.0f;
                        if(123<water_t && water_t<=130)High_Aim=(((float)water_t-123.0f)/7.0f) + 12.0f;
                        if(130<water_t && water_t<=136)High_Aim=(((float)water_t-130.0f)/6.0f) + 13.0f;
                    }
                    break;

                default:


                    break;

            }

        }



        K=High_Now/15.0f+0.15;
        if(K<0.2)K=0.2;
        if(K>0.8)K=0.8;

        High_Now=High0_Now*(1-K) + High1_Now*K;
//        OLED_printf(2,0,"M:%d S:%d P:%.1f",mode,start,(float)cap_step/2.0f);//当前模式
//        OLED_printf(2,2,"wt:%d st:%.d",water_t,sys_time/1000);//显示电容值
//        OLED_printf(2,4,"H:%.2f,%.2f",High0_Now,High1_Now);//显示水位
//        OLED_printf(2,7,"H:%.2f,%.2f",High_Aim,High_Now);//显示水位

        OLED_P8x16Str(0,0,(unsigned char*)"mode:");
        sprintf(mode1,"%d",mode);
        OLED_P8x16Str(60,0,mode1);
        OLED_P8x16Str(70,0,(unsigned char*)"num:");
        sprintf(number1,"%d",number);
        OLED_P8x16Str(105,0,number1);
        OLED_P8x16Str(0,2,(unsigned char*)"start:");
        sprintf(start1,"%d",start);
        OLED_P8x16Str(60,2,start1);
        OLED_P8x16Str(0,4,(unsigned char*)"HIGHAIM:");
        OLED_P8x16Str(0,6,(unsigned char*)"HIGHNOW:");






    }
}

void Caculate0_Cap_To_High(void)
{
    u8 i=0;

    if((ch0_cap)<=CAP0_BUFF[0])
    {
        High0_Now=0;
    }
    else if(CAP0_BUFF[0]<=(ch0_cap) && (ch0_cap)<=CAP0_BUFF[30])
    {
        for(i=0;i<30;i++)
        {
            if(CAP0_BUFF[i]<(ch0_cap) && (ch0_cap)<=CAP0_BUFF[i+1])
            {
                High0_Now=0.5f * ((ch0_cap)-CAP0_BUFF[i])/(CAP0_BUFF[i+1]-CAP0_BUFF[i]) +i*0.5f;
            }
        }
    }
    else
    {
        High0_Now=15.0f;
    }

}

void Caculate1_Cap_To_High(void)
{
    u8 i=0;

    if((ch1_cap)<=CAP1_BUFF[0])
    {
        High1_Now=0;
    }
    else if(CAP1_BUFF[0]<=(ch1_cap) && (ch1_cap)<=CAP1_BUFF[30])
    {
        for(i=0;i<30;i++)
        {
            if(CAP1_BUFF[i]<(ch1_cap) && (ch1_cap)<=CAP1_BUFF[i+1])
            {
                High1_Now=0.5f * ((ch1_cap)-CAP1_BUFF[i])/(CAP1_BUFF[i+1]-CAP1_BUFF[i]) +i*0.5f;
            }
        }
    }
    else
    {
        High1_Now=15.0f;
    }

}



