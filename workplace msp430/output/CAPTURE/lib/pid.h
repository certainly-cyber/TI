/*
 * pid.h
 *
 *  Created on: 2020年10月3日
 *      Author: asus
 */

#ifndef PID_H_
#define PID_H_

#define PID_Mode_Pos   0      // 位置式PID
#define PID_Mode_Inc   1      // 增量式PID

#define INC_PID_DEFAULT_Kp 600//130
#define INC_PID_DEFAULT_Ki 0//3
#define INC_PID_DEFAULT_Kd 0
#define INTGRAL_MAX         4000.0
#define PID_INTGRAL_MAX     +INTGRAL_MAX
#define PID_INTGRAL_MIN     -INTGRAL_MAX

typedef struct PID_Structure{
    float Kp;
    float Ki;
    float Kd;
    float err[3];   // err[0]:本次误差   err[1]:上次误差  err[2]:上上次误差,用于增量式PID
    float intgral;  // 累加值,用于位置式PID
    float intgral_max;
    float intgral_min;
    float diff;     // 差分值,用于位置式PID
    float ref;      // 目标值
    float fdb;      // 反馈值
    float output;
    float  (*Calc)(struct PID_Structure *pid,int PID_Mode);
    void  (*Reset)(struct PID_Structure *pid,int PID_Mode);
    void  (*Init) (struct PID_Structure *pid,float kp,float ki,float kd,int PID_Mode);
}PID_Structure;

#define INCREASE_PID_DEFAULT {  \
    INC_PID_DEFAULT_Kp,\
    INC_PID_DEFAULT_Ki,\
    INC_PID_DEFAULT_Kd,\
    {0,0,0},\
    0,\
    PID_INTGRAL_MAX,\
    PID_INTGRAL_MIN,\
    0,\
    0,\
    0,\
    0,\
    &PID_Calc,\
    &PID_Reset,\
    &PID_Init,\
}

void PID_Reset(PID_Structure *pid,int PID_Mode);
float PID_Calc(PID_Structure *pid,int PID_Mode);
void PID_Init(PID_Structure *pid,float kp,float ki,float kd,int PID_Mode);


#endif /* PID_H_ */
