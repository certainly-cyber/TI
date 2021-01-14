/*
 * exti_interrupt.c
 *
 *  Created on: 2020Äê8ÔÂ11ÈÕ
 *      Author: certainly
 */
# include "driverlib.h"
# include "exti_interrupt.h"
void exti_interrupt_init()
{
       //PA.x output
       GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
       GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN7);
       GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
       GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);
       //Enable P1.1 internal resistance as pull-Up resistance
       GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN1);
       //P1.1 interrupt enabled
       GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN1);
       //P1.1 Hi/Lo edge
       GPIO_selectInterruptEdge(GPIO_PORT_P1,GPIO_PIN1,GPIO_HIGH_TO_LOW_TRANSITION);
       //P1.1 IFG cleared
       GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN1);
       //Enable P2.1 internal resistance as pull-Up resistance
       GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2,GPIO_PIN1);
       //P2.1 interrupt enabled
       GPIO_enableInterrupt(GPIO_PORT_P2,GPIO_PIN1);
       //P2.1 Hi/Lo edge
       GPIO_selectInterruptEdge(GPIO_PORT_P2,GPIO_PIN1,GPIO_HIGH_TO_LOW_TRANSITION);
       //P2.1 IFG cleared
       GPIO_clearInterrupt(GPIO_PORT_P2,GPIO_PIN1);
}



