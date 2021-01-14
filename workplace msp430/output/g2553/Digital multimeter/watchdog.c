/*
 * watchdog.c
 *
 *  Created on: 2020年8月1日
 *      Author: certainly
 */
#include "msp430g2553.h"
#include "watchdog.h"
void watchdog_init()
{
    WDTCTL = WDT_ADLY_1000;                     // Set Watchdog Timer interval to ~1000ms
    IE1 |= WDTIE;                             // Enable WDT interrupt
}
void InitSystemClock(void)
{
    /*配置DCO为16MHz*/
    DCOCTL = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
    /*配置SMCLK的时钟源为DCO*/
    BCSCTL2 &= ~SELS;
    /*SMCLK的分频系数置为1*/
    BCSCTL2 &= ~(DIVS0 | DIVS1);

}



