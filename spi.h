/*
 * spi.h
 *
 *  Created on: Aug 8, 2016
 *      Author: Michal
 */

#ifndef SPI_H_
#define SPI_H_

#include "msp430g2553.h"

#define CS BIT4

int Init_SPI(void);
char SPI_txrxChar(char msg);
void spi_CSH(void);
void spi_CSL(void);

unsigned char getRegister(char reg);
void setRegister(char reg, char value);
void powerUp(void);

#endif /* SPI_H_ */
