void main (void)
{
   volatile unsigned int tmp = 0;
   volatile int result2;
   volatile float b = 0;
   WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
   initSPI();
   GPIO_INIT();
   tmp = ADSCON_CH0OP;
   WriteSPI(tmp,1);

   while(1)
   {
       result2 =  WriteSPI(tmp,1);
       result2 = (int)result2;
//     b = (float)result2 / 32768 * 4.096*2;
       b = (float)result2 / 32768 *2.048*2;
   }

}
