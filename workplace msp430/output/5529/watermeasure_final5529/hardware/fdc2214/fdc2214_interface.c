#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "i2c.h"
#include "fdc2214_register.h"
#include "fdc2214_interface.h"

/*
 * @brief: 初始化fdc2214使用的GPIO功能口
 * @param： none
 * @return： none
 */
static void fdc2214_gpio_init(void)
{
    //ADDR & SD OUTPUT
    FDC_GPIO_DIR |= FDC_ADDR_PIN | FDC_SD_PIN;
    FDC_GPIO_OUT |= FDC_ADDR_PIN;
    FDC_GPIO_OUT &= ~FDC_SD_PIN;

    //INTB INPUT
    FDC_GPIO_DIR &= ~FDC_INTB_PIN;
    FDC_GPIO_REN |= FDC_INTB_PIN;
    FDC_GPIO_OUT |= FDC_INTB_PIN;
}

/*
 * @brief：选择fdc2213的地址值
 * @param：state
 *          @arg：true，选择地址位输出高电平，地址为0x2B
 *          @arg：true，选择地址位输出低电平，地址为0x2A
 * @return：设置的地址值，0x2B或者0x2A
 */
static uint8_t fdc2214_select_address(bool state)
{
    if(state)
    {
        FDC_GPIO_OUT |= FDC_ADDR_PIN;
        return (FDC_ADDRESS_BASE | 0x01);
    }
    else
    {
        FDC_GPIO_OUT &= ~FDC_ADDR_PIN;
        return FDC_ADDRESS_BASE;
    }
}


/*
 * @brief：初始化fdc2214
 * @param： none
 * @return：none
 */
void fdc2214_init(void)
{
    fdc2214_gpio_init();

    i2c_init(fdc2214_select_address(true));

}

/*
 * @brief: 获取Manufacturer id
 * @param：none
 * @return：Manufacturer id
 */
uint16_t fdc2214_get_manufacturer_id(void)
{
    return i2c_read_register(REG_MANUFACTURER_ID);;
}

/*
 * @brief: 获取设备ID
 * @param：none
 * @return：device id
 */
uint16_t fdc2214_get_device_id(void)
{
    return i2c_read_register(REG_DEVICE_ID);
}

/*
 * @brief: 检测设备是否是fdc2214
 * @param：none
 * @return：true or false
 */
bool fdc2214_check(void)
{
    if(fdc2214_get_device_id() == VALUE_DEVICE_ID)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*
 * @brief: 配置fdc2214工作模式, 开启所有通道工作，ch0，ch1，ch2，ch3配置参数介绍可参考数据手册p42-43
 * @param: none
 * @return: 工作模式
 */
void fdc2214_configure_work_mode(void)
{
    i2c_write_register(REG_RCOUNT_CH0, VALUE_RCOUNT_CH0);
    i2c_write_register(REG_RCOUNT_CH1, VALUE_RCOUNT_CH1);
    i2c_write_register(REG_RCOUNT_CH2, VALUE_RCOUNT_CH2);
    i2c_write_register(REG_RCOUNT_CH3, VALUE_RCOUNT_CH3);

    i2c_write_register(REG_SETTLECOUNT_CH0, VALUE_SETTLECOUNT_CH0);
    i2c_write_register(REG_SETTLECOUNT_CH1, VALUE_SETTLECOUNT_CH1);
    i2c_write_register(REG_SETTLECOUNT_CH2, VALUE_SETTLECOUNT_CH2);
    i2c_write_register(REG_SETTLECOUNT_CH3, VALUE_SETTLECOUNT_CH3);

    i2c_write_register(REG_CLOCK_DIVIDERS_CH0, VALUE_CLOCK_DIVIDER_CH0);
    i2c_write_register(REG_CLOCK_DIVIDERS_CH1, VALUE_CLOCK_DIVIDER_CH1);
    i2c_write_register(REG_CLOCK_DIVIDERS_CH2, VALUE_CLOCK_DIVIDER_CH2);
    i2c_write_register(REG_CLOCK_DIVIDERS_CH3, VALUE_CLOCK_DIVIDER_CH3);

    i2c_write_register(REG_STATUS_CONFIG, VALUE_ERROR_CONFIG);

    i2c_write_register(REG_MUX_CONFIG, VALUE_MUX_CONFIG);

    i2c_write_register(REG_DRIVE_CURRENT_CH0, VALUE_DRIVE_CURRENT_CH0);
    i2c_write_register(REG_DRIVE_CURRENT_CH1, VALUE_DRIVE_CURRENT_CH1);
    i2c_write_register(REG_DRIVE_CURRENT_CH2, VALUE_DRIVE_CURRENT_CH2);
    i2c_write_register(REG_DRIVE_CURRENT_CH3, VALUE_DRIVE_CURRENT_CH3);

    i2c_write_register(REG_CONFIG, VALUE_CONFIG);
}

/*
 * @brief: 获取通道数据
 * @param：channel:通道
 * @return：通道值
 */
uint32_t fdc2214_get_channel_data(e_channel channel)
{
    uint32_t value = 0, value_lsb = 0;
    switch(channel)
    {
    case CHANNEL_0:
        value = i2c_read_register(REG_DATA_CH0);
        value_lsb = i2c_read_register(REG_DATA_LSB_CH0);
        break;
    case CHANNEL_1:
        value = i2c_read_register(REG_DATA_CH1);
        value_lsb = i2c_read_register(REG_DATA_LSB_CH1);
        break;
    case CHANNEL_2:
        value = i2c_read_register(REG_DATA_CH2);
        value_lsb = i2c_read_register(REG_DATA_LSB_CH2);
        break;
    case CHANNEL_3:
        value = i2c_read_register(REG_DATA_CH3);
        value_lsb = i2c_read_register(REG_DATA_LSB_CH3);
        break;
    default:
        break;
    }

    return (value << 16) | (value_lsb & 0x0000ffff);
}

/*
 * @brief: 获取某个工作模式下的所使用的所有通道数据
 * @param：none
 * @return：通道数据
 */
t_channel_data fdc2214_get_data(void)
{
    t_channel_data value = {0,0,0,0};

    value.v_ch0 = fdc2214_get_channel_data(CHANNEL_0);
    value.v_ch1 = fdc2214_get_channel_data(CHANNEL_1);
    value.v_ch2 = fdc2214_get_channel_data(CHANNEL_2);
    value.v_ch3 = fdc2214_get_channel_data(CHANNEL_3);

    return value;
}

/*
 * @brief: 计算电容值
 * @param: data, 通道数据
 * @return： 电容值
 */
double fdc2214_calc_capatiance(uint32_t data)
{
    double fsensor = 0.0;

    fsensor = 1.0 * FDC_FREF_X * (data & 0x0FFFFFFF) / pow(2,28);                   //MHz

    double capatiance = 0.0;
    capatiance = 1.0 / (1.0 * FDC_PARALLEL_INDUCTOR * pow(2 * M_PI * fsensor, 2));  //uF

    return (capatiance * 1000000 - FDC_PARALLEL_CAPACITOR);                         //pF
}

/*
 * @brief: 根据已经获取的通道数据取出电容值
 * @param: *p_data, 指向通道数据的指针
 * @return: 通道电容值
 */
t_channel_capatiance fdc2214_get_capatiance(t_channel_data *p_data)
{
    t_channel_capatiance cap={0};

    cap.v_ch0 = fdc2214_calc_capatiance(p_data->v_ch0);
    cap.v_ch1 = fdc2214_calc_capatiance(p_data->v_ch1);
    cap.v_ch2 = fdc2214_calc_capatiance(p_data->v_ch2);
    cap.v_ch3 = fdc2214_calc_capatiance(p_data->v_ch3);

    return cap;
}

/*
 * @brief：根据电容值计算获取水位信息
 * @param: none
 * @return: 水位值
 */
double fdc2214_get_waterLevel(t_channel_capatiance *p_data)
{
    return 0.0;
}
