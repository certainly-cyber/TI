  #include "driverlib.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
//! MCLK = SMCLK = default DCO=1M; ACLK = REFO~32kHz;
uint8_t receivedData = 0x00;
uint8_t transmitData = 0x00;
float a=0.223;
void GPIO_init();
void USART_init();
void TIME_init();
void main (void)
{
    //Stop WDT
    WDT_A_hold(WDT_A_BASE);
    TIME_init();
    GPIO_init();
    USART_init();
    __enable_interrupt();
    while(1){}
  //  __bis_SR_register(LPM4_bits + GIE);




}

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
    param.clockPrescalar = 109;
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
void TIME_init()
{

    //REFO Clock Sources ACLK
    UCS_initClockSignal(
        UCS_ACLK,
        UCS_REFOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );

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

#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void)
{
    unsigned int temp1;
    int i1;
    for(i1=0;i1<12000;i1++);//延时消抖
    if((P1IN&0xff)!=0xff){
     temp1=P1IFG;
     P1IFG=0x00;
     if(temp1==0x02){UartPutchar('2');}//P1.1被按下
    }

}
//******************************************************************************
//
//This is the PORT2_VECTOR interrupt vector service routine
//
//******************************************************************************
#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void)
{
    unsigned int temp2;
    int i2;
    for(i2=0;i2<12000;i2++);//延时消抖
    if((P2IN&0xff)!=0xff)
    {temp2=P2IFG;
     P2IFG=0x00;
     if(temp2==0x02){ UartPutchar('1');}//P2.1被按下
    }

}
//  Echo back RXed character
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR (void)
{   unsigned int counter=0;

    switch (__even_in_range(UCA0IV,4)){

        //Vector 2 - RXIFG
        case 2:
            receivedData = USCI_A_UART_receiveData(USCI_A0_BASE);
            if(receivedData=='1'){GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);}
            if(receivedData=='2'){GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);}
//            USCI_A_UART_transmitData(USCI_A0_BASE,receivedData);
//            printf("Hello world %d!\r\n", counter);
//            printf("Hello world %f!\r\n", a);

            break;
        default: break;
    }
}
