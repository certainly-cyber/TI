/*
 * usart.h
 *
 *  Created on: 2020Äê10ÔÂ5ÈÕ
 *      Author: certainly
 */

#ifndef USART_H_
#define USART_H_
#include <stdio.h>
#include <string.h>
#include <stdint.h>
void usart_init();
int fputc(int _c, register FILE *_fp);
int fputs(const char *_ptr, register FILE *_fp);



#endif /* USART_H_ */
