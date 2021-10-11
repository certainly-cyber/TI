#include "msp.h"
#include "delay.h"
#include "driverlib.h"
#include "oled_i2c.h"
#include "mpu6050.h"
#include "mpu6050main.h"
#include <stdint.h>
#include <stdbool.h>
#include <uart.h>
#include <stdio.h>
#include <string.h>
#include "filter.h"
uint8_t receivedData = 0x00;
uint8_t transmitData = 0x00;
int aclk,mclk,smclk,hsmclk,bclk=0;
short a=0;
u8 res1=1;
short x,y,z=0;
#define CALIBRATION_START 0x0003F000
unsigned long *Ptr1=(unsigned long*)0x0003F000;
uint8_t simulatedCalibrationData[4096]={0};
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
//I2C_OLED_Init();
//OLED_All(0);//清屏
//OLED_P8x16Str(0,0,(unsigned char*)"Voltage:");
//OLED_P8x16Str(70,0,(unsigned char*)"0.0001");
//OLED_P8x16Str(0,2,(unsigned char*)"Current:");
//OLED_P8x16Str(78,2,(unsigned char*)"0.000");
//OLED_P8x16Str(0,4,(unsigned char*)"Resistor:");
//OLED_P8x16Str(78,4,(unsigned char*)"0.000");
MPU_Init();
//delay_s(2);
//MPU_Init();
while(1){


a=MPU_Get_Temperature();
printf("温度：%d\r\n",a);
MPU_Get_Gyroscope(&x,&y,&z);
printf("位置:%d,%d,%d\r\n",x,y,z);
MPU_Get_Accelerometer(&x,&y,&z);
printf("加速度:%d,%d,%d\r\n",x,y,z);
  delay_s(2);
//              res1=MPU_Read_Byte(MPU_PWR_MGMT1_REG);
//              printf("%d\r\n",res1);

}

}
void EUSCIA0_IRQHandler(void)
{

    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE,status);
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        receivedData=MAP_UART_receiveData(EUSCI_A0_BASE);
        if(receivedData=='4')MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
        if(receivedData=='5')MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
    MAP_Interrupt_disableSleepOnIsrExit();//必须加此句，否则会卡死在这里不会返回主函数

}
