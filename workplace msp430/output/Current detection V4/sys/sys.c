/*
 * sys.c
 *
 *  Created on: 2020年8月30日
 *      Author: DELL
 */
#include <msp430.h>
#include "sys.h"


void Init_20MHz_MCLK()
{

//    P1DIR |= BIT0;           //配置P1.0位输出模式
//    P1SEL |= BIT0;           //功能服用：ACLK输出（备份时钟）
//    P2DIR |= BIT2;           //配置P2.2位输出模式
//    P2SEL |= BIT2;           //功能服用：SMCLK输出（子系统时钟输出）
//    P7DIR |= BIT7;           //配置P7.7位输出模式
//    P7SEL |= BIT7;           //功能服用：MCLK输出（主系统时钟输出）
    P5SEL |= BIT4|BIT5;
    PMMCTL0_H = 0xA5;                                 //解锁PMM寄存器，开启PMM电源管理
    SVSMLCTL |= SVSMLRRL_1 + SVMLE;                   //配置SVML电压
    PMMCTL0 = PMMPW + PMMCOREV_3;                     //配置内核电呀
    while((PMMIFG & SVSMLDLYIFG) == 0);               //等待设置完成
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG + SVSMLDLYIFG);
    if((PMMIFG & SVMLIFG) == 1)                       //判断电压是否上升到VSVML
        while((PMMIFG & SVMLVLRIFG) == 0);            //如果没有则等待
    SVSMLCTL &= ~SVMLE;                                //关掉SVML模块
    PMMCTL0_H = 0X00;                                  //重新上锁锁存配置

/*******************************************
时钟调配模块，配置时钟
**********************************/
    __bis_SR_register(SCG0);                           //给SR寄存器的SCGC0位置1，
                                                       //关闭锁相环，开始对时钟进行配置
    UCSCTL0 = 0;
    UCSCTL1 = DCORSEL_6;
    UCSCTL2 = FLLD_1 |344;
    __bic_SR_register(SCG0);                            //给SR寄存器的SCGC0位置0，
    __delay_cycles(782000);

    while(SFRIFG1 & OFIFG){                             //检测OFIFG fault flag
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);     //clear OSC fault flags
        SFRIFG1 &= ~OFIFG;                              //清除OFIFG fault flag
    }
    UCSCTL4 = UCSCTL4&(~(SELS_7|SELM_7))|SELS_3|SELM_3;//时钟选择，MCLK\SMCLK=24M，ACLK=32.768K

/*******************************************
GPIO口调配模块，便于调配时通过IO口输出，观察频率，不用则注释掉
**********************************/

}

