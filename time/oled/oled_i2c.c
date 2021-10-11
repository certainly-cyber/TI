/*
 * oled.c
 *
 *  Created on: 2020��8��11��
 *      Author: certainly
 */
/*
 * oled_i2c.c
 *
 *  Created on: 2020��7��26��
 *      Author: certainly
 */
/*
 * I2C_OLED.c
 *
 *  Created on: 2016��5��17��
 *      Author: xiaomo
 */
#include "driverlib.h"
#include"oled_i2c.h"
#include"codetab.h"
#include "stdio.h"
#include "delay.h"
int i=0;

u8 OLED_GRAM[128][8];    //�Դ�
#define SLAVE_ADDRESS       0x3C
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        48000000,                                // SMCLK = 3MHz (default)
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};
//P6.6 SDA
//P6.7 SCL
void oled_i2c_init()
{
MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
           GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

   /* Initializing I2C Master to SMCLK at 100khz with no autostop */
   MAP_I2C_initMaster(EUSCI_B2_BASE, &i2cConfig);

   /* Specify slave address */
   MAP_I2C_setSlaveAddress(EUSCI_B2_BASE, SLAVE_ADDRESS);

   /* Enable I2C Module to start operations */
   MAP_I2C_enableModule(EUSCI_B2_BASE);
}
void Single_Write_OLED(u8 REG_Address,u8 REG_data)
{
    /* Making sure the last transaction has been completely sent out */
//    while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE));

    /* Send out EEPROM Mock Read Cmd (2 databytes) */

    MAP_I2C_masterSendMultiByteStart(EUSCI_B2_BASE, 0x3C);  // Start + 1Byte
    for(i=0;i<1230;i++){;}
    //delay_us(1);
    MAP_I2C_masterSendMultiByteNext(EUSCI_B2_BASE,REG_Address); // Poll for TXINT,Send 1Byte
    for(i=0;i<1230;i++){;}
    //delay_us(1);
    MAP_I2C_masterSendMultiByteNext(EUSCI_B2_BASE, REG_data); // Poll for TXINT,Send 1Byte
    for(i=0;i<1230;i++){;}
    //delay_us(1);
    /*---------------------------------------------*/
    /* Now we need to initiate the read */
    /* Wait until 2nd Byte has been output to shift register */
//    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
    MAP_I2C_masterSendMultiByteStop(EUSCI_B2_BASE);
    //delay_us(1);
    for(i=0;i<1230;i++){;}
}
void OLED_WRITE_COM(u8 com)             //дָ��
{
    Single_Write_OLED(0x00,com);
}

void OLED_WRITE_dat(u8 data)                //д����
{
    Single_Write_OLED(0x40,data);
}

/*********************OLED ��������************************************/
void OLED_Set_Pos(u8 x, u8 y) //����OLED����
{
    OLED_WRITE_COM(0xb0+y);
    OLED_WRITE_COM(((x&0xf0)>>4)|0x10);
    OLED_WRITE_COM((x&0x0f)|0x01);
}
/*********************OLEDȫ��************************************/
void OLED_All(u8 bmp_dat)               //OLEDȫ��дͬһ����
{
    u8 y,x;
    for(y=0;y<8;y++)
    {
        OLED_WRITE_COM(0xb0+y);
        OLED_WRITE_COM(0x01);
        OLED_WRITE_COM(0x10);
        for(x=0;x<128;x++)
        OLED_WRITE_dat(bmp_dat);
    }
}
/*********************OLED��λ************************************/
void OLED_CLC(void)   //����
{
    OLED_All(0);
}

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7**********/
void Draw_BMP(const u8 *BMP)
{
    u8 x,y;
    for(y=0;y<8;y++)
    {
        OLED_Set_Pos(0,y);
    for(x=0;x<128;x++)
        OLED_WRITE_dat(*BMP++);
    }
}
void I2C_OLED_Init(void) //OLED��ʼ��
{
    OLED_WRITE_COM(0xAE); //�ر���ʾ
    OLED_WRITE_COM(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
    OLED_WRITE_COM(80);   //[3:0],��Ƶ����;[7:4],��Ƶ��
    OLED_WRITE_COM(0xA8); //��������·��
    OLED_WRITE_COM(0X3F); //Ĭ��0X3F(1/64)
    OLED_WRITE_COM(0xD3); //������ʾƫ��
    OLED_WRITE_COM(0X00); //Ĭ��Ϊ0

    OLED_WRITE_COM(0x40); //������ʾ��ʼ�� [5:0],����.

    OLED_WRITE_COM(0x8D); //��ɱ�����
    OLED_WRITE_COM(0x14); //bit2������/�ر�
    OLED_WRITE_COM(0x20); //�����ڴ��ַģʽ
    OLED_WRITE_COM(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
    OLED_WRITE_COM(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
    OLED_WRITE_COM(0xC8); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
    OLED_WRITE_COM(0xDA); //����COMӲ����������
    OLED_WRITE_COM(0x12); //[5:4]����

    OLED_WRITE_COM(0x81); //�Աȶ�����
    OLED_WRITE_COM(0xEF); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
    OLED_WRITE_COM(0xD9); //����Ԥ�������
    OLED_WRITE_COM(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WRITE_COM(0xDB); //����VCOMH ��ѹ����
    OLED_WRITE_COM(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_WRITE_COM(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
    OLED_WRITE_COM(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
    OLED_WRITE_COM(0xAF); //������ʾ
    OLED_CLC();
}
//����
//x:0~127
//y:0~63
//t:1 ��� 0,���
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
    u8 pos,bx,temp=0;
    if(x>127||y>63)return;//������Χ��.
    pos=7-y/8;
    bx=y%8;
    temp=1<<(7-bx);
    if(t)OLED_GRAM[x][pos]|=temp;
    else OLED_GRAM[x][pos]&=~temp;
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,���;1,���
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)
{
    u8 x,y;
    for(x=x1;x<=x2;x++)
    {
        for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
    }
    OLED_Refresh_Gram();//������ʾ
}
//�����Դ浽LCD
void OLED_Refresh_Gram(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_Set_Pos(0,i);
        for(n=0;n<128;n++)
        OLED_WRITE_dat(OLED_GRAM[n][i]);
    }
}


/***************������������ʾ6*8һ���׼ASCII�ַ���  ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void OLED_P6x8Str(u8 x,u8 y, u8 ch[])
{
    u8 c=0,i=0,j=0;
    while (ch[j]!='\0')
    {
        c =ch[j]-32;
        if(x>126){x=0;y++;}             //�߽�����  ǿ������
        OLED_Set_Pos(x,y);
        for(i=0;i<6;i++)
        OLED_WRITE_dat(F6x8[c][i]);
        x+=6;
        j++;
    }
}
/*******************������������ʾ8*16һ���׼ASCII�ַ���  ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void OLED_P8x16Str(u8 x,u8 y,u8 ch[])
{
    u8 c=0,i=0,j=0;
    while (ch[j]!='\0')
    {
        c =ch[j]-32;
        if(x>120){x=0;y++;}                 //�߽�����  ǿ������
        OLED_Set_Pos(x,y);
        for(i=0;i<8;i++)
        OLED_WRITE_dat(F8X16[c*16+i]);
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
        OLED_WRITE_dat(F8X16[c*16+i+8]);
        x+=8;
        j++;
    }
}
/*****************������������ʾ16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************************/
void OLED_P16x16Ch(u8 x,u8 y,u8 num)
{
        u8 wm=0;
        u16 adder=32*num;
        OLED_Set_Pos(x , y);
        for(wm = 0;wm < 16;wm++)
        {
            OLED_WRITE_dat(F16x16[adder]);
            adder += 1;
        }
        OLED_Set_Pos(x,y + 1);
        for(wm = 0;wm < 16;wm++)
        {
            OLED_WRITE_dat(F16x16[adder]);
            adder += 1;
        }
}
/*****************������������С�����ֲ𿪲�����������ʽ���أ�nΪС������****************************/
    int decimal(float a,int n)
    {
        int b=a;
        int i;
        a=a-b;
        for(i=0;i<n;i++){a=a*10;}
        int c=a;
        return c;
    }

    /*****************������������ʾС��***************************/
        void decimal_show(u8 x,u8 y,float a,int n)
        {
        int m=1;//�����Զ����������λ��
        int i=0;
        int w=10;
        int z=0;
        int q=1;
        int remain=0;
        int remain1=0;
        int last=1;
        char b[5];
        char b2[6];
        int number[5]={0};//�������ڴ��С�����֣��ǵ�һλС�����Ŵ��
        int decimal1=a;
        sprintf(b,"%d",decimal1);
        while(decimal1%w!=decimal1){w=w*10;m=m+1;}//�����������ֵĸ���
        OLED_P8x16Str(x,y,b);//��ӡ��������
        if(n!=0)//ϣ����С���Ż���㡢��ӡС����
        {
            int decimal2=decimal(a,n);

                   for(i=0;i<n;i++)
                   {   remain=0;
                       for(w=0;w<n;w++)//���ڼ����Ѿ���ŵ����ִ�С
                       {
                       remain=remain+last*number[w];
                       last=last*10;

                       }
                       last=1;//��λ
                       remain1=decimal2;
                       remain1=remain1-remain;//��ֵΪʣ�µĴ�С
                       q=1;//��λ
                       for(z=0;z<n-i-1;z++){q=q*10;}
                       remain1=remain1/q;

                       number[n-i-1]=remain1;//����Ϊ�´�ŵ����ִ�С
                   }
                   i=0;
        OLED_P8x16Str(x+m*8, y, (unsigned char *)".");//��ӡС����
        for(i=0;i<n;i++)
        {
        sprintf(b2,"%d",number[n-i-1]);
        OLED_P8x16Str(x+(i*8)+(m+1)*8, y,b2);//��ӡС������
        }
        }
        }










