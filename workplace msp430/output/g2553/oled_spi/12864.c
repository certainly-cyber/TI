//12864.c
#include"MSP430G2553.h"
#include"12864.h"
#include"qumo.h"
#include"stdio.h"
void delay(int ms)//延时
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
   for(jj=0;jj<500;jj++);

}



//向LCD写入地址
void LCD_WrDat(unsigned char dat)
{
   unsigned char i=8;
     DC_SET;
     SCL_CLR;
  while(i--)
  {
    if(dat&0x80)
      {SDA_SET;}
    else
      {SDA_CLR;}
        SCL_SET ;
        SCL_CLR;
        dat<<=1;
  }
}
//向LCD屏中写入命令
void LCD_WrCmd(unsigned char cmd)
{
    unsigned char i=8;
    DC_CLR;
    SCL_CLR;
  while(i--)
  {
    if(cmd&0x80)
    {SDA_SET;}
    else{SDA_CLR;}
    SCL_SET;
    SCL_CLR;
    cmd<<=1;
  }
}
//延时函数
void LCD_DLY_ms(unsigned int ms)
{
  unsigned int a;
  while(ms)
  {
    a=1335;
    while(a--);
    ms--;
  }
  return;
}
//LCD屏清屏
void LCD_CLS(void)
{
    unsigned char y,x;
    for(y=0;y<8;y++)
    {
        LCD_WrCmd(0xb0+y);
        LCD_WrCmd(0x01);
        LCD_WrCmd(0x10);
        for(x=0;x<X_WIDTH;x++)
            LCD_WrDat(0);
    }
}
//清屏操作
void LCD_Fill(unsigned char bmp_dat)
{
    unsigned char y,x;

    for(y=0;y<8;y++)
    {
        LCD_WrCmd(0xb0+y);
        LCD_WrCmd(0x01);
        LCD_WrCmd(0x10);
        for(x=0;x<X_WIDTH;x++)
            LCD_WrDat(bmp_dat);
    }
}

void LCD_Set_Pos(unsigned char x, unsigned char y)//设置坐标
{
  LCD_WrCmd(0xb0+y);
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  LCD_WrCmd((x&0x0f)|0x00);
}
//LCD液晶屏初始化
void LCD_Init(void)
{
        SCL_SET;
        RST_CLR;
    LCD_DLY_ms(50);
        RST_SET;

    //从上电到下面开始初始化要有足够的时间，即等待RC复位完毕

  LCD_WrCmd(0xae);//--turn off oled panel
  LCD_WrCmd(0x00);//---set low column address
  LCD_WrCmd(0x10);//---set high column address
  LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  LCD_WrCmd(0x81);//--set contrast control register
  LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
  LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  LCD_WrCmd(0xa6);//--set normal display
  LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  LCD_WrCmd(0x3f);//--1/64 duty
  LCD_WrCmd(0xd3);//-set display offset Shift Mapping RAM Counter (0x00~0x3F)
  LCD_WrCmd(0x00);//-not offset
  LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  LCD_WrCmd(0xd9);//--set pre-charge period
  LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  LCD_WrCmd(0xda);//--set com pins hardware configuration
  LCD_WrCmd(0x12);
  LCD_WrCmd(0xdb);//--set vcomh
  LCD_WrCmd(0x40);//Set VCOM Deselect Level
  LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  LCD_WrCmd(0x02);//
  LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
  LCD_WrCmd(0x14);//--set(0x10) disable
  LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
  LCD_WrCmd(0xaf);//--turn on oled panel
  LCD_Fill(0x00);  //初始清屏
  LCD_Set_Pos(0,0);
}

//==============================================================
//函数名：LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>126){x=0;y++;}
    LCD_Set_Pos(x,y);
    for(i=0;i<6;i++)
      LCD_WrDat(F6x8[c][i]);
    x+=6;
    j++;
  }
}

//==============================================================
//函数名：LCD_P6x8num(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入number
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================

void LCD_P6x8num(unsigned char x,unsigned char y, unsigned char ch[])
{
    unsigned char c=0,i=0,j=0;
    while (ch[j]!='\0')
    {
        c =ch[j]-32;
        if(x>126){x=0;y++;}             //边界限制  强制拉回
        LCD_Set_Pos(x,y);
        for(i=0;i<6;i++)
        LCD_WrDat(F6x8[c][i]);
        x+=6;
        j++;
    }
}
//==============================================================
//函数名：LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================

void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char *ch)
{
  unsigned char c=0,i=0,j=0;

  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    LCD_Set_Pos(x,y);
    for(i=0;i<8;i++)
      LCD_WrDat(F8X16[c*16+i]);
    LCD_Set_Pos(x,y+1);
    for(i=0;i<8;i++)
      LCD_WrDat(F8X16[c*16+i+8]);
    x+=8;
    j++;
  }
}

void LCD_P16x16Ch(unsigned char x,unsigned char y,unsigned char count)
{
    unsigned char wm=0;
    unsigned int adder=32*count;
    LCD_Set_Pos(x , y);
    for(wm = 0;wm < 16;wm++)
    {
        LCD_WrDat(F14x16[adder]);
        adder += 1;
    }
    LCD_Set_Pos(x,y + 1);
    for(wm = 0;wm < 16;wm++)
    {
        LCD_WrDat(F14x16[adder]);
        adder += 1;
    }
}

void Draw_LibLogo(void)
{
    word ii=0;
    byte x,y;

    for(y=0;y<8;y++)
    {
        LCD_Set_Pos(34,y);
        for(x=34;x<94;x++)
            LCD_WrDat(LIBLOGO60x58[ii++]);

    }
}


void Draw_BMP(byte x0,byte y0,byte x1,byte y1)
{
  word ii=0;
  byte x,y;

  if(y1%8==0) y=y1/8;
  else y=y1/8+1;
    for(y=y0;y<=y1;y++)
    {
        LCD_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
        {
            LCD_WrDat(yangfeng[ii++]);
        }
    }
}
/*****************功能描述：将小数部分拆开并以整数的形式返回，n为小数个数****************************/
    int decimal(float a,int n)
    {
        int b=a;
        int i;
        a=a-b;
        for(i=0;i<n;i++){a=a*10;}
        int c=a;
        return c;
    }
/*****************功能描述：显示小数***************************/
    void decimal_show(unsigned char x,unsigned char y,float a,int m,int n)
    {
    char b[5];
    char b2[6];
    int decimal1=a;
    sprintf(b,"%d",decimal1);
    int decimal2=decimal(a,n);
    sprintf(b2,"%d",decimal2);
    LCD_P8x16Str(x,y,b);
    LCD_P8x16Str(m*8, y, (unsigned char *)".");
    LCD_P8x16Str((m+1)*8, y,b2);
    }



