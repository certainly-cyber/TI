#include "outputdata.h"
#include "outputdata.h"
#include "driverlib.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
//#include "uart.h"
//#include "arm_math.h"

float OutData[4] = { 0 };
long  int CheckSum_OutData[4] = {0};
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
void usart_init()
{
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                  GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);





          //![Simple UART Example]
          /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    //       /* Enable UART module */
         MAP_UART_enableModule(EUSCI_A0_BASE);
}


unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

void OutPut_Data(void)
{
    int temp[4] = {0};
    unsigned int temp1[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
    for(i=0;i<4;i++)
     {
      
      temp[i]  = (int)OutData[i];

      temp1[i] = (unsigned int)temp[i];
      
     }
     
    for(i=0;i<4;i++) 
    {
      databuf[i*2]   = (unsigned char)(temp1[i]%256);
      databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }
    
    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++){}
  //  uart_putchar(UART1,databuf[i]);
      printf("%d",databuf[i]);

}


void OutPut_CheckSumData(void)
{
	unsigned short CRC_Tmp,i;
        unsigned char databuf[9] = {0};
        int temp[4] = {0};
        unsigned int temp1[4] = {0};

	
        for(i=0;i<4;i++)
        {
            temp[i]  = (int)CheckSum_OutData[i];
            temp1[i] = (unsigned int)temp[i];
        }
        for(i=0;i<4;i++) 
        {
            databuf[i*2]   = (unsigned char)(temp1[i]%256);
            databuf[i*2+1] = (unsigned char)(temp1[i]/256);
        }
        
	CRC_Tmp = 0;
	for(i = 0; i < 8; i++) CRC_Tmp += databuf[i];
	databuf[8] = CRC_Tmp ;
        
        for(i=0;i<9;i++)      printf("%d",databuf[i]);
}
