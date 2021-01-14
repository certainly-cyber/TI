#include "driverlib.h"
#include "pwm.h"
#include "usart.h"
int i=0;
int flag2=0;
void main()
{
    WDT_A_hold(WDT_A_BASE);
    USART_init();
    pwm_init();
    //PA.x output
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);

    __enable_interrupt();
    while(1){

    }

}


