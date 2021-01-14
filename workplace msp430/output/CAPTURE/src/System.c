/*
 * System.c
 *
 *  Created on: 2020年10月6日
 *      Author: asus
 */

#include <lib/encoder.h>
#include <lib/pwm.h>
#include <lib/System.h>
#include "driverlib.h"
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "control_task.h"
#include "adc14_multi_channal.h"
#include "delay.h"
#include "key.h"
int smclk=0;
/*************************系统初始化**************************/
void System_init(void)
{
  //      Clock_init();                             //48MHZ DCO
//        smclk=CS_getSMCLK();
        delay_init();
        oled_app();
        KEY_init();
        Uart_init();                              //波特率115200 8N1
        adc14_init();                             //AD初始化
        Pwm_init();                               //PWM初始化
        Encoder_init();                           //P7.1 P7.2两路捕获模式采集上升沿
        set_PID_param();                          //设置PID参数
        Remoter(speed);                             //启动小车
 //       Motor();                                //调试使用，直接输出PWM波
        Timer32_init();                           //定时100ms 采集一次脉冲数
        MAP_Interrupt_enableMaster();             //开启总中断
}
/***********************************************************/


/**************************升频48M**************************/
void Clock_init()
{
        FlashCtl_setWaitState(FLASH_BANK0, 1);
        FlashCtl_setWaitState(FLASH_BANK1, 1);
        MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
        CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
        CS->KEY = CS_KEY_VAL;                        // 解锁时钟寄存器
        CS->CTL1 = CS_CTL1_SELA_2 |                  //设置ACLK=REFO, SMCLK=MCLK=DCO
                CS_CTL1_SELS_3 |
                CS_CTL1_SELM_3;
        CS->KEY = 0;                            // 加锁时钟寄存器
}
/***********************************************************/


/***********32位定时器初始化10ms（近似，需要更改）************/
void Timer32_init(void)
{
    MAP_Timer32_initModule(TIMER32_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
                    TIMER32_PERIODIC_MODE);
        MAP_Timer32_setCount(TIMER32_BASE, 128000*4*5);
        MAP_Interrupt_setPriority(INT_T32_INT1,0x10);
        MAP_Interrupt_enableInterrupt(INT_T32_INT1);
        MAP_Timer32_enableInterrupt(TIMER32_BASE);
        MAP_Timer32_startTimer(TIMER32_BASE,true);
}
/***********************************************************/


/**************串口初始化结构体******************************/
const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        26,                                      // BRDIV =
        0,                                       // UCxBRF =
        0,                                     // UCxBRS =
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};
/***********************************************************/


/***********************串口初始化**************************/
void Uart_init()
{
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,     //设置P1.2 P1.3为串口功能
                   GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);    //初始化串口参数
    MAP_UART_enableModule(EUSCI_A0_BASE);      //开启串口

}
/***********************************************************/



