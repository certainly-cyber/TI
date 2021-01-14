/*
 * ADS112C04.c
 *
 *  Created on: 2020年8月11日
 *      Author: DELL
 */

#include <msp430.h>
#include <stdint.h>
#include "ADS112C04.h"
#include "hardware_ic.h"


/*
 * @fn:     void adsSoftwareRST(void)
 * @brief:  软件复位ADS
 * @para:   none
 * @return: none
 * @comment:none
 */
void adsSoftwareRST(void)
{
    uint8_t t_cmd[1] = {CMD_RESET};
    i2cSend(ADS_ADDR,t_cmd,1);
}
/*
 * @fn:     void adsStartCONV(void)
 * @brief:  开始转换
 * @para:   none
 * @return: none
 * @comment:none
 */
void adsStartCONV(void)
{
    uint8_t t_cmd[1] = {CMD_START};
    i2cSend(ADS_ADDR,t_cmd,1);
}
/*
 * @fn:     void adsPowerDown(void)
 * @brief:  进入掉电模式
 * @para:   none
 * @return: none
 * @comment:none
 */
void adsPowerDown(void)
{
    uint8_t t_cmd[1] = {CMD_PD};
    i2cSend(ADS_ADDR,t_cmd,1);
}
/*
 * @fn:     void adsReadDATA(uint8_t *ptr)
 * @brief:  读转换结果
 * @para:   none
 * @return: none
 * @comment:none
 */
void adsReadDATA(uint8_t *ptr)
{
    uint8_t t_cmd[1] = {CMD_RDATA};
    i2cRead(ADS_ADDR,t_cmd,ptr,1,2);
}
/*
 * @fn:     void adsReadREG(uint8_t reg_addr,uint8_t *ptr)
 * @brief:  读寄存器值
 * @para:   none
 * @return: none
 * @comment:none
 */
void adsReadREG(uint8_t reg_addr,uint8_t *ptr)
{
    uint8_t t_cmd[1] = {CMD_RREG};
    t_cmd[0] |= (reg_addr << 2);
    i2cRead(ADS_ADDR,t_cmd,ptr,1,1);
}
/*
 * @fn:     void adsWriteREG(uint8_t reg_addr,uint8_t data)
 * @brief:  写寄存器值
 * @para:   none
 * @return: none
 * @comment:none
 */
void adsWriteREG(uint8_t reg_addr,uint8_t data)
{
    uint8_t t_cmd[2] = {CMD_WREG,0x00};
    t_cmd[0] |= (reg_addr << 2);
    t_cmd[1] = data;
    i2cSend(ADS_ADDR,t_cmd,2);
}
/*
 * @fn:     void hardwareResetADS(void)
 * @brief:  硬件复位ADS
 * @para:   none
 * @return: none
 * @comment:none
 */
void hardwareResetADS(void)
{
    /*拉低RESET进行复位*/
    P1OUT &= ~BIT4;
    /*延时一段时间*/
    __delay_cycles(10000);
    /*释放RESET，复位完成*/
    P1OUT |= BIT4;
}
/*
 * @fn:     void initADS(void)
 * @brief:  初始化ADS
 * @para:   none
 * @return: none
 * @comment:none
 */

/*
 * @fn:     void adsReadREG(uint8_t reg_addr,uint8_t *ptr)
 * @brief:  在conversion counter下读寄存器值 分成3个部分读取
 * @para:   none
 * @return: none
 * @comment:none
 */
void adsReadcounterPLUSdata(uint8_t *ptr)
{
    uint8_t t_cmd[1] = {CMD_RDATA};
    i2cRead(ADS_ADDR,t_cmd,ptr,1,3);//注意数组内元素个数为三个
}


void initADS(void)
{
    /*设置RESET所连端口P1.4为输出*/
    P1DIR |= BIT4;
    /*硬件复位ADS*/
    hardwareResetADS();
    /*软件复位ADS*/
    adsSoftwareRST();
    //__delay_cycles(1000);
    /*设置寄存器00*/
//    adsWriteREG(0x00,0xD1);  //1101 0001
    adsWriteREG(0x00,0x81);  //AINP = AIN0, AINN = AVSS
    /*设置寄存器01*/
//    adsWriteREG(0x01,0xD8);  //1101 1000
//    adsWriteREG(0x01,0xD9);  //1101 1001
    adsWriteREG(0x01,0x18);  //0001 1000 速率每秒40次，高速模式，连续转换
    /*设置寄存器02*/
//    adsWriteREG(0x02,0x06);    //
//    /*设置寄存器03*/
//    adsWriteREG(0x03,0x48);    //0100 1000
}



