#include "delay.h"
#include <systick.h>
#include "driverlib.h"

static uint8_t  fac_us=0;//us延时倍乘数 
static uint16_t fac_ms=0;//ms延时倍乘数
extern uint32_t SystemCoreClock;
void delay_init(void)
{

    SysTick_disableInterrupt();   //禁用时钟中断

    WDT_A_holdTimer();
    fac_us=48;  //时钟为48Mhz，
    fac_ms=(uint16_t)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

} 
void delay_ms(uint16_t nms)
{
 uint32_t temp;
    SysTick->LOAD=nms*fac_ms; //时间加载
    SysTick->VAL=0x00;        //清空计数器
    SysTick_enableModule();
    do
    {
        temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
  SysTick_disableModule();
    SysTick->VAL =0X00;       //清空计数器
}

void delay_us(uint32_t nus)
{
  uint32_t temp;
    SysTick->LOAD=nus*fac_us; //时间加载
    SysTick->VAL=0x00;        //清空计数器
    SysTick_enableModule();
    do
    {
        temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
  SysTick_disableModule();
    SysTick->VAL =0X00;       //清空计数器
}
void delay_s(uint32_t s)
{
    int time_i=0;
    int time_real=s*1000;
    for(time_i=0;time_i<time_real;time_i++)
    {
        delay_ms(1);
    }
}






