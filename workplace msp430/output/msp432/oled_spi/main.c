#include "driverlib.h"
#include "oled_spi.h"
#include "delay.h"



/**
 * main.c
 */
void main(void)
{
	delay_init();
	spi_oled_init();
	OLED_Init();
	OLED_DisPlay_On();
	while(1)
	{
	    //OLED_WR_Byte(0xAE,OLED_CMD);
	    OLED_Printf("asd");
	}

}
