/*
 * control_task.h
 *
 *  Created on: 2020年10月3日
 *      Author: asus
 */

#ifndef CONTROL_TASK_H_
#define CONTROL_TASK_H_

#include <stdint.h>
extern float res[5];

extern float outl,outr,outdiff;
extern float left, right;
extern float ADC_Diff; // 获取的ADC差值（用于循迹）
extern int speed;
extern int cap_flag;
extern int threshold;

void set_PID_param(void);
void set_PID_diff(int diff);

void Control_Task(void);

void Printf(void);
int limit_pwm(int pwm);

/**************/
void SetTarget(int speedl,int speedr);
void calculate(void);


void Motor(void);
void Motor_L(float pwm);
void Motor_R(float pwm);

void Remoter(int speed);
/************************/

void AX_MOVE_Kinematics(int16_t vx,int16_t vy,int16_t vz);
float lowpass_1order(float vi,float vk_1,float sample_freq,float cutfreq);

void Read_Encoder(void);
#endif /* CONTROL_TASK_H_ */
