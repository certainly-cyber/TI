/*
 * ADS1118.c
 *
 *  Created on: 2020年9月3日
 *      Author: lenovo
 */


#include <ADS1118.h>

static int msb;
static unsigned int temp;
static uint8_t dummy1, dummy2, dummy3, dummy0 = 0;


void initSPI()
{
  // Set CS low to start access to device
//  P3OUT &= ~BIT4;

  //Clock Polarity: The inactive state is high
  //MSB First, 8-bit, Master, 3-pin mode, Synchronous
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 |= 0x20;                          // /2
  UCA0BR1 = 0;                              //
  UCA0MCTL = 0;                             // No modulation must be cleared for SPI
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

//  UCA0IE |= UCRXIE;                          // Enable USCI0 RX interrupt
}



int WriteSPI(unsigned int config, int mode)
{
    volatile unsigned int fuck = 0;
    volatile int a = 0;
    temp = config;
    fuck = temp;

// we always set BIT15 for the data SS conversion in config for now
//  if (mode==1)
//      temp = (temp | 0x8000);     // mode == 1, means to read the data and start a new conversion.

    // Set CS low to start access to device
    P3OUT &= ~BIT5;
//    __delay_cycles(10);

    //或许需要加一个延时?


//change the polarity of UCI0B_CLK to driver ADS1118.
// OK for FR5969 driverlib as is
//    UCB0CTL1 |= UCSWRST;
//    UCB0CTL0 = (UCMSB + UCMST + UCMODE_0 + UCSYNC)>>8;
//    UCB0CTL1 &= ~UCSWRST;

// Xmit MSB
    //USCI_B0 TX buffer ready?
    while (!USCI_A_SPI_getInterruptStatus(USCI_A0_BASE,
                                          USCI_A_SPI_TRANSMIT_INTERRUPT));
    //Transmit MSB to slave
    USCI_A_SPI_transmitData(USCI_A0_BASE,(uint8_t)((temp & 0xFF00) >> 8));
    // wait until done
    while (!USCI_A_SPI_getInterruptStatus(USCI_A0_BASE,
                                          USCI_A_SPI_RECEIVE_INTERRUPT));

    dummy0 =  USCI_A_SPI_receiveData(USCI_A0_BASE);

// Xmit LSB
    //USCI_A0 TX buffer ready?
    while (!USCI_A_SPI_getInterruptStatus(USCI_A0_BASE,
                                          USCI_A_SPI_TRANSMIT_INTERRUPT));
    //Transmit MSB to slave
    USCI_A_SPI_transmitData(USCI_A0_BASE,(uint8_t)(temp & 0x00FF));
    // wait until done
    while (!USCI_A_SPI_getInterruptStatus(USCI_A0_BASE,
                                          USCI_A_SPI_RECEIVE_INTERRUPT));

    dummy1 =  USCI_A_SPI_receiveData(USCI_A0_BASE);

    // Note to self..
    // We repeat the process here again, and throw the result away...
    // it is the return of the original command, so we can use it for a sanity check
    //

    // Xmit MSB
        //USCI_B0 TX buffer ready?
        while (!USCI_A_SPI_getInterruptStatus(USCI_A0_BASE,
                                              USCI_A_SPI_TRANSMIT_INTERRUPT));
        //Transmit MSB to slave
        USCI_A_SPI_transmitData(USCI_A0_BASE,(uint8_t)((temp & 0xFF00) >> 8));
        // wait until done
        while (!USCI_A_SPI_getInterruptStatus(USCI_A0_BASE,
                                              USCI_A_SPI_RECEIVE_INTERRUPT));

        dummy2 =  USCI_A_SPI_receiveData(USCI_A0_BASE);

    // Xmit LSB
        //USCI_A0 TX buffer ready?
        while (!USCI_A_SPI_getInterruptStatus(USCI_A0_BASE,
                                              USCI_A_SPI_TRANSMIT_INTERRUPT));
        //Transmit MSB to slave
        USCI_A_SPI_transmitData(USCI_A0_BASE,(uint8_t)(temp & 0x00FF));
        // wait until done
        while (!USCI_A_SPI_getInterruptStatus(USCI_A0_BASE,
                                              USCI_A_SPI_RECEIVE_INTERRUPT));

        dummy3 =  USCI_A_SPI_receiveData(USCI_A0_BASE);


//    //change back the polarity of UCI0B_CLK for deriving LCD
//    UCB0CTL1 |= UCSWRST;
//    UCB0CTL0 = (UCCKPL + UCMSB + UCMST + UCMODE_0 + UCSYNC)>>8;
//    UCB0CTL1 &= ~UCSWRST;

    // Set CS high to end transaction
    P3OUT |= BIT5;

//     sanity check... we should get our original config back
//    dummy2 |= 0x80; // reset lowest bit..   ASD returns 1 for this bit
//    dummy3 &= 0xFE; // clear high bit..     ASD returns 0 for this bit
    temp = ((dummy2 << 8) | dummy3);
    temp = temp<<1;
    temp |= 0x8000;
//    a = (config == temp);
    if(config == temp)
    {
        msb = (unsigned int)((dummy0<<8) | dummy1);
        P1OUT |= BIT0;
    }
    else
    {
        msb = 0x0; // error on the SPI Tx/Rx
    }
    return msb;
}

void GPIO_INIT()
{
    //CS pin
    P3DIR |= BIT5;
    //test pin
    P1DIR |= BIT0;


    //SPI Pins
    P3SEL |= BIT3 + BIT4;                     // P3.3 TXD
                                              // P3.4 RXD
    P2SEL |= BIT7;                            // P2.7 option select
}
