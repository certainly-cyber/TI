


#include "driverlib.h"
#include "mygpio.h"
#include "mytimer.h"
#include "delay.h"
int CountFlag=0;
unsigned int Count=0;
double time=0;
int EchoFlag=1;
double distance=0;
void main (void)
{
    //Stop WDT
    WDT_A_hold(WDT_A_BASE);
    Mygpio_init();
    Mytimer_init();
    //Enter LPM0
    __bis_SR_register(LPM0_bits + GIE);
    //For debugger
    __no_operation();
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (GPIO_getInterruptStatus(GPIO_PORT_P1,GPIO_PIN2)&&EchoFlag) // echo 端输入上升沿 开始计数
    {
        Timer_A_startCounter(TIMER_A1_BASE,TIMER_A_CONTINUOUS_MODE);
        GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN2);
        GPIO_selectInterruptEdge(GPIO_PORT_P1,GPIO_PIN2,GPIO_HIGH_TO_LOW_TRANSITION);
        GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN2);
        EchoFlag--;
    }
    else  if (GPIO_getInterruptStatus(GPIO_PORT_P1,GPIO_PIN2)&&(!EchoFlag)) // echo 端输入下降沿 停止计数
    {
        Timer_A_stop(TIMER_A1_BASE);
        Count=Timer_A_getCounterValue(TIMER_A1_BASE);
        time=((double)Count+CountFlag*65536)/1048576;
        distance=time*340/2;
        GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN2);
        GPIO_selectInterruptEdge(GPIO_PORT_P1,GPIO_PIN2,GPIO_LOW_TO_HIGH_TRANSITION);
        GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN2);
        EchoFlag++;
    }
}
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if (GPIO_getInterruptStatus(GPIO_PORT_P2,GPIO_PIN1)) //  按键按下
    {
        GPIO_clearInterrupt(GPIO_PORT_P2,GPIO_PIN1);
        GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
        delay_us(11);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    }
}


#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR (void)
{
    switch ( __even_in_range(TA1IV,14) ){
        case  0: break;                          //No interrupt
        case  2: break;                          //CCR1 not used
        case  4: break;                          //CCR2 not used
        case  6: break;                          //CCR3 not used
        case  8: break;                          //CCR4 not used
        case 10: break;                          //CCR5 not used
        case 12: break;                          //CCR6 not used
        case 14:
           CountFlag++;
            break;
        default: break;
    }
}
