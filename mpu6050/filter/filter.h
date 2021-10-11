/*
 * filter.h
 *
 *  Created on: 2021Äê5ÔÂ4ÈÕ
 *      Author: certainly
 */

#ifndef FILTER_FILTER_H_
#define FILTER_FILTER_H_
#include "delay.h"
#include "driverlib.h"
#include <math.h>

typedef struct
{
        float Input_Butter[3];
        float Output_Butter[3];
}Butter_BufferData;

typedef struct
{
        float a[3];
        float b[3];
}Butter_Parameter;

u32 Bubble_Filter(u32 *value);

float KalmanFilter1(float *Original_Data);
float KalmanFilter2(float *Original_Data);

float KalmanFilter3(float *Original_Data);
float KalmanFilter4(float *Original_Data);



float Bubble_Filter_Float(float *value);









#endif /* FILTER_FILTER_H_ */
