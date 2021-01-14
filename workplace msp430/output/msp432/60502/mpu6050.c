/*
 * mpu6050.c
 *
 *  Created on: 2020年10月11日
 *      Author: certainly
 */
#include "driverlib.h"
#include "mpu6050.h"
#include "delay.h"
const eUSCI_I2C_MasterConfig i2cConfig1 =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        48000000,                                // SMCLK = 3MHz (default)
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

void mpu6050_i2c_init()
{
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
            GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initializing I2C Master to SMCLK at 100khz with no autostop */
    MAP_I2C_initMaster(EUSCI_B2_BASE, &i2cConfig1);

    /* Specify slave address */
    MAP_I2C_setSlaveAddress(EUSCI_B2_BASE, dev_addr);

    /* Enable I2C Module to start operations */
    MAP_I2C_enableModule(EUSCI_B2_BASE);
}
unsigned char I2C_Read(unsigned char address)
{
    unsigned char r_value;
    int i1=0;
    int i2=0;
    MAP_I2C_masterSendMultiByteStart(EUSCI_B2_BASE,dev_addr);  // Start + 1Byte
    delay_us(3);
 //   for(i2=0;i2<1230;i2++){;}
    MAP_I2C_masterSendMultiByteNext(EUSCI_B2_BASE,address); //连续转换
    delay_us(3);
 //   for(i2=0;i2<1230;i2++){;}
    I2C_masterReceiveStart(EUSCI_B2_BASE);
    delay_us(3);
 //   for(i2=0;i2<1230;i2++){;}
    MAP_I2C_masterReceiveMultiByteStop(EUSCI_B2_BASE);
    delay_us(3);
 //   for(i2=0;i2<1230;i2++){;}
    r_value=MAP_I2C_masterReceiveMultiByteNext(EUSCI_B2_BASE);
    delay_us(3);
 //   for(i2=0;i2<1230;i2++){;}
    return r_value;
}
void I2C_Write(unsigned char address,unsigned char data)
{
    int i2=0;
    MAP_I2C_masterSendMultiByteStart(EUSCI_B2_BASE,dev_addr);  // Start + 1Byte
    delay_us(3);
 //   for(i2=0;i2<1230;i2++){;}
    MAP_I2C_masterSendMultiByteNext(EUSCI_B2_BASE,address); //连续转换
    delay_us(3);
 //   for(i2=0;i2<1230;i2++){;}
    MAP_I2C_masterSendMultiByteNext(EUSCI_B2_BASE,data);
    delay_us(3);
 //   for(i2=0;i2<1230;i2++){;}
    I2C_masterSendMultiByteStop(EUSCI_B2_BASE);

}




