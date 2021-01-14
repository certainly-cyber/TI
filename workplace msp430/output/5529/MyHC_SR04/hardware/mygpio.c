/*
 * mygpio.c
 *
 *  Created on: 2020Äê8ÔÂ18ÈÕ
 *      Author: Dell
 */
# include"driverlib.h"
# include"mygpio.h"
void Mygpio_init()
{
    //P1.0 as trigger
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    //P1.2 as echo
    GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN2);
    GPIO_setAsInputPin(GPIO_PORT_P1,GPIO_PIN2);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P1,GPIO_PIN2);
    GPIO_selectInterruptEdge(GPIO_PORT_P1,GPIO_PIN2,GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN2);
    //P2.1 as switch
    GPIO_setAsInputPin(GPIO_PORT_P2,GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2,GPIO_PIN1);
    GPIO_selectInterruptEdge(GPIO_PORT_P2,GPIO_PIN1,GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P2,GPIO_PIN1);

}


