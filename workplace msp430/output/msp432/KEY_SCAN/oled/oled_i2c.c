/*
 * oled.c
 *
 *  Created on: 2020年8月11日
 *      Author: certainly
 */
/*
 * oled_i2c.c
 *
 *  Created on: 2020年7月26日
 *      Author: certainly
 */
/*
 * I2C_OLED.c
 *
 *  Created on: 2016年5月17日
 *      Author: xiaomo
 */
#include "driverlib.h"
#include"oled_i2c.h"
#include"codetab.h"
#include "stdio.h"
#include "delay.h"
int i=0;

u8 OLED_GRAM[128][8];    //显存
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
void OLED_WRITE_COM(u8 com)             //写指令
{
    Single_Write_OLED(0x00,com);
}

void OLED_WRITE_dat(u8 data)                //写数据
{
    Single_Write_OLED(0x40,data);
}

/*********************OLED 设置坐标************************************/
void OLED_Set_Pos(u8 x, u8 y) //设置OLED坐标
{
    OLED_WRITE_COM(0xb0+y);
    OLED_WRITE_COM(((x&0xf0)>>4)|0x10);
    OLED_WRITE_COM((x&0x0f)|0x01);
}
/*********************OLED全屏************************************/
void OLED_All(u8 bmp_dat)               //OLED全屏写同一数据
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
/*********************OLED复位************************************/
void OLED_CLC(void)   //清屏
{
    OLED_All(0);
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7**********/
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
void I2C_OLED_Init(void) //OLED初始化
{
    OLED_WRITE_COM(0xAE); //关闭显示
    OLED_WRITE_COM(0xD5); //设置时钟分频因子,震荡频率
    OLED_WRITE_COM(80);   //[3:0],分频因子;[7:4],震荡频率
    OLED_WRITE_COM(0xA8); //设置驱动路数
    OLED_WRITE_COM(0X3F); //默认0X3F(1/64)
    OLED_WRITE_COM(0xD3); //设置显示偏移
    OLED_WRITE_COM(0X00); //默认为0

    OLED_WRITE_COM(0x40); //设置显示开始行 [5:0],行数.

    OLED_WRITE_COM(0x8D); //电荷泵设置
    OLED_WRITE_COM(0x14); //bit2，开启/关闭
    OLED_WRITE_COM(0x20); //设置内存地址模式
    OLED_WRITE_COM(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    OLED_WRITE_COM(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
    OLED_WRITE_COM(0xC8); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    OLED_WRITE_COM(0xDA); //设置COM硬件引脚配置
    OLED_WRITE_COM(0x12); //[5:4]配置

    OLED_WRITE_COM(0x81); //对比度设置
    OLED_WRITE_COM(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
    OLED_WRITE_COM(0xD9); //设置预充电周期
    OLED_WRITE_COM(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WRITE_COM(0xDB); //设置VCOMH 电压倍率
    OLED_WRITE_COM(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_WRITE_COM(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_WRITE_COM(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示
    OLED_WRITE_COM(0xAF); //开启显示
    OLED_CLC();
}
//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
    u8 pos,bx,temp=0;
    if(x>127||y>63)return;//超出范围了.
    pos=7-y/8;
    bx=y%8;
    temp=1<<(7-bx);
    if(t)OLED_GRAM[x][pos]|=temp;
    else OLED_GRAM[x][pos]&=~temp;
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,清空;1,填充
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)
{
    u8 x,y;
    for(x=x1;x<=x2;x++)
    {
        for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
    }
    OLED_Refresh_Gram();//更新显示
}
//更新显存到LCD
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


/***************功能描述：显示6*8一组标准ASCII字符串  显示的坐标（x,y），y为页范围0～7****************/
void OLED_P6x8Str(u8 x,u8 y, u8 ch[])
{
    u8 c=0,i=0,j=0;
    while (ch[j]!='\0')
    {
        c =ch[j]-32;
        if(x>126){x=0;y++;}             //边界限制  强制拉回
        OLED_Set_Pos(x,y);
        for(i=0;i<6;i++)
        OLED_WRITE_dat(F6x8[c][i]);
        x+=6;
        j++;
    }
}
/*******************功能描述：显示8*16一组标准ASCII字符串  显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16Str(u8 x,u8 y,u8 ch[])
{
    u8 c=0,i=0,j=0;
    while (ch[j]!='\0')
    {
        c =ch[j]-32;
        if(x>120){x=0;y++;}                 //边界限制  强制拉回
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
/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
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
/*****************功能描述：将小数部分拆开并以整数的形式返回，n为小数个数****************************/
    int decimal(float a,int n)
    {
        int b=a;
        int i;
        a=a-b;
        for(i=0;i<n;i++){a=a*10;}
        int c=a;
        return c;
    }

    /*****************功能描述：显示小数***************************/
        void decimal_show(u8 x,u8 y,float a,int n)
        {
        int m=1;//这里自动检测整数的位数
        int i=0;
        int w=10;
        int z=0;
        int q=1;
        int remain=0;
        int remain1=0;
        int last=1;
        char b[5];
        char b2[6];
        int number[5]={0};//这里用于存放小数数字，是第一位小数倒着存放
        int decimal1=a;
        sprintf(b,"%d",decimal1);
        while(decimal1%w!=decimal1){w=w*10;m=m+1;}//计算整数部分的个数
        OLED_P8x16Str(x,y,b);//打印整数部分
        if(n!=0)//希望有小数才会计算、打印小数点
        {
            int decimal2=decimal(a,n);

                   for(i=0;i<n;i++)
                   {   remain=0;
                       for(w=0;w<n;w++)//用于计算已经存放的数字大小
                       {
                       remain=remain+last*number[w];
                       last=last*10;

                       }
                       last=1;//归位
                       remain1=decimal2;
                       remain1=remain1-remain;//差值为剩下的大小
                       q=1;//归位
                       for(z=0;z<n-i-1;z++){q=q*10;}
                       remain1=remain1/q;

                       number[n-i-1]=remain1;//这里为新存放的数字大小
                   }
                   i=0;
        OLED_P8x16Str(x+m*8, y, (unsigned char *)".");//打印小数点
        for(i=0;i<n;i++)
        {
        sprintf(b2,"%d",number[n-i-1]);
        OLED_P8x16Str(x+(i*8)+(m+1)*8, y,b2);//打印小数部分
        }
        }
        }










