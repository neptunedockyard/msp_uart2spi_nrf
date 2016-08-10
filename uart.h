/*
 * uart.h
 *
 *  Created on: Aug 8, 2016
 *      Author: Michal
 */

#ifndef UART_H_
#define UART_H_

#include "msp430g2553.h"

static char Logo1[] = {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'};
static char Logo2[] =  {'*', 'T', 'i', ' ', 'M', 'S', 'P', '4', '3', '0', ' ', 'S', 'y', 's', 't', 'e', 'm', '*'};

void Init_UART();
void UART_sendChar(char msg);
void UART_sendTxt(char *TxArray,  char ArrayLength);
void UART_printLogo();
void UART_printStatus();
void UART_puts(const char *str);
void UART_putc(unsigned char c);

#endif /* UART_H_ */
