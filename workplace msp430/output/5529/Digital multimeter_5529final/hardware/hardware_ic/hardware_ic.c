/*
 * hardware_ic.c
 *
 *  Created on: 2020年8月11日
 *      Author: DELL
 */

#include <msp430.h>
#include <stdint.h>
#include "hardware_ic.h"

//*
// * @fn:     void initI2C(void)
// * @brief:  初始化i2c bus
// * @para:   none
// * @return: none
// * @comment:none
// */
void initI2C(void)
{
    /*设置IO复用*/
    P3SEL |= BIT0 | BIT1;
    /*禁能USCI模块*/
    UCB0CTL1 |= UCSWRST;
    /*I2C Master*/
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;
    /*clock from SMCLK*/
    UCB0CTL1 = UCSWRST + UCSSEL_2;
    /*set tx speed*/
    UCB0BR0 = 10;
    UCB0BR1 = 0;
    /*设置slave 地址*/
    UCB0I2CSA = ADS_ADDR;
    /*使能I2C*/
    UCB0CTL1 &= ~UCSWRST;
}
/*
 * @fn: uint8_t i2cSend(uint8_t slaveaddr,uint8_t *ptr,uint8_t byteno)
 * @brief:  none
 * @para:   none
 * @return: none
 * @comment:none
 */
unsigned char i2cSend(unsigned char slaveaddr,unsigned char *ptr,unsigned char byteno)
{
    unsigned int timeout = 50000;

    do{
        timeout --;//在规定的超时时间内等待总线空闲
    }while((UCB0STAT & UCBBUSY) & (timeout != 0));

    if(timeout == 0){//超时返回
        while(1);
    }
    UCB0I2CSA = slaveaddr;//设置slave地址

    UCB0CTL1 |= UCTR;//transmitter模式

    UCB0CTL1 |= UCTXSTT;//发送start

    do{//连续发送数据
        while(!(UCB0IFG & UCTXIFG));//上一Byte是否发送完成？
        UCB0TXBUF = *ptr;
        ptr ++;
    }while(--byteno);
    while(!(UCB0IFG & UCTXIFG));//等待最后一个Byte发送完成

    UCB0CTL1 |= UCTXSTP;//发送stop

    __delay_cycles(500);//适当延时，可有可无

    return 0;
}

/*
 * @fn: uint8_t i2cRead(uint8_t slaveaddr,uint8_t *tx_ptr,uint8_t *rx_ptr,uint8_t tx_no,uint8_t rx_no)
 * @brief:  利用I2C Bus读取数据
 * @para:   slaveaddr：从机地址
 *          tx_ptr：读之前需要发送的命令，例如被读从机的寄存器地址
 *          rx_ptr：读出数据缓冲区
 *          tx_no：发送命令的长度
 *          rx_no：接收数据的长度
 * @return: 读成功返回0，否则不为0
 * @comment:none
 */
unsigned char i2cRead(unsigned char slaveaddr,unsigned char *tx_ptr,unsigned char *rx_ptr,unsigned char tx_no,unsigned char rx_no)
{
    unsigned int timeout = 50000;

    do{
        timeout --;//在规定的超时时间内等待总线空闲
    }while((UCB0STAT & UCBBUSY) & (timeout != 0));

    if(timeout == 0){//超时返回
        return 1;
    }

    UCB0I2CSA = slaveaddr;//设置slave地址

    UCB0CTL1 |= UCTR | UCTXSTT;//transmitter模式，发送start
    do{//连续发送数据
        while(!(UCB0IFG & UCTXIFG));//上一Byte是否发送完成？
        UCB0TXBUF = *tx_ptr;
        tx_ptr ++;
    }while(--tx_no);
    while(!(UCB0IFG & UCTXIFG));//等待最后一个Byte发送完成

    __delay_cycles(10);

    UCB0CTL1 &= ~UCTR;//receiver模式

    UCB0CTL1 |= UCTXSTT;//发送start

    do{//连续接收数据
        if(rx_no == 1){//如果是最后一个数据，要在此处发送STOP，NACK也会自动发送
            while(UCB0CTL1 & UCTXSTT);
            UCB0CTL1 |= UCTXSTP;
        }
        while(!(UCB0IFG & UCRXIFG));//完成一次接收？
        *rx_ptr = UCB0RXBUF;
        rx_ptr ++;
    }while(--rx_no);

    __delay_cycles(1000);//适当延时，可有可无
    return 0;
}


