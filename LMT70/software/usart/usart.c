/*
 * usart.c
 *
 *  Created on: 2020Äê10ÔÂ5ÈÕ
 *      Author: certainly
 */
#include "usart.h"
#include "driverlib.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        312,                                     // BRDIV = 78
        8,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling

};
void usart_init()
{
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
               GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);





       //![Simple UART Example]
       /* Configuring UART Module */
       MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

//       /* Enable UART module */
      MAP_UART_enableModule(EUSCI_A0_BASE);
//
//       /* Enabling interrupts */
//       MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
//       MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
//       MAP_Interrupt_enableSleepOnIsrExit();
//       MAP_Interrupt_enableMaster();

}
int fputc(int _c, register FILE *_fp)
{

  MAP_UART_transmitData(EUSCI_A0_BASE,_c );


  return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++)
  {
    MAP_UART_transmitData(EUSCI_A0_BASE,_ptr[i]);
  }

  return len;
}


