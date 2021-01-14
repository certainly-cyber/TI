#include <driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "oled_i2c.h"
#include "delay.h"
int aclk,mclk,smclk,hsmclk,bclk=0;
/* Slave Address for I2C Slave */
#define SLAVE_ADDRESS       0x3C


/* I2C Master Configuration Parameter */
// Baud rate selectable (100KBPS, 400KBPS)
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        48000000,                                // SMCLK = 3MHz (default)
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

int main(void)
{
    //* Disabling the Watchdog  */
  //  MAP_WDT_A_holdTimer();
    delay_init();
    /* Select Port 1 for I2C - Set Pin 6, 7 to input Primary Module Function,
     *   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
     */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
            GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initializing I2C Master to SMCLK at 100khz with no autostop */
    MAP_I2C_initMaster(EUSCI_B2_BASE, &i2cConfig);

    /* Specify slave address */
    MAP_I2C_setSlaveAddress(EUSCI_B2_BASE, SLAVE_ADDRESS);

    /* Enable I2C Module to start operations */
    MAP_I2C_enableModule(EUSCI_B2_BASE);

    I2C_OLED_Init();
    OLED_All(0);//«Â∆¡
    OLED_P8x16Str(0,0,(unsigned char*)"Voltage:");
    OLED_P8x16Str(70,0,(unsigned char*)"0.0003");
    OLED_P8x16Str(0,2,(unsigned char*)"Current:");
    OLED_P8x16Str(78,2,(unsigned char*)"0.000");
    OLED_P8x16Str(0,4,(unsigned char*)"Resistor:");
    OLED_P8x16Str(78,4,(unsigned char*)"0.000");
    while (1)
    {

//         Single_Write_OLED(0x01,0x02);
//         aclk = CS_getACLK();
//         mclk = CS_getMCLK();
//         smclk = CS_getSMCLK();
//         hsmclk = CS_getHSMCLK();
//         bclk = CS_getBCLK();


    }
}
