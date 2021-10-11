
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
int sign=0;

unsigned char Re_buf[11],temp_buf[11],counter=0;
unsigned char t,he;
float a[3],w[3],angle[3],T;
static unsigned char Temp[11];
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
//115200版本
eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        26,                                     // BRDIV = 78
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
 //   LED_init();
    delay_init();
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);




    //![Simple UART Example]
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A1_BASE, &uartConfig);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A1_BASE);
    //![Simple UART Example]
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);
    /* Enabling interrupts */
    MAP_UART_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA1);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster();

    //![Simple UART Example]

    while(1)
    {
        if(sign)
          {
             memcpy(Temp,Re_buf,11);
             sign=0;
             if(Re_buf[0]==0x55)       //检查帧头
             {
                switch(Re_buf[1])
                {
                   case 0x51: //标识这个包是加速度包
                      a[0] = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16;      //X轴加速度
                      a[1] = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16;      //Y轴加速度
                      a[2] = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16;      //Z轴加速度
                      T    = ((short)(Temp[9]<<8 | Temp[8]))/340.0+36.25;      //温度
                      break;
                   case 0x52: //标识这个包是角速度包
                      w[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X轴角速度
                      w[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y轴角速度
                      w[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z轴角速度
                      T    = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;      //温度
                      break;
                   case 0x53: //标识这个包是角度包
                      angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X轴滚转角（x 轴）
                      angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
                      angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z轴偏航角（z 轴）
                      T        = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;   //温度

                      //printf("X轴角度：%.2f   Y轴角度：%.2f   Z轴角度：%.2f\r\n",angle[0],angle[1],angle[2]);
                      break;
                   default:  break;
                }
                printf("X角度：%.2f  Y角度：%.2f  Z角度：%.2f  X速度：%.2f  Y速度：%.2f  Z速度：%.2f\r\n",angle[0],angle[1],angle[2],w[0],w[1],w[2]);
             }

          }
          delay_ms(50);


//        uart_write_multi((uint8_t *)TEXT_Buffer,TEXT_LENTH);
//        delay_s(5);
//        MAP_PCM_gotoLPM0();
    }
}

/* EUSCI A0 UART ISR - Echoes data back to PC host */
void EUSCIA1_IRQHandler(void)
{

    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A1_BASE);
    MAP_UART_clearInterruptFlag(EUSCI_A1_BASE,status);
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {



         Temp[counter] = MAP_UART_receiveData(EUSCI_A1_BASE);   //接收数据
         //网购给的程序
         //if(counter == 0 && Re_buf[0] != 0x55) return;      //第 0 号数据不是帧头，跳过
         if(counter == 0 && Temp[0] != 0x55) return;      //第 0 号数据不是帧头，跳过
         counter++;
         if(counter==11) //接收到 11 个数据
         {
            memcpy(Re_buf,Temp,11);
            counter=0; //重新赋值，准备下一帧数据的接收
            sign=1;
         }

//        receivedData=MAP_UART_receiveData(EUSCI_A1_BASE);
//        if(receivedData=='4')MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
//        if(receivedData=='5')MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
    MAP_Interrupt_disableSleepOnIsrExit();//必须加此句，否则会卡死在这里不会返回主函数

}
void PORT1_IRQHandler(void)
{
    uint32_t status;

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
        UartPutchar('2');

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
              UartPutchar('3');
          }

    }


}




