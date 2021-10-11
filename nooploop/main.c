
/* DriverLib Includes */
#include <driverlib.h>
const uint8_t TEXT_Buffer[]={"USART TEST\r\n"};
#define TEXT_LENTH sizeof(TEXT_Buffer)
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <uart.h>
#include <stdio.h>
#include <string.h>
#include "delay.h"
uint8_t receivedData = 0x00;
uint8_t transmitData = 0x00;
int flag,flag1,flag2=0;
u8 text[256];
float final=0;
int i1=0;
int number,number2=0;
float solve(u8 byte[])
{
int32_t temp = (int32_t)(byte[0] << 8 | byte[1] << 16 | byte[2] << 24) / 256;
float result = temp/1000.0f;
return result;
}
void UartPutchar1(unsigned char c)
{
    MAP_UART_transmitData(EUSCI_A0_BASE,c);
}
void uart_write_multi1(uint8_t *p_buffer, uint8_t length)//写多个数据
{
    uint8_t index = length;
    while(index--)
    {
        MAP_UART_transmitData(EUSCI_A0_BASE,*p_buffer++);
    }
}
unsigned char UartGetchar1()
{
 unsigned char c;
 c=MAP_UART_receiveData(EUSCI_A0_BASE);
 return c;
}
void UartPutchar2(unsigned char c)
{
    MAP_UART_transmitData(EUSCI_A1_BASE,c);
}
void uart_write_multi2(uint8_t *p_buffer, uint8_t length)//写多个数据
{
    uint8_t index = length;
    while(index--)
    {
        MAP_UART_transmitData(EUSCI_A1_BASE,*p_buffer++);
    }
}
unsigned char UartGetchar2()
{
 unsigned char c;
 c=MAP_UART_receiveData(EUSCI_A1_BASE);
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
//USART1 115200
eUSCI_UART_Config uartConfig2 =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        26,                                     // BRDIV = 78
        0,                                       // UCxBRF = 2
        111,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling

};
//USART0 9600
eUSCI_UART_Config uartConfig1 =
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
void LED_init()
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
}
//![Simple UART Config]
int aclk,mclk,smclk,hsmclk,bclk=0;
int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();
    KEY_init();
    LED_init();
    delay_init();
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);



    //![Simple UART Example]
    /* Configuring UART2 Module */
    MAP_UART_initModule(EUSCI_A1_BASE, &uartConfig2);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A1_BASE);
    /* Enabling interrupts */
    MAP_UART_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA1);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster();
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig1);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);
    //![Simple UART Example]
    printf("hi\r\n");
    while(1)
    {

        if(flag==1)
        {


 //       MAP_UART_disableModule(EUSCI_A1_BASE);
        for(i1=0;i1<3;i1++){printf("%x\r\n",text[i1]);}
        final=solve(text);
        printf("结果为:%f\r\n",final);
        flag=0;

//        MAP_UART_enableModule(EUSCI_A1_BASE);

        }


    }
}

/* EUSCI A0 UART ISR - Echoes data back to PC host */
void EUSCIA1_IRQHandler(void)
{

    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A1_BASE);
    MAP_UART_clearInterruptFlag(EUSCI_A1_BASE,status);
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        if(flag==0&&flag1==0&&flag2==0)
                {
            receivedData =MAP_UART_receiveData(EUSCI_A1_BASE);//(USART1->DR);  //读取接收到的数
 //           printf("%x",receivedData);
//            if(receivedData=='1'){MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);}

                if(receivedData==0x57){flag1=1;goto A;}
                }

                if(flag==0&&flag1==1&&flag2==0)
                {receivedData =MAP_UART_receiveData(EUSCI_A1_BASE);

                if(receivedData==0x00){flag2=1;goto A;}
              }
                if(flag==0&&flag1==1&&flag2==1)
                {

                    number=number+1;
                    receivedData=MAP_UART_receiveData(EUSCI_A1_BASE);
                    if(number>6){text[number2]=receivedData;number2=number2+1;}
                    if(number==9){flag=1;flag1=0;flag2=0;number2=0;number=0;}
                }

                A:receivedData=0;

    }
    MAP_Interrupt_disableSleepOnIsrExit();//必须加此句，否则会卡死在这里不会返回主函数

}






