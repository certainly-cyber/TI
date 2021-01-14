#include "driverlib.h"
#include <msp430.h>
#include "oled_i2c.h"
#include "zimo.h"
#include "watchdog.h"
#include "exti_interrupt.h"
#include "mathclub.h"
#include "hardware_ic.h"
#include "ADS112C04.h"
#include "math.h"

/**c
 * main.c
 */
int i=0;
int flag=0;
int select = 0;
//   float a[2]={2.1,3.1};
float a=1.999;
int main(void)
{

    //hyx注释
    volatile float sum = 0; // 直流交流求和临时数值
    volatile uint8_t e = 0; //if判断中间变量
    volatile float d = 0; //直流档平均值
    volatile int c = 0; //直流取10次的中间变量
    volatile int b = 0; //从十六进制转十进制的中间变量
    volatile float a[10] = { 0 }; //直流电压数组
    volatile float ac = 0;

    //hyx修改
    volatile int counterhelp = 0; //交流取样0xff循环数
    volatile int countersum = 0; //循环总数
    volatile float final = 0; //交流电有效值

//    uint8_t data[2] = {0,0};
    uint8_t data[3] = { 0, 0, 0 }; //从ADS寄存器取得的原始数值
    uint8_t reg[2] = { 0, 0 }; //ADS转换完成取值位置
    initI2C();
    initADS();

TIME_init();
WATCHDOG_init();
exti_interrupt_init();
I2C_OLED_Init();
OLED_All(0);//清屏
OLED_P8x16Str(0,0,(unsigned char*)"Voltage:");
//OLED_P8x16Str(70,0,(unsigned char*)"0.0000");
OLED_P8x16Str(0,2,(unsigned char*)"Current:");
OLED_P8x16Str(78,2,(unsigned char*)"0.000");
OLED_P8x16Str(0,4,(unsigned char*)"Resistor:");
OLED_P8x16Str(78,4,(unsigned char*)"0.000");
decimal_show(70,0,8.767,1,3);
//float d=urms(2,a);
__enable_interrupt();
while(1)
{
 while(flag==1)
 {
     adsPowerDown();
     __bis_SR_register(LPM3_bits + GIE);
 }// Enter LPM3 w/ interrupt
 if(flag==0)
 {
     OLED_P8x16Str(0,6,(unsigned char*)"Nor-power-mode");
 }
 if (flag == 0)
 {
//            OLED_P8x16Str(0, 6, (unsigned char*) "Nor-power-mode");
     if (select == 1)                   //2v量程
     {

             adsReadREG(0x02, reg);     //读寄存器2
             e = DRDY & reg[0];        //确认转换完成
             if (e == 128)
             {
                 adsReadDATA(data);        //读数据
                 b = (int) ((data[0] << 8) + data[1]);
                 a[c] = (float) b / 32768 * 2.048;
                 sum = sum + a[c];
                 c = c + 1;
                 if (c == 10)
                 {
                     c = 0;
                     d = sum / 10;
                     sum = 0;
                     //   d = (a[0]+a[1]+a[2]+a[3]+a[4]+a[5]+a[6]+a[7]+a[8]+a[9])/10;
                     decimal_show(70, 0, d, 1, 4);
//                            OLED_P8x16Str(0, 6, (unsigned char*) "mode:2V");
                 }
                 OLED_P8x16Str(0, 6,(unsigned char*) "mode:2V           ");
             }


     }


     if (select == 2)                   //0.2v量程
     {
//                adsPowerDown();
//                initADS(0x81, 0x18, 0, 0);
//                __delay_cycles(3000);
//                adsStartCONV();

             adsReadREG(0x02, reg);     //读寄存器2
             e = DRDY & reg[0];        //确认转换完成
             if (e == 128)
             {
                 adsReadDATA(data);        //读数据
                 b = (int) ((data[0] << 8) + data[1]);
                 a[c] = (float) b / 32768 * 2.048;
                 sum = sum + a[c];
                 c = c + 1;
                 if (c == 10)
                 {
                     c = 0;
                     d = sum / 100;
                     sum = 0;
                     //   d = (a[0]+a[1]+a[2]+a[3]+a[4]+a[5]+a[6]+a[7]+a[8]+a[9])/10;
                     decimal_show(70, 0, d, 1, 4);
                     //OLED_P8x16Str(0, 6, (unsigned char*) "mode:2V");
                 }
                 OLED_P8x16Str(0, 6,(unsigned char*) "mode:0.2V           ");
             }


     }


     if (select == 3)              //200k量程
     {
//                adsPowerDown();
//                initADS(0x81, 0x18, 0, 0);
//                __delay_cycles(3000);
//                adsStartCONV();

             adsReadREG(0x02, reg);     //读寄存器2
             e = DRDY & reg[0];        //确认转换完成
             if (e == 128)
             {
                 adsReadDATA(data);        //读数据
                 b = (int) ((data[0] << 8) + data[1]);
                 a[c] = (float) b / 32768 * 2.048;
                 sum = sum + a[c];
                 c = c + 1;
                 if (c == 10)
                 {
                     c = 0;
                     d = sum / 0.0001;
                     sum = 0;
                     //   d = (a[0]+a[1]+a[2]+a[3]+a[4]+a[5]+a[6]+a[7]+a[8]+a[9])/10;
                     decimal_show(70, 0, d, 1, 4);
                     //                            OLED_P8x16Str(0, 6, (unsigned char*) "mode:2V");
                 }
                 OLED_P8x16Str(0, 6,
                               (unsigned char*) "mode:200K           ");
             }


     }


     if (select == 4)            //2欧量程
     {
//                adsPowerDown();
//                initADS(0x81, 0x18, 0, 0);
//                __delay_cycles(3000);
//                adsStartCONV();

             adsReadREG(0x02, reg);     //读寄存器2
             e = DRDY & reg[0];        //确认转换完成
             if (e == 128)
             {
                 adsReadDATA(data);        //读数据
                 b = (int) ((data[0] << 8) + data[1]);
                 a[c] = (float) b / 32768 * 2.048;
                 sum = sum + a[c];
                 c = c + 1;
                 if (c == 10)
                 {
                     c = 0;
                     d = sum / 10;
                     sum = 0;
                     //   d = (a[0]+a[1]+a[2]+a[3]+a[4]+a[5]+a[6]+a[7]+a[8]+a[9])/10;
                     decimal_show(70, 0, d, 1, 4);
                     //                            OLED_P8x16Str(0, 6, (unsigned char*) "mode:2V");
                 }
                 OLED_P8x16Str(0, 6,(unsigned char*) "mode:2ou           ");
             }




     }

     if (select == 5)            //交流电压量程0.2V
     {
//                adsPowerDown();
//                initADS(0x81, 0x18, 0x98, 0);
//                __delay_cycles(3000);
//                adsStartCONV();

             adsReadREG(0x02,reg);//读刚才设置的寄存器，确认写入成功
             e = DRDY & reg[0];
             if(e!=0)
             {
             adsReadcounterPLUSdata(data);//读数据
                 if(data[0]== 255)
                 {
                     counterhelp++;
                 }
                 countersum = 255*counterhelp + data[0];
             b = (int) ((data[1]<< 8) + data[2]);
             ac = (float)b / 32768 * 2.048;
             ac = (ac-1)*2/10;
             sum = sum + ac*ac;
                 if(countersum == 1000)
                 {
                     final=sqrt(sum/1000);
                     sum = 0;
                     countersum = 0;
                     decimal_show(70, 0, final, 1, 4);
                 }
                 OLED_P8x16Str(0, 6,(unsigned char*) "mode:ac0.2V           ");
             }


     }



     if (select == 6)            //交流电压量程2V
     {
//                adsPowerDown();
//                initADS(0x81, 0x18, 0x98, 0);
//                __delay_cycles(3000);
//                adsStartCONV();

             adsReadREG(0x02,reg);//读刚才设置的寄存器，确认写入成功
             e = DRDY & reg[0];
             if(e!=0)
             {
             adsReadcounterPLUSdata(data);//读数据
                 if(data[0]== 255)
                 {
                     counterhelp++;
                 }
                 countersum = 255*counterhelp + data[0];
             b = (int) ((data[1]<< 8) + data[2]);
             ac = (float)b / 32768 * 2.048;
             ac = (ac-1)*2;
             sum = sum + ac*ac;
                 if(countersum == 1000)
                 {
                     final=sqrt(sum/1000);
                     sum = 0;
                     countersum = 0;
                     decimal_show(70, 0, final, 1, 4);
                 }
                 OLED_P8x16Str(0, 6,(unsigned char*) "mode:ac2V           ");
             }


     }

 }


}
return 0;
}


//Watchdog Timer interrupt service routine
//1 min会进入LPM3
#pragma vector=WDT_VECTOR
__interrupt void WDT_A_ISR (void)
{
    i=i+1;
    if(i==10)
    {
      i=0;
      flag=1;
      OLED_P8x16Str(0,6,(unsigned char*)"Low-power-mode");//显示进入低功耗
      adsPowerDown();
      __bis_SR_register(LPM3_bits + GIE);
    }
}
//******************************************************************************
//
//This is the PORT1_VECTOR interrupt vector service routine
//
//******************************************************************************
#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void)
{
    unsigned int temp1;
    int i1;
    for(i1=0;i1<12000;i1++);//延时消抖
    if((P1IN&0xff)!=0xff)
    {
     temp1=P1IFG;
     P1IFG=0x00;
     if(temp1==0x02)
     {

     }//P1.1被按下
    }


    if(flag==1)
    {
        LPM3_EXIT;
    }

    flag=0;
    i=0;
    return;

}

//******************************************************************************
//
//This is the PORT2_VECTOR interrupt vector service routine
//
//******************************************************************************
#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void)
{
    unsigned int temp2;
    int i2;
    for(i2=0;i2<12000;i2++);//延时消抖
    if((P2IN&0xff)!=0xff)
    {temp2=P2IFG;
     P2IFG=0x00;
     if(temp2==0x02)
     {

     }//P2.1被按下
    }
    if(flag==1)
    {
        LPM3_EXIT;
    }

    flag=0;
    i=0;
    return;


}

