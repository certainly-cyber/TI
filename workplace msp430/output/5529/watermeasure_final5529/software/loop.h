/*
 * loop.h
 *
 *  Created on: 2020年8月13日
 *      Author: certainly
 */

#ifndef SOFTWARE_LOOP_H_
#define SOFTWARE_LOOP_H_
#include "delay.h"
extern u16 loop_5ms,loop_10ms,loop_20ms,loop_50ms,loop_100ms,loop_500ms,loop_1000ms;
extern u32 sys_time;
extern uint32_t CAP0_BUFF[32];
extern uint32_t CAP1_BUFF[32];
extern float High_Now,High_Aim;//水位高度
extern float Kp,Ki,Kd;//PID参数
extern float Error,Last_Error;//误差
extern float Integral;//积分
extern float Diff;//微分
extern int start;
extern int mode;
extern int8_t cap_step;
void Loop_1000ms_Task(void);
void Loop_50ms_Task(void);
void Loop_10ms_Task(void);

//void Caculate0_Cap_To_High(void);
//void Caculate1_Cap_To_High(void);




#endif /* SOFTWARE_LOOP_H_ */
