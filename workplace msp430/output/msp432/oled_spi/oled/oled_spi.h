/*
 * oled_spi.h
 *
 *  Created on: 2020年10月6日
 *      Author: certainly
 */

#ifndef OLED_OLED_SPI_H_
#define OLED_OLED_SPI_H_

//-----------------OLED端口定义----------------



#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据

#define uint8 unsigned char
#define uint32 unsigned int
#define WHICH_SPI &hspi1//根据使用的SPI更改
void spi_oled_init();
void OLED_ClearPoint(uint8 x,uint8 y);
void OLED_ColorTurn(uint8 i);
void OLED_DisplayTurn(uint8 i );
void OLED_WR_Byte(uint8 dat,uint8 cmd);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8 x,uint8 y);
void OLED_DrawLine(uint8 x1,uint8 y1,uint8 x2,uint8 y2);
void OLED_DrawSquare(uint8 x1,uint8 y1,uint8 x2,uint8 y2);
void OLED_DrawCircle(uint8 x,uint8 y,uint8 r);
void OLED_ShowChar(uint8 x,uint8 y,uint8 chr,uint8 size1);
void OLED_ShowString(uint8 x,uint8 y,uint8 *chr,uint8 size1);
void OLED_ShowNum(uint8 x,uint8 y,uint32 num,uint8 len,uint8 size1);
void OLED_ShowChinese(uint8 x,uint8 y,uint8 num,uint8 size1);
void OLED_ScrollDisplay(uint8 num,uint8 space);
void OLED_WR_BP(uint8 x,uint8 y);
void OLED_ShowPicture(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 BMP[]);
void OLED_Printf(uint8 str[]);
void OLED_Init(void);





#endif /* OLED_OLED_SPI_H_ */
