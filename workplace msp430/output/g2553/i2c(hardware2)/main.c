#include "msp430g2553.h"



unsigned char RX_Data;

void I2C_Init(unsigned char SA);//I2C初始化，SA为从器件地址

void I2C_ReadData(unsigned char address);//I2C读出某一地址的数据

unsigned char I2C_WriteData(unsigned char address,unsigned char data);//I2C向某一地址写入数据

void delay(void);


void I2C_Init(unsigned char SA)

{

    P1SEL |= (BIT6 + BIT7);

    P1SEL2|= (BIT6 + BIT7);

    UCB0CTL1 |= UCSWRST;                      // Enable SW reset

    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode

    UCB0CTL1 |= UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset

    UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz

    UCB0BR1 = 0;

    UCB0CTL0 &= ~UCSLA10; // 7位地址模式

    UCB0I2CSA = SA;                         // Slave Address is 2ch

    UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation

    delay();
}
unsigned char I2C_WriteData(unsigned char address,unsigned char data)

{

    while( UCB0CTL1& UCTXSTP );

    UCB0CTL1 |= UCTR;                // 写模式

    UCB0CTL1 |= UCTXSTT;             // 发送启动位


    UCB0TXBUF = address;           // 发送字节地址

        // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可

    while((IFG2 & UCB0TXIFG)==0)

    {

        if( UCB0STAT& UCNACKIFG )      // 若无应答 UCNACKIFG=1

        {

            return 1;

        }

    }


    UCB0TXBUF = data;          // 发送字节内容

    while((IFG2 & UCB0TXIFG)==0);     // 等待UCTXIFG=1


    UCB0CTL1 |= UCTXSTP;

    while(UCB0CTL1& UCTXSTP);       // 等待发送完成


    return 0;

}
unsigned char I2C_WriteNData( unsigned char address, unsigned char *pWbuf, unsigned char len)

{

unsigned char i;

while( UCB0CTL1& UCTXSTP );

UCB0CTL1 |= UCTR;                // 写模式

UCB0CTL1 |= UCTXSTT;             // 发送启动位


UCB0TXBUF = address;           // 发送字节地址

// 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可

while((IFG2 & UCB0TXIFG)==0)

{

    if( UCB0STAT& UCNACKIFG )      // 若无应答 UCNACKIFG=1

    {

         return 1;

    }

}


for( i= 0; i < len; i++)

{

    UCB0TXBUF = *pWbuf++;      // 发送寄存器内容

    while(UCB0CTL1& UCTXSTP);   // 等待UCTXIFG=1

 }


UCB0CTL1 |= UCTXSTP;

while(UCB0CTL1& UCTXSTP);       // 等待发送完成


return 0;

}
void I2C_ReadData(unsigned char address)

{

    UCB0CTL1 |= UCTR ;

    UCB0CTL1 |= UCTXSTT;             // I2C TX, start condition

    UCB0TXBUF = address;////eeprom low addr

    while((IFG2 & UCB0TXIFG)==0);

    UCB0CTL1 &= ~UCTR;             // I2C RX,

    UCB0CTL1 |= UCTXSTT;

    while(UCB0CTL1&UCTXSTT);

    UCB0CTL1 |= UCTXSTP;

    while((IFG2&UCB0RXIFG)==0);

    RX_Data = UCB0RXBUF;

    while(UCB0CTL1 & UCTXSTP);

}
unsigned char  I2C_ReadNData(unsigned char address, unsigned char *pRead, unsigned char len )

{

unsigned char i;

while( UCB0CTL1& UCTXSTP );

UCB0CTL1 |= UCTR; // 写模式

UCB0CTL1 |= UCTXSTT; // 发送启动位和写控制字节

UCB0TXBUF = address; // 发送字节地址

// 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可

while((IFG2 & UCB0TXIFG)==0);

UCB0CTL1 &= ~UCTR; // 读模式

UCB0CTL1 |= UCTXSTT; // 发送启动位和读控制字节

while(UCB0CTL1& UCTXSTT); // 等待UCTXSTT=0

// 若无应答 UCNACKIFG = 1

for( i= 0; i< len -1 ; i++)

{

    while((IFG2&UCB0RXIFG)==0);  // 读取字节内容，不包括最后一个字节内容

    *pRead++= UCB0RXBUF;

 }

UCB0CTL1 |= UCTXSTP; // 在接收最后一个字节之前发送停止位

while((IFG2&UCB0RXIFG)==0); // 读取最后一个字节内容

*pRead = UCB0RXBUF;

while( UCB0CTL1& UCTXSTP );

return 0;

}

void delay(void)

{

    unsigned int i,n;

    for(i=0;i<100;i++)

        for(n=0;n<0xff;n++);

}


int main( void )

{

     unsigned char ReadBuf[10];

     unsigned char temp = 1;

     // Stop watchdog timer to prevent time out reset

     WDTCTL = WDTPW + WDTHOLD;

     Uart_Init();

     P1DIR |= BIT0;

     P1OUT |= BIT0;

     I2C_Init(0x51);//1010 000X>>1


     //I2C_WriteData(0x02,5);

     _EINT();

     //I2C_Init(0x51);//1010 000X>>1

     //I2C_ReadData(tUart.RecBuf[0]-0x30);

     while(1)

     {

          if(tUart.RecFlag)

          {

                tUart.RecFlag = 0;

                tUart.Len = 0;


                //I2C_Init(0x51);//1010 000X>>1

                //I2C_ReadData(tUart.RecBuf[0]-0x30);
                I2C_WriteData(0x03,temp++);

                I2C_ReadNData(0x02, ReadBuf, 8 );

                UartSendStr(ReadBuf);

                //UartSendByte(RX_Data);

          }

      }

}

