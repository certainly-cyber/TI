
    #include <msp430.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdint.h>
    int fputc(int _c, register FILE *_fp);
    int fputs(const char *_ptr, register FILE *_fp);
    void UartPutchar(unsigned char c);
    unsigned char UartGetchar();
    void uart_init();
    int mode=0;
    int main(void)
    {
      WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
      if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
      {
        while(1);                               // do not load, trap CPU!!
      }
      DCOCTL = 0;                               // Select lowest DCOx and MODx settings
      BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
      DCOCTL = CALDCO_1MHZ;
      P1DIR|=BIT6;
      P1OUT &= ~BIT6;
      P1IE |=  BIT3;                            // P1.3 interrupt enabled
      P1IES |= BIT3;                            // P1.3 Hi/lo edge
      P1REN |= BIT3;                            // Enable Pull Up on SW2 (P1.3)
      P1OUT |= BIT3;                            //这里使得配置生效
      P1IFG &= ~BIT3;                           // P1.3 IFG cleared                                             //BIT3 on Port 1 can be used as Switch2
      uart_init();
      __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
    }
    //  Echo back RXed character, confirm TX buffer is ready first
    #pragma vector=USCIAB0RX_VECTOR
    __interrupt void USCI0RX_ISR(void)
    {  unsigned int counter=0;
      while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
      UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
       while (!(IFG2&UCA0TXIFG));
       printf("Hello world %d!\r\n", counter);
      if(UCA0RXBUF=='7')
       P1OUT|=BIT6;
      if(UCA0RXBUF=='6')
       P1OUT&= ~BIT6;

    }
    // Port 1 interrupt service routine
    #pragma vector=PORT1_VECTOR
    __interrupt void Port_1(void)
    {
         unsigned int i;
         static int key_up=1;//按键按松开标志
             if(mode)key_up=1;  //支持连按
             if(key_up&&P1IFG&&BIT3)
             {
                 for(i=0;i<10;i++){};
                 key_up=0;
                 if(P1IFG&BIT3)
                      {
                     UartPutchar('8');
                          P1IFG &= ~BIT3;
                      }
             }
             else {P1IFG &= ~BIT3;key_up=1; };


         return;

}



    void UartPutchar(unsigned char c)
    {
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF=c;
    IFG2 &=~UCA0RXIFG;
    }
    void uart_write_multi(uint8_t *p_buffer, uint8_t length)//写多个数据
    {
        uint8_t index = length;
        while(index--)
        {
            while (!(IFG2 & UCA0TXIFG));             // USCI_A1 TX buffer ready?
            UCA0TXBUF = *p_buffer++;                       // TX
            IFG2 &=~UCA0RXIFG;
        }
    }
    unsigned char UartGetchar()
    {
     unsigned char c;
     while(!(IFG2 & UCA0RXIFG));
     c=UCA0RXBUF;
     IFG2 &=~UCA0TXIFG;
     return c;
    }

    int fputc(int _c, register FILE *_fp)
    {
      while(!(IFG2 & UCA0TXIFG));
      UCA0TXBUF = (unsigned char) _c;

      return((unsigned char)_c);
    }

    int fputs(const char *_ptr, register FILE *_fp)
    {
      unsigned int i, len;

      len = strlen(_ptr);

      for(i=0 ; i<len ; i++)
      {
          while(!(IFG2 & UCA0TXIFG));
        UCA0TXBUF = (unsigned char) _ptr[i];
      }

      return len;
    }
    void uart_init()
    {
    DCOCTL = CALDCO_1MHZ;
    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 104;                            // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
    }
