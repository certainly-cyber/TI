/*
 * pwm.c
 *
 *  Created on: 2020Äê10ÔÂ2ÈÕ
 *  Author: asus
 *      TA0.1-4 output PWM
 */
/* DriverLib Includes */
#include "driverlib.h"
#include "msp.h"
#include <stdint.h>

#define TIMER_PERIOD 2000
const Timer_A_UpDownModeConfig upDownConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock SOurce
        TIMER_A_CLOCKSOURCE_DIVIDER_1,          // SMCLK/1 = 48MHz
        TIMER_PERIOD,                           // 2000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,    // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};
const Timer_A_CompareModeConfig compareConfig_PWM3 =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_3,          // Use CCR2
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_RESET,              // Toggle output but
        10                                // 96 Duty Cycle
};
const Timer_A_CompareModeConfig compareConfig_PWM4 =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_4,          // Use CCR2
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_RESET,              // Toggle output but
        10                                // 96 Duty Cycle
};

void Pwm_init(void)
{
 /* Setting P9.2 P9.3 and peripheral outputs for CCR */
        MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9,
                0x0C, GPIO_PRIMARY_MODULE_FUNCTION);  // 9.2 9.3 TIMA3 3 4

        /* Configuring Timer_A3 for UpDown Mode and starting */
        MAP_Timer_A_configureUpDownMode(TIMER_A3_BASE, &upDownConfig);
        MAP_Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UPDOWN_MODE);

        /* Initialize compare registers to generate PWM1 */
        MAP_Timer_A_initCompare(TIMER_A3_BASE, &compareConfig_PWM3);
        /* Initialize compare registers to generate PWM2 */
        MAP_Timer_A_initCompare(TIMER_A3_BASE, &compareConfig_PWM4);
}
