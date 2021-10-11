/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 * MSP432 Flash Controller - Programming Calibration Data
 *
 * Description: This example shows the use of the the Flash Controller APIs
 * to erase and program simulated calibration data to a specific area in memory.
 * Data in this example is programmed to user area of memory. The "fake"
 * calibration data is stored in a RAM array and set using the memset function,
 * however in a real application this buffer would be filled out using a serial
 * interface such as I2C.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 ******************************************************************************/
/* DriverLib Includes */
#include <driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <uart.h>
#include <stdio.h>
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
uint8_t receivedData = 0x00;
uint8_t transmitData = 0x00;
#define CALIBRATION_START 0x0003F000
unsigned long *Ptr1=(unsigned long*)0x0003F000;
/* Statics */
int i=0;
uint8_t simulatedCalibrationData[4096]={0};
uint8_t a[32]={0};
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
void KEY_init()
{
     MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
     MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
     MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
     MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
     MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
     MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
     MAP_Interrupt_enableInterrupt(INT_PORT1);

}
int main(void)
{
    /* Since this program has a huge buffer that simulates the calibration data,
     * halting the watch dog is done in the reset ISR to avoid a watchdog
     * timeout during the zero
     */

    /* Setting our MCLK to 48MHz for faster programming */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

//    /* Initializing our buffer to a pattern of 0xA5 */
    KEY_init();
    /* Selecting P1.2 and P1.3 in UART mode */
      MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
              GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);





      //![Simple UART Example]
      /* Configuring UART Module */
      MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

      /* Enable UART module */
      MAP_UART_enableModule(EUSCI_A0_BASE);
      /* Enabling interrupts */
      MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
      MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
      MAP_Interrupt_enableSleepOnIsrExit();
      MAP_Interrupt_enableMaster();

//开始读数据

    /* Going to LPM3 when not in use */
    while (1)
    {
        MAP_PCM_gotoLPM3();
    }
}

void PORT1_IRQHandler(void)
{
    uint32_t status;
    char b[5];
    int i3;
    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    if(status & GPIO_PIN1)
   {
        for(i3=0;i3<123;i3++){}
        if(status & GPIO_PIN1)
        {
        while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0)
        {

        }
        memset(simulatedCalibrationData, 9, 4096);
        //
        //    //![FlashCtl Program]
        //    /* Unprotecting Info Bank 0, Sector 0  */
        MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR31);
        //
        //    /* Trying to erase the sector. Within this function, the API will
        //        automatically try to erase the maximum number of tries. If it fails,
        //         trap in an infinite loop */
        if(!MAP_FlashCtl_eraseSector(CALIBRATION_START))
            while(1);

        //    /* Trying to program the memory. Within this function, the API will        automatically try to program the maximum number of tries. If it fails,
        //        trap inside an infinite loop */
        if(!MAP_FlashCtl_programMemory(simulatedCalibrationData,
                (void*) CALIBRATION_START, 4096))
                    while(1);

        /* Setting the sector back to protected  */
        MAP_FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR31);

    }
    }
    if(status & GPIO_PIN4)
    {
        for(i3=0;i3<123;i3++){}
        if(status & GPIO_PIN4)
              {
              while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0)
              {



              }
              for(i=0;i<32;i++){
                                 a[i]=Ptr1[i];
                              }
              printf("%d\r\n",a[0]);
//              sprintf(b,"%d",a[0]);
//              UartPutchar(b[0]);

          }

    }


}
/* EUSCI A0 UART ISR - Echoes data back to PC host */
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
