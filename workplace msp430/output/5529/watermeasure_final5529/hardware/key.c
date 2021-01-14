/*
 * key.c
 *
 *  Created on: 2020年8月12日
 *      Author: certainly
 */
# include"driverlib.h"
# include"key.h"
int flag1=0;
int flag2=0;
int flag3=0;
void KEY_init()
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2,GPIO_PIN1);
    GPIO_setAsOutputPin( GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);

}
int keyscan()
{
flag1=0;
flag2=0;
flag3=0;
int i2=0;
int i1=0;
int i3=0;
if(!(P2IN&0x02))
{
 for(i3=0;i3<120;i3++);//延时消抖
 if(P2IN&0x02){return 0;}
while(!(P2IN&0x02))
{
i1=i1+1;
if(i1==1000){i1=0;i2=i2+1;}
if(i2==110){flag3=1;}
}
if(flag3==0){flag1=1;}


}
if(!(P1IN&0x02))
{
    for(i3=0;i3<120;i3++);//延时消抖
    if(P1IN&0x02){return 0;}
    while(!(P1IN&0x02));
    flag2=1;
}
    if(flag3){return 1;}
    if(flag1){return 2;}
    if(flag2){return 3;}
    else
        return 4;
}


