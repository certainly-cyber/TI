/*
 * usart.c
 *
 *  Created on: 2020年9月13日
 *      Author: certainly
 */
#include "driverlib.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "pwm.h"
uint8_t receivedData = 0x00;
int flag=0;
int flag_switch=0;
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
    param.clockPrescalar = 109;
    param.firstModReg = 0;
    param.secondModReg = 2;
    param.parity = USCI_A_UART_NO_PARITY;
    param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
    param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
    param.uartMode = USCI_A_UART_MODE;
    param.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    if (STATUS_FAIL == USCI_A_UART_init(USCI_A0_BASE, &param))
      {
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

int fputc(int _c, register FILE *_fp)
{
  USCI_A_UART_transmitData(USCI_A0_BASE,_c);


  return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++)
  {
    USCI_A_UART_transmitData(USCI_A0_BASE,_ptr[i]);
  }

  return len;
}
//******************************************************************************
//
//This is the PORT1_VECTOR interrupt vector service routine
//
//******************************************************************************

//  Echo back RXed character
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR (void)
{


    switch (__even_in_range(UCA0IV,4)){

        //Vector 2 - RXIFG
        case 2:
            receivedData = USCI_A_UART_receiveData(USCI_A0_BASE);
            if(receivedData=='1'){GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0); setcompare_motor(103);}
            if(receivedData=='2'){ GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);setcompare_motor(112);}
            if(receivedData=='3'){setcompare_steer(1);}
            if(receivedData=='4'){setcompare_steer(5);}


            break;
        default: break;
    }
}




