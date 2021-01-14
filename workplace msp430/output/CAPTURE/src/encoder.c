#include <lib/printf.h>
#include "driverlib.h"
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>

/************************编码器初始化*****************************/
void Encoder_init(void)
{
      TA0CTL=TASSEL_0+MC_2;
      TA1CTL=TASSEL_0+MC_2;
      P7DIR&=~BIT1;
      P7SEL1|=BIT1;
      P7DIR&=~BIT2;
      P7SEL1|=BIT2;
//      MAP_Interrupt_enableSleepOnIsrExit();
//      MAP_Interrupt_enableMaster();
}
/***********************************************************/


/**********************编码器读数***************************/
int read_number(int Wheel)//0代表P7.1，1代表P7.2
{
    int all_number=0;;
    if(Wheel==0){all_number=TA0R;TA0CTL|=TACLR;}
    if(Wheel==1){all_number=TA1R;TA1CTL|=TACLR;}
    return all_number;
}
/**********************************************************/




