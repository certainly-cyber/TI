/*
 * bluetooth.h
 *
 *  Created on: 2020Äê9ÔÂ3ÈÕ
 *      Author: certainly
 */

#ifndef BLUETOOTH_BLUETOOTH_H_
#define BLUETOOTH_BLUETOOTH_H_
#include <stdint.h>

void GPIO_init();
void USART_init();
void UartPutchar(unsigned char c);
void uart_write_multi(uint8_t *p_buffer, uint8_t length);
unsigned char UartGetchar();



#endif /* BLUETOOTH_BLUETOOTH_H_ */
