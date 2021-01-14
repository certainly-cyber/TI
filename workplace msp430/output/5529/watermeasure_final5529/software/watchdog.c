/*
 * watchdog.c
 *
 *  Created on: 2020年8月13日
 *      Author: certainly
 */
# include"driverlib.h"
# include"watchdog.h"
void TIME_init()//! MCLK = SMCLK = default DCO=1M; ACLK = REFO~32kHz;
{

    //REFO Clock Sources ACLK
    UCS_initClockSignal(
        UCS_ACLK,
        UCS_REFOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );

}
void WATCHDOG_init()//时钟源为ACLK，2ms进入一次中断
{
WDT_A_initIntervalTimer(WDT_A_BASE,
        WDT_A_CLOCKSOURCE_ACLK,
        WDT_A_CLOCKDIVIDER_64);

    WDT_A_start(WDT_A_BASE);

    //Enable Watchdog Interupt
    SFR_clearInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
    SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
}




