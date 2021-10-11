#include "delay.h"
#include <systick.h>
#include "driverlib.h"

static uint8_t  fac_us=0;//us��ʱ������ 
static uint16_t fac_ms=0;//ms��ʱ������
extern uint32_t SystemCoreClock;
void delay_init(void)
{

    SysTick_disableInterrupt();   //����ʱ���ж�

    WDT_A_holdTimer();
	fac_us=48;	//ʱ��Ϊ48Mhz��
	fac_ms=(uint16_t)fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   
	FlashCtl_setWaitState(FLASH_BANK0, 1);
	FlashCtl_setWaitState(FLASH_BANK1, 1);
	MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
	CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
	
} 
void delay_ms(uint16_t nms)
{
 uint32_t temp;	    	 
	SysTick->LOAD=nms*fac_ms; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick_enableModule();
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
  SysTick_disableModule();
	SysTick->VAL =0X00;       //��ռ�����	 
}

void delay_us(uint32_t nus)
{
  uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick_enableModule();
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
  SysTick_disableModule();
	SysTick->VAL =0X00;       //��ռ�����	 
}







