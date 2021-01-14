#include "msp.h"
#include "driverlib.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "delay.h"
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


/**
 * main.c
 */
void main()
{

    delay_init();
    mpu6050_i2c_init();
    MPU6050_initialize();
//    DMP_Init();
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
//                GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
//    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Enable UART module */

    while(1)
    {
      //  IIC_WriteByte(0x68,0x66,12);

//        printf("Phj: \r\n");
//            Read_DMP();
//            printf("Pitch: %f\r\n",Pitch);
//            printf("Roll: %f\r\n",Roll);
//            delay_ms(20);


    }
}
