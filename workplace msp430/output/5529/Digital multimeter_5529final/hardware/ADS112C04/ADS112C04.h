/*
 * ADS112C04.h
 *
 *  Created on: 2020Äê8ÔÂ11ÈÕ
 *      Author: DELL
 */

#ifndef HARDWARE_ADS112C04_ADS112C04_H_
#define HARDWARE_ADS112C04_ADS112C04_H_


#include <stdint.h>

#define     CMD_RESET       0x06
#define     CMD_START       0x08
#define     CMD_PD          0x02
#define     CMD_RDATA       0x10
#define     CMD_RREG        0x20
#define     CMD_WREG        0x40
#define     DRDY            0x80


void adsSoftwareRST(void);
void adsStartCONV(void);
void adsPowerDown(void);
void adsReadDATA(uint8_t *ptr);
void adsReadREG(uint8_t reg_addr,uint8_t *ptr);
void adsWriteREG(uint8_t reg_addr,uint8_t data);
void hardwareResetADS(void);
void adsReadcounterPLUSdata(uint8_t *ptr);
void initADS(void);




#endif /* HARDWARE_ADS112C04_ADS112C04_H_ */
