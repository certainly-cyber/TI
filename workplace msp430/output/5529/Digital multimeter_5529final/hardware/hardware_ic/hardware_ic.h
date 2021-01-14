/*
 * hardware_ic.h
 *
 *  Created on: 2020Äê8ÔÂ11ÈÕ
 *      Author: DELL
 */

#ifndef HARDWARE_HARDWARE_IC_HARDWARE_IC_H_
#define HARDWARE_HARDWARE_IC_HARDWARE_IC_H_

#define     ADS_ADDR        0x40

void initI2C(void);

unsigned char i2cRead(unsigned char slaveaddr,unsigned char *tx_ptr,unsigned char *rx_ptr,unsigned char tx_no,unsigned char rx_no);
unsigned char i2cSend(unsigned char slaveaddr,unsigned char *ptr,unsigned char byteno);






#endif /* HARDWARE_HARDWARE_IC_HARDWARE_IC_H_ */
