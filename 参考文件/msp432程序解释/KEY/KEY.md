``` C
 采取这种方式判断那里被按下
void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    /* Toggling the output on the LED */
    if(status & GPIO_PIN1)
    {
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }

}

```
+  MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);
+ 此行代码在于在LPM3模式下打开SRAM
+ MAP_Interrupt_enableMaster();
+ 这是一个空的框架，用于写其他的中断开启