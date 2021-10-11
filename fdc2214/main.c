#include "msp.h"
#include "delay.h"
#include "driverlib.h"
#include "oled_i2c.h"
#include "fdc2214main.h"
#include "fdc2214.h"
#include <stdint.h>
#include <stdbool.h>
#include <uart.h>
#include <stdio.h>
#include <string.h>
uint8_t receivedData = 0x00;
uint8_t transmitData = 0x00;
int aclk,mclk,smclk,hsmclk,bclk=0;
float a=0;
void UartPutchar(unsigned char c)
{
    MAP_UART_transmitData(EUSCI_A0_BASE,c);
}
void uart_write_multi(uint8_t *p_buffer, uint8_t length)//写多个数据
{
    uint8_t index = length;
    while(index--)
    {
        MAP_UART_transmitData(EUSCI_A0_BASE,*p_buffer++);
    }
}
unsigned char UartGetchar()
{
 unsigned char c;
 c=MAP_UART_receiveData(EUSCI_A0_BASE);
 return c;
}

int fputc(int _c, register FILE *_fp)
{
    MAP_UART_transmitData(EUSCI_A0_BASE,_c);


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
/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 9600 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
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

/**
 * main.c
 */
void main(void)
{
delay_init();
MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
FDC2214_Init();
MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
/* Selecting P1.2 and P1.3 in UART mode */
MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
        GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
  /* Enable UART module */
  MAP_UART_enableModule(EUSCI_A0_BASE);
  /* Enabling interrupts */
  MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
  MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
  MAP_Interrupt_enableSleepOnIsrExit();
  MAP_Interrupt_enableMaster();
I2C_OLED_Init();
OLED_All(0);//清屏
OLED_P8x16Str(0,0,(unsigned char*)"Voltage:");
OLED_P8x16Str(70,0,(unsigned char*)"0.0001");
OLED_P8x16Str(0,2,(unsigned char*)"Current:");
OLED_P8x16Str(78,2,(unsigned char*)"0.000");
OLED_P8x16Str(0,4,(unsigned char*)"Resistor:");
OLED_P8x16Str(78,4,(unsigned char*)"0.000");
while(1){

  a=get_single_capacity();
  printf("hi\r\n");
  printf("%f\r\n",a);
  delay_s(2);


}

}
void EUSCIA0_IRQHandler(void)
{

    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        receivedData=MAP_UART_receiveData(EUSCI_A0_BASE);
        if(receivedData=='4')MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
        if(receivedData=='5')MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }

}
