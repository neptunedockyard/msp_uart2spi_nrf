/*
 * i2c.h
 *
 *  Created on: Aug 8, 2016
 *      Author: Michal
 */

#ifndef SI2C_H_
#define SI2C_H_

#include "msp430g2553.h"
#include "slcd.h"

static char tgtAddress = 0x27;
static char myAddress = 0x96;

int TXByteCtr;
unsigned char PRxData;
//int Rxd = 0;
static unsigned char *PTxData;
static unsigned char TXData[] = {LCD_CLEARDISPLAY,LCD_DISPLAYON,LCD_BLINKON,LCD_BACKLIGHT};

int Init_i2c();
void Set_TGT(char address);
void Transmit();
void Transmit_char(char input);
//void Transmit_char(int input);
void Receive();

#endif /* SI2C_H_ */
