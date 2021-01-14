/*
 * bluetooth.c
 *
 *  Created on: 2020年9月3日
 *      Author: certainly
 */

#ifndef BLUETOOTH_BLUETOOTH_C_
#define BLUETOOTH_BLUETOOTH_C_
#include"bluetooth.h"
#include"driverlib.h"
#include <stdint.h>
void GPIO_init()
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
void USART_init()
{    //P3.3,4 = USCI_A0 TXD/RXD
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P3,
        GPIO_PIN3 + GPIO_PIN4
        );

    //Baudrate = 9600, clock freq = 1.048MHz
    //UCBRx = 109, UCBRFx = 0, UCBRSx = 2, UCOS16 = 0
    USCI_A_UART_initParam param = {0};
    param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 2083;
    param.firstModReg = 0;
    param.secondModReg = 2;
    param.parity = USCI_A_UART_NO_PARITY;
    param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
    param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
    param.uartMode = USCI_A_UART_MODE;
    param.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    if (STATUS_FAIL == USCI_A_UART_init(USCI_A0_BASE, &param)){
        return;
    }

    //Enable UART module for operation
    USCI_A_UART_enable(USCI_A0_BASE);

    //Enable Receive Interrupt
      USCI_A_UART_clearInterrupt(USCI_A0_BASE,
        USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(USCI_A0_BASE,
        USCI_A_UART_RECEIVE_INTERRUPT);


}
void UartPutchar(unsigned char c)
{
    USCI_A_UART_transmitData(USCI_A0_BASE,c);
}
void uart_write_multi(uint8_t *p_buffer, uint8_t length)//写多个数据
{
    uint8_t index = length;
    while(index--)
    {
        USCI_A_UART_transmitData(USCI_A0_BASE,*p_buffer++);
    }
}
unsigned char UartGetchar()
{
 unsigned char c;
 c=USCI_A_UART_receiveData(USCI_A0_BASE);
 return c;
}



#endif /* BLUETOOTH_BLUETOOTH_C_ */
