#include "msp.h"
#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "oled_i2c.h"
#include "delay.h"
int a=9;
int flag1,flag2,flag3,flag4=0;
/**
 * main.c
 */
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

void main(void)
{
    delay_init();
    KEY_init();
    oled_i2c_init();
    I2C_OLED_Init();
    OLED_All(0);//清屏
    OLED_P8x16Str(0,0,(unsigned char*)"Time:");
     MAP_Interrupt_enableMaster();
    while(1){ decimal_show(70,0,a,0);}

}
void PORT1_IRQHandler(void)
{
    uint32_t status;
    int i1=0;
    int i2=0;
    int i3=0;
    int i4=0;


    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    if(status & GPIO_PIN1)
   {
        for(i3=0;i3<123;i3++){}
        if(status & GPIO_PIN1)
        {
        while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0)
        {
            i1=i1+1;
            delay_ms(1);
            if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0&&i1==1000){flag3=1; a=a+10;}//功能1



        }
        if(flag3==0){a=a+1;}//功能2
        flag3=0;
        i2=0;
        i1=0;

    }
    }
    if(status & GPIO_PIN4)
    {
        for(i3=0;i3<123;i3++){}
        if(status & GPIO_PIN4)
              {
              while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0)
              {
                  i4=i4+1;
                  delay_ms(1);
                  if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0&&i4==1000){flag4=1; a=a-10;}//功能3



              }
              if(flag4==0){a=a-1;}//功能4
              flag4=0;
              i4=0;
              i1=0;

          }

    }


}
