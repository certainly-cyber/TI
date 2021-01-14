/*
 * mytimer.c
 *
 *  Created on: 2020Äê8ÔÂ18ÈÕ
 *      Author: Dell
 */
# include"driverlib.h"
# include"mytimer.h"
void Mytimer_init()
{
    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);
    Timer_A_initContinuousModeParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    param.timerClear = TIMER_A_DO_CLEAR;
    param.startTimer = false;
    Timer_A_initContinuousMode(TIMER_A1_BASE, &param);
}



