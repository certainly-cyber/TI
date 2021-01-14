#include <lib/control_task.h>
#include <lib/encoder.h>
#include <lib/pid.h>
#include <lib/printf.h>
#include "driverlib.h"
#include "msp.h"
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"

#define Wheel_left  0
#define Wheel_right 1

char ch[8]={0};
float outl = 0,outr = 0,outdiff=0;
int speed = 15;
int threshold = 0;

PID_Structure PID_R = INCREASE_PID_DEFAULT;
PID_Structure PID_L = INCREASE_PID_DEFAULT;
PID_Structure PID_Diff = INCREASE_PID_DEFAULT;

float ADC_Diff;
int cap_flag = 1;
/**
 * @name: set_PID_param
 * @msg:  设置PID参数
 * @param {void}
 * @return {void}
 */
void set_PID_param()
{
    PID_L.Kp = 17;
    PID_L.Ki = 5;
    PID_L.Kd = 0;

    PID_R.Kp = 17;
    PID_R.Ki = 5;
    PID_R.Kd = 0;

    PID_Diff.Kp = 3;
    PID_Diff.Ki = 0;
    PID_Diff.Kd = 0;
}

void set_PID_diff(int diff)
{
    PID_Diff.fdb = diff;
}

void Read_Encoder(void){
   /*读左右轮的脉冲数*/
    static int i =0;

    PID_L.fdb =   read_number(Wheel_left);
    PID_R.fdb =   read_number(Wheel_right);

}

/**
 * @name: Control_Task
 * @msg:  控制的主循环
 * @param {void}
 * @return {void}
 */
void Control_Task(void)
{
    Read_Encoder();
    calculate();
    Motor();
}

/****************************低通滤波****************************/
/**
 * @name:     lowpass_1order
 * @msg:      一阶低通滤波
 * @param vi：当前检测值；vk_1上一次运算得出值；sample_freq采样率；cutfreq截止频率
 * @return：   这一次运算结果
 */
float lowpass_1order(float vi,float vk_1,float sample_freq,float cutfreq)
{
    float rc = 1.0 / 2.0 / 3.1415926535 / cutfreq;
    float cof1 = 1/(1+rc*sample_freq);
    float cof2 = rc*sample_freq/(1+rc*sample_freq);
    float vk = vi * cof1 + vk_1 * cof2;
    return vk;
}
/****************************************************************/



void calculate(void)
{
    static float outl_static,outr_static =0;

    PID_L.Calc(&PID_L,PID_Mode_Inc);
    PID_R.Calc(&PID_R,PID_Mode_Inc);
    outl += PID_L.output;
    outr += PID_R.output;
    outl_static = outl;
    outr_static = outr;

    PID_Diff.Calc(&PID_Diff,PID_Mode_Pos);
    outdiff=PID_Diff.output;
    if(outdiff > 0)
    {
        PID_R.ref = speed + outdiff + 5;
        PID_L.ref = 0;
    }
    else if(outdiff<0)
    {
            PID_R.ref = 0;
            PID_L.ref = speed - outdiff + 5;
    }else
    {
    PID_R.ref = speed + outdiff;
    PID_L.ref = speed - outdiff;
    }
    outl = limit_pwm(outl);
    outr = limit_pwm(outr);
    cap_flag =1;
}




/**
 * @name: Printf
 * @msg:  串口打印数据到上位机
 * @param {void}
 * @return {void}
 */
void Printf(void)
{
    int j = 5000;
    sprintf(ch,"s:%f,%f,%f,%d,%d\n\r",PID_L.ref,PID_L.fdb,PID_L.output,TA3CCR3,TA3CCR4);
 //   sprintf(ch,"s:%f,%f,%f,%i,%f,%f,%f,%f,%i,%f\n\r",PID_L.ref,PID_L.fdb,PID_L.output,TA3CCR3,PID_L.intgral,PID_R.ref,PID_R.fdb,PID_R.output,TA3CCR4,PID_R.intgral);
    print(EUSCI_A0_BASE,"%s\r\n",ch);
    while(j)
        j--;
}

/**
 * @name: Remoter
 * @msg: 控制小车前进,更改各个轮子的目标值
 * @param {int speed}
 * @return {void}
 */

void Remoter(int speed)
{
    PID_L.ref = (int)speed;
    PID_R.ref = speed;
}


/**
 * @name: Motor
 * @msg: 驱动电机，pwm输入正数正转，输入负数反转
 * @param {float pwm}
 * @return {void}
 */

void Motor(void)
{
    Motor_L(outl);
    Motor_R(outr);
}

void Motor_L(float pwm)
{
    if (pwm>0)
    {
        TA3CCR3 = pwm;
    }else
    {
        TA3CCR3 = -pwm;
    }
}
void Motor_R(float pwm)
{
    if (pwm>0)
    {
        TA3CCR4 = pwm;
    }else
    {
        TA3CCR4 = -pwm;
    }
}

/**
 * @name: limit_pwm
 * @msg: pwm限幅，这里的幅值是+-1000
 * @param {int pwm}
 * @return {int pwm}
 */

int limit_pwm(int pwm)
{
  if (pwm>=0)
  {
    if (pwm>=1800)
    {
      pwm = 1800;
    }
  }
  else if (pwm<=0)
  {
    pwm = 0;
  }

  return pwm;
}


