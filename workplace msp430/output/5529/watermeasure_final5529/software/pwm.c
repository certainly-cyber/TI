/*
 * pwm.c
 *
 *  Created on: 2020Äê8ÔÂ13ÈÕ
 *      Author: certainly
 */
# include"driverlib.h"
# include"pwm.h"
Timer_B_initCompareModeParam initComp1Param = {0};
void PWM_init()
{
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN5
        );



    //Start timer
    Timer_B_initUpModeParam initUpParam = {0};
    initUpParam.clockSource = TIMER_B_CLOCKSOURCE_SMCLK;
    initUpParam.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_4;
    initUpParam.timerPeriod = 1000;
    initUpParam.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
    initUpParam.captureCompareInterruptEnable_CCR0_CCIE =
        TIMER_B_CCIE_CCR0_INTERRUPT_DISABLE;
    initUpParam.timerClear = TIMER_B_DO_CLEAR;
    initUpParam.startTimer = false;
    Timer_B_initUpMode(TIMER_B0_BASE, &initUpParam);

    Timer_B_startCounter(TIMER_B0_BASE,
        TIMER_B_UP_MODE
        );

    //Initialize compare mode to generate PWM1

    initComp1Param.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_5;
    initComp1Param.compareInterruptEnable = TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE;
    initComp1Param.compareOutputMode = TIMER_B_OUTPUTMODE_RESET_SET;
    initComp1Param.compareValue = 750;
    Timer_B_initCompareMode(TIMER_B0_BASE, &initComp1Param);
    __enable_interrupt();

}
void setcompare(int pwm)
{
    initComp1Param.compareValue = pwm;
    Timer_B_initCompareMode(TIMER_B0_BASE, &initComp1Param);
}

