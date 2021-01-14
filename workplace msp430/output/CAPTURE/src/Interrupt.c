/*
 * Interrupt.c
 *
 *  Created on: 2020年10月6日
 *      Author: asus
 */

#include <lib/control_task.h>
#include <lib/encoder.h>
#include <lib/pid.h>
#include <lib/printf.h>
#include <lib/pwm.h>
#include "driverlib.h"
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include <printf.h>
float left =10000,
      right=10000;

/***********************Timer32中断***********************/
void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_BASE);
    Control_Task();
    MAP_Timer32_setCount(TIMER32_BASE,128000*4*5);  //50ms
}

/* EUSCI A0 UART ISR - Echoes data back to PC host */
void EUSCIA0_IRQHandler(void)
{

}
