/*
 * adc14.c
 *
 *  Created on: 2020年10月10日
 *      Author: DELL
 */
#include "msp.h"
#include "driverlib.h"
#include "adc14_multi_channal.h"
#include <stdint.h>
#include <string.h>
#include "control_task.h"

static uint16_t resultsBuffer[5];             //用来存转换的原始值
float result[5];                              //原来存转换的真实电压

void adc14_init(void)
{
    /* Zero-filling buffer   将数组清零*/
    memset(resultsBuffer, 0x00, 8 * sizeof(uint16_t));


    /* Initializing ADC (MCLK/1/1) 初始化ADC时钟，全一分频*/
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,0);

    /* Configuring GPIOs for Analog In  初始化GPIO口                                                  */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
                   GPIO_PIN5 | GPIO_PIN4 | GPIO_PIN2 | GPIO_PIN1
                    | GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);


    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM4 (A0 - A4)  repeat)
     * 将转换好的值放在ADC_MEM0 - ADC_MEM4里，多通道重复转换模式*/
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM4, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,       //此处改基准电压，现在为3.3V
                                        ADC_INPUT_A0, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM1,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A1, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM2,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A3, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM3,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A4, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM4,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A5, false);

    /* Enabling the interrupt when a conversion on channel 4 (end of sequence)
     *  is complete and enabling conversions */
    MAP_ADC14_enableInterrupt(ADC_INT4);                        //ADC使能中断

    /* Enabling Interrupts */
//    MAP_Interrupt_enableInterrupt(INT_ADC14);
//    MAP_Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();

}




//函数获取5路电压信息
//*result为float型数组
void getvol (float *result)
{
    uint16_t results_Buffer[5];
    uint64_t status;
    uint8_t i;
    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);
    if(status & ADC_INT4)
    {
        MAP_ADC14_getMultiSequenceResult(results_Buffer);
        for(i=0;i<5;i++)
        {
        result[i] = (float)results_Buffer[i]*0.00020141603;            //测白色区域电压在0.1几V，测黑色区域，电压在2V左右
        }                                                              // P5.5的值放在数组0中，P5.4的值放在数组1中，以此类推
    }
}


/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM7. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
//
//void ADC14_IRQHandler(void)                        //ADC中断，在这里读转换的值
//{
//    uint64_t status;
//    uint8_t i;
//
//    status = MAP_ADC14_getEnabledInterruptStatus();
//    MAP_ADC14_clearInterruptFlag(status);
//
//    if(status & ADC_INT4)
//    {
//        MAP_ADC14_getMultiSequenceResult(resultsBuffer);
//        for(i=0;i<5;i++)
//        {
//        result[i] = (float)resultsBuffer[i]*0.00020141603;
//        }
//    }
//
//}
