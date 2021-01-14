#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

#define BUFFER_SIZE         2

typedef enum{WRITE, READ}e_transmit;
typedef struct
{
    uint8_t tx_count;
    uint8_t rx_count;
}t_i2c_count;
typedef struct
{
    e_transmit operation;
    uint8_t registerAddress;
    uint8_t tx_buffer[BUFFER_SIZE];
    uint8_t rx_buffer[BUFFER_SIZE];
    t_i2c_count transmit;
}t_i2c;


static t_i2c _device = {WRITE,0,{0},{0},0};

/*
 * P4端口需映射成i2c
 */
static void i2c_port_mapping(void)
{
    __disable_interrupt();                  //disable all interrupts

    PMAPPWD = 0x02D52;                      //enable port mapping register

    P4MAP1 = PM_UCB1SDA;                    //p4.1 --> SDA
    P4MAP2 = PM_UCB1SCL;                    //p4.2 --> SCL

    PMAPPWD = 0;                            //disable port mapping register
}

void i2c_init(uint8_t deviceAddress)
{
    i2c_port_mapping();

    I2C_PORT_DIR |= I2C_SCL_PIN | I2C_SDA_PIN;
    I2C_PORT_OUT |= I2C_SCL_PIN | I2C_SDA_PIN;
    __delay_cycles(1000);
    I2C_PORT_SEL |= I2C_SCL_PIN | I2C_SDA_PIN;
    __delay_cycles(1000);
    UCB1CTL1 |= UCSWRST;                        //enable sw reset
    UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;       //I2C master, synchronous mode
    UCB1CTL1 = UCSSEL_2 + UCSWRST;              //SMCLK, keep sw reset
    UCB1BR0 = 12;                               //~100K
    UCB1BR1 = 0;
    UCB1I2CSA = deviceAddress;                  //mount device address
    UCB1CTL1 &= ~UCSWRST;                       //clear sw reset, resume operation
    UCB1IE |= UCRXIE + UCTXIE;                  //enable tx and rx interrupts
}


void i2c_write_register(uint8_t registerAddress, uint16_t value)
{
    _device.operation = WRITE;
    _device.registerAddress = registerAddress;

    _device.tx_buffer[1] = (value >> 8) & 0xff;     //方便中断中操作，LSB方式存入，中断中MSB方式发送
    _device.tx_buffer[0] = value & 0xff;

    _device.transmit.tx_count = 1 + BUFFER_SIZE;

    while (UCB1CTL1 & UCTXSTP);

    UCB1CTL1 |= UCTR + UCTXSTT;                 //start i2c write operation

    __bis_SR_register(LPM0_bits + GIE);         //Enter LPM0, enable interrupts
    __no_operation();

    //while (UCB0CTL1 & UCTXSTP);                 // Ensure stop condition got sent
}


uint16_t i2c_read_register(uint8_t registerAddress)
{
    _device.operation = READ;
    _device.registerAddress = registerAddress;

    _device.transmit.tx_count = 1;
    _device.transmit.rx_count = BUFFER_SIZE;

    //UCB1CTL1 &= ~UCTR;
    while (UCB1CTL1 & UCTXSTP);

    UCB1CTL1 |= UCTR | UCTXSTT;
      __bis_SR_register(LPM0_bits + GIE);
    __no_operation();

    return (((uint16_t)_device.rx_buffer[0] << 8) | _device.rx_buffer[1]);
}

#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
    switch(__even_in_range(UCB1IV, 12))
    {
    case USCI_I2C_UCRXIFG:
        if(_device.transmit.rx_count)
        {
            _device.rx_buffer[0] = UCB1RXBUF;

            UCB1CTL1 |= UCTXSTP;                        //只接收两个数据，接收一个后需产生停止信号

            _device.transmit.rx_count = 0;              //下一次接收最后的数据
        }
        else
        {
            _device.rx_buffer[1] = UCB1RXBUF;
            __bic_SR_register_on_exit(LPM0_bits);       //退出LPM0_bits
        }
        UCB1IFG &= ~UCRXIFG;
        break;
    case USCI_I2C_UCTXIFG:
        if(_device.operation == READ)
        {
            if(_device.transmit.tx_count)
            {
                UCB1TXBUF = _device.registerAddress;
                _device.transmit.tx_count = 0;
            }
            else
            {
                UCB1CTL1 &= ~UCTR;
                UCB1CTL1 |= UCTXSTT;                    // I2C start condition
            }
        }
        else if(_device.operation == WRITE)
        {
            if(_device.transmit.tx_count)
            {
                _device.transmit.tx_count--;
                if(_device.transmit.tx_count >= BUFFER_SIZE)
                {
                    UCB1TXBUF = _device.registerAddress;
                }
                else
                {
                    UCB1TXBUF = _device.tx_buffer[_device.transmit.tx_count];
                }
            }
            else
            {
                UCB1CTL1 |= UCTXSTP;                    // I2C STOP
                __bic_SR_register_on_exit(LPM0_bits);
            }
        }
        UCB1IFG &= ~UCTXIFG;
        break;
    default:
        break;
    }
}



