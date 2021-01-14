/*
 * tem_sensor.c
 *
 *  Created on: 2020年10月3日
 *      Author: certainly
 */
#include "tem_sensor.h"
#include "driverlib.h"
int i1=0;
//*****读信息*******************************************
unsigned char DataL,DataH,preceg;
unsigned int memread_wuti(void)
{
    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, 0x00);  // Start + 1Byte
    for(i1=0;i1<1230;i1++){;}
    MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE,0x07); // Poll for TXINT,Send 1Byte
    for(i1=0;i1<1230;i1++){;}
    I2C_masterReceiveStart(EUSCI_B0_BASE);
    for(i1=0;i1<1230;i1++){;}

    DataL =MAP_I2C_masterReceiveMultiByteNext(EUSCI_B0_BASE);
    for(i1=0;i1<1230;i1++){;}
    DataH =MAP_I2C_masterReceiveMultiByteNext(EUSCI_B0_BASE);
    for(i1=0;i1<1230;i1++){;}
    preceg= MAP_I2C_masterReceiveMultiByteNext(
               EUSCI_B0_BASE);
    MAP_I2C_masterReceiveMultiByteStop(EUSCI_B0_BASE);
    for(i1=0;i1<1230;i1++){;}
    preceg= MAP_I2C_masterReceiveMultiByteNext(
            EUSCI_B0_BASE);
    for(i1=0;i1<1230;i1++){;}

    return(DataH*256+DataL);
}
unsigned int memread_huanjing(void)
{

    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, 0x00);  // Start + 1Byte
    for(i1=0;i1<1230;i1++){;}
    MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE,0x06); // Poll for TXINT,Send 1Byte
    for(i1=0;i1<1230;i1++){;}
    I2C_masterReceiveStart(EUSCI_B0_BASE);
    for(i1=0;i1<1230;i1++){;}

    DataL =MAP_I2C_masterReceiveMultiByteNext(EUSCI_B0_BASE);
    for(i1=0;i1<1230;i1++){;}
    DataH =MAP_I2C_masterReceiveMultiByteNext(EUSCI_B0_BASE);
    for(i1=0;i1<1230;i1++){;}
    preceg= MAP_I2C_masterReceiveMultiByteNext(
               EUSCI_B0_BASE);
    MAP_I2C_masterReceiveMultiByteStop(EUSCI_B0_BASE);
    for(i1=0;i1<1230;i1++){;}
    preceg= MAP_I2C_masterReceiveMultiByteNext(
            EUSCI_B0_BASE);
    for(i1=0;i1<1230;i1++){;}

    return(DataH*256+DataL);
}
float transmit(unsigned int a)
{   float b;
   b=0.02*a-273.15;
    return b;
}



