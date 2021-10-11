/*
 * lmt70.c
 *
 *  Created on: 2020��10��5��
 *      Author: certainly
 */
#include "driverlib.h"
#include "lmt70.h"
#include "delay.h"
#include "usart.h"
#define VOL2TEMP_TABLE_SIZE 21
unsigned char elech,elecm,elecl,config,config2=0;
uint32_t AD_B_Result=0;
float AD_F_Result=0;
float final_tem_70=0;
int i,i1=0;
//�¶ȱ�
float v2t_tabl[VOL2TEMP_TABLE_SIZE] = {
                                1350.441,1300.593,1250.398,1199.884,1149.070,
                                1097.987,1046.647, 995.050, 943.227, 891.178,
                                 838.882, 786.360, 733.608, 680.654, 627.490,
                                 574.117, 520.551, 466.760, 412.739, 358.164,
                                 302.785
} ;

/* б�ʱ���ʮ��������ʹ�����Բ�ֵ�� */
float slope_tabl[VOL2TEMP_TABLE_SIZE] = {
                                4.985,5.020,5.051,5.081,5.108,
                                5.134,5.160,5.182,5.205,5.230,
                                5.252,5.275,5.295,5.316,5.337,
                                5.357,5.379,5.402,5.458,5.538,
                                5.538
} ;


/* �۰���ʵ�� */
int16_t haltserach( float vol)
{
    int16_t low = 0 ;
    int16_t up = VOL2TEMP_TABLE_SIZE ;
    int16_t mid ;

    while ( up >= low)
    {
        mid = ( low + up) >> 1 ;

        if ( v2t_tabl[mid] < vol)
        {
            up = mid - 1 ;
        }
        else if ( vol < v2t_tabl[mid])
        {
            low = mid + 1 ;
        }
        else
        {
            return mid ;
        }
    }

    /* �������Ա������-1��RT2TEMP_TABLE_SIZE��ʾ���ڱ��� */
    /* ����ֵ����ȡ�±߽��¶Ȳ����� */

    return up ;
}

/** @brief  ��lmt70��ȡ�¶�����
  * @details ʹ��һ�׵������ʵ�ֵ�ѹ-�¶�ת��
  * @param[in]  vol lmt70�����ѹ
  * @retval ��ѹ-�¶�ת������Ŵ�100�� -5000��+15000��
  */
float lmt70_get_temp( float vol)
{
    int16_t i ;
    float rev = 0 ;

    i = haltserach(vol) ;

    if ( ( i != -1) && ( i != VOL2TEMP_TABLE_SIZE))
    {
//        rev = ( ( ( i * 10) - 50) + ( ( v2t_tabl[i] - vol) / slope_tabl[i])) * 100 ;
        rev = ( ( ( (float)i * 10) - 50) + ( ( v2t_tabl[i] - vol) / slope_tabl[i]))  ;
    }

    return rev ;
}
void lmt70_init()
{
   MAP_I2C_masterSendMultiByteStart(EUSCI_B2_BASE, 0x68);  // Start + 1Byte
   for(i=0;i<1230;i++){;}
    MAP_I2C_masterSendMultiByteNext(EUSCI_B2_BASE,0x9c); //����ת��
   for(i=0;i<1230;i++){;}
   MAP_I2C_masterSendMultiByteStop(EUSCI_B2_BASE);
   for(i=0;i<1230;i++){;}
}
float memread_lmt70()
{
    I2C_masterReceiveStart(EUSCI_B2_BASE);
    for(i1=0;i1<1230;i1++){;}
    for(i1=0;i1<1230;i1++){;}
    elech=MAP_I2C_masterReceiveMultiByteNext(EUSCI_B2_BASE);
    for(i1=0;i1<1230;i1++){;}
    elecm=MAP_I2C_masterReceiveMultiByteNext(EUSCI_B2_BASE);
    for(i1=0;i1<1230;i1++){;}
    elecl=MAP_I2C_masterReceiveMultiByteNext(EUSCI_B2_BASE);
    for(i1=0;i1<1230;i1++){;}
    MAP_I2C_masterReceiveMultiByteStop(EUSCI_B2_BASE);
    for(i1=0;i1<1230;i1++){;}
    config= MAP_I2C_masterReceiveMultiByteNext(
            EUSCI_B2_BASE);
    for(i1=0;i1<1230;i1++){;}
    config2= MAP_I2C_masterReceiveMultiByteNext(
            EUSCI_B2_BASE);
    for(i1=0;i1<1230;i1++){;}
    AD_B_Result=(elech<<16)|(elecm<<8)|(elecl);
    AD_B_Result=AD_B_Result&0x01ffff;
    final_tem_70=((float)AD_B_Result)*0.015625;
    return final_tem_70;

}
float tem_get()
{   float finaltem;
    finaltem=memread_lmt70();
    finaltem=lmt70_get_temp(finaltem);
    return finaltem;
}

