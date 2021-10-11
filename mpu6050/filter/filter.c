/*
 * filter.c
 *
 *  Created on: 2021��5��4��
 *      Author: certainly
 */

#include "delay.h"
#include "driverlib.h"
#include <math.h>
#include "filter.h"

/* ð�� ��ֵ�˲�  */
u32 Bubble_Filter(u32 *value)
{
        u8 i,j,swapFlag; //������־λ
        u32 res = 0;   //reserve �ݴ�
        u32 med = 0;   //��ֵ

        for(j = 0;j < 10-1;j++){
              swapFlag = 0; //ÿһ����ѭ������
                for(i = 0;i < 9-j;i++){
                        if( value[i] > value[i+1] ){ //>����   <����
                                res = value[i];
                                value[i] = value[i+1];
                                value[i+1] = res;

                                swapFlag = 1; //��������1
                        }
                }
                if(0 == swapFlag) break;//δ��������������ǰ����
        }
        med = (*(value+4)+ *(value+5)+ *(value+6))/3;   //�м�ƽ��ֵ
        return med;
}



float Bubble_Filter_Float(float *value)
{
        u8 i,j,swapFlag; //������־λ
        float res = 0.0f;   //reserve �ݴ�
        float med = 0.0f;   //��ֵ

        for(j = 0;j < 10-1;j++){
              swapFlag = 0; //ÿһ����ѭ������
                for(i = 0;i < 9-j;i++){
                        if( value[i] > value[i+1] ){ //>����   <����
                                res = value[i];
                                value[i] = value[i+1];
                                value[i+1] = res;

                                swapFlag = 1; //��������1
                        }
                }
                if(0 == swapFlag) break;//δ��������������ǰ����
        }
        med = (float)(*(value+4)+ *(value+5)+ *(value+6))/3.0f; //�м�ƽ��ֵ
        return med;
}
/*
        Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
        R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��
����p�ĳ�ֵ�������ȡ�����ǲ���Ϊ0��Ϊ0�Ļ��������˲�������Ϊ�Ѿ��������˲����ˣ�

q,r��ֵ��Ҫ�����Գ����������˾���(������¶ȼ��ж��ƣ��Լ���ĳ������ж�ǿ)

q���������˲����������ʵ�����ߵ�����̶ȣ�qԽ��Խ�ӽ���

r�������˲��������ƽ���̶ȣ�rԽ��Խƽ����

*/
float KalmanFilter1(float *Original_Data)
{
        static float prevData=0;
        static float p=10, q=0.0001, r=0.001, kGain=0;

        p = p+q;
        kGain = p/(p+r);

        *Original_Data = prevData+(kGain*(*Original_Data-prevData));
        p = (1-kGain)*p;

        prevData = *Original_Data;

        return *Original_Data;
}

float KalmanFilter2(float *Original_Data)
{
        static float prevData=0;
        static float p=10, q=0.0001, r=0.001, kGain=0;

        p = p+q;
        kGain = p/(p+r);

        *Original_Data = prevData+(kGain*(*Original_Data-prevData));
        p = (1-kGain)*p;

        prevData = *Original_Data;

        return *Original_Data;
}


float KalmanFilter3(float *Original_Data)
{
        static float prevData=0;
        static float p=10, q=0.0001, r=0.001, kGain=0;

        p = p+q;
        kGain = p/(p+r);

        *Original_Data = prevData+(kGain*(*Original_Data-prevData));
        p = (1-kGain)*p;

        prevData = *Original_Data;

        return *Original_Data;
}

float KalmanFilter4(float *Original_Data)
{
        static float prevData=0;
        static float p=10, q=0.0001, r=0.001, kGain=0;

        p = p+q;
        kGain = p/(p+r);

        *Original_Data = prevData+(kGain*(*Original_Data-prevData));
        p = (1-kGain)*p;

        prevData = *Original_Data;

        return *Original_Data;
}



