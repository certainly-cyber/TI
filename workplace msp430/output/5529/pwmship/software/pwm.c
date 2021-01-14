/*
 * pwm.c
 *
 *  Created on: 2020年9月13日
 *      Author: certainly
 */
#include "driverlib.h"
#include "pwm.h"

#define DUTY_CYCLE1 108
#define DUTY_CYCLE2 102
#define TIMER_PERIOD1 127//100Hz
#define TIMER_PERIOD 528//50Hz
int i_pwm=0;
int steerpwm=3;
Timer_A_initCompareModeParam initComp1Param = {0};

Timer_A_initUpDownModeParam initUpDownParam = {0};
Timer_A_initUpModeParam initUpParam = {0};
void pwm_init()
{
   GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P2,
        GPIO_PIN0
        );
   GPIO_setAsOutputPin(
       GPIO_PORT_P1,
       GPIO_PIN2
       );
   GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN2);

    //Start Timer（电机）

    initUpDownParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    initUpDownParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_40
            ;
    initUpDownParam.timerPeriod = TIMER_PERIOD1;
    initUpDownParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initUpDownParam.captureCompareInterruptEnable_CCR0_CCIE =
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    initUpDownParam.timerClear = TIMER_A_DO_CLEAR;
    initUpDownParam.startTimer = false;
    Timer_A_initUpDownMode(TIMER_A1_BASE, &initUpDownParam);

    Timer_A_startCounter( TIMER_A1_BASE,
            TIMER_A_UPDOWN_MODE
            );
    //Initialze compare registers to generate PWM1(电机）

    initComp1Param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    initComp1Param.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    initComp1Param.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;
    initComp1Param.compareValue = DUTY_CYCLE1;
    Timer_A_initCompareMode(TIMER_A1_BASE, &initComp1Param);
    //Initialze compare registers to generate PWM2(舵机）
    initUpParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    initUpParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initUpParam.timerPeriod = TIMER_PERIOD;
    initUpParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initUpParam.captureCompareInterruptEnable_CCR0_CCIE =
           TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    initUpParam.timerClear = TIMER_A_DO_CLEAR;
    initUpParam.startTimer = false;
    Timer_A_initUpMode(TIMER_A2_BASE, &initUpParam);

       Timer_A_startCounter(TIMER_A2_BASE,
               TIMER_A_UP_MODE
               );


}
void setcompare_motor(int pwm)
{
    initComp1Param.compareValue = pwm;
    Timer_A_initCompareMode(TIMER_A1_BASE, &initComp1Param);  

}
void setcompare_steer(int pwm)
{
    steerpwm=pwm;

}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER2_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER1_A0_VECTOR)))
#endif
void TIMER0_A0_ISR (void)
{
    i_pwm=i_pwm+1;
if(i_pwm==steerpwm){

    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN2);
}
if(i_pwm==40){
    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN2);

    i_pwm=0;
}


//    GPIO_toggleOutputOnPin(
//        GPIO_PORT_P1,
//        GPIO_PIN2
//        );


}


