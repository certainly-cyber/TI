/*
 * pid.c
 *
 *  Created on: 2020年10月3日
 *      Author: asus
 */

#include <lib/pid.h>
#include <stdint.h>

void PID_Reset(PID_Structure *pid,int PID_Mode){
    pid->Kp = 0;
    pid->Ki = 0;
    pid->Kd = 0;
    pid->err[0] = 0;
    pid->err[1] = 0;
    pid->err[2] = 0;
    pid->intgral = 0;
    pid->fdb = 0;
    pid->ref = 0;
    pid->output = 0;
}

void PID_Init(PID_Structure *pid,float kp,float ki,float kd,int PID_Mode){
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->fdb = 0;
    pid->ref = 0;
    pid->output = 0;
    pid->err[0] = 0;
    pid->err[1] = 0;
    if(PID_Mode == PID_Mode_Inc){
        pid->err[2] = 0;
    }
    else{
        pid->intgral_max = INTGRAL_MAX;
        pid->intgral = 0;
        pid->diff = 0;
    }
}

float PID_Calc(PID_Structure *pid,int PID_Mode){
    if(PID_Mode == PID_Mode_Inc){
        pid->err[2] = pid->err[1];
        pid->err[1] = pid->err[0];
        pid->err[0] = pid->ref - pid->fdb;

        pid->output =  pid->Kp * (pid->err[0] - pid->err[1])
                          + pid->Ki * pid->err[0]
                          + pid->Kd *( pid->err[0] - 2 * pid->err[1] + pid->err[2]);
        return (pid->output);
    }
    else{ // 位置式
        pid->err[2] = pid->err[1];
        pid->err[1] = pid->err[0];
        pid->err[0] = pid->ref - pid->fdb;
        pid->intgral += pid->err[0];
        if(pid->intgral >=  pid->intgral_max) pid->intgral =  pid->intgral_max;
        if(pid->intgral <= -pid->intgral_max) pid->intgral = -pid->intgral_max;
        pid->diff = pid->err[0] - pid->err[1];

        pid->output = pid->Kp*pid->err[0] + pid->Ki*pid->intgral + pid->Kd*pid->diff;
        return (pid->output);

    }
}


