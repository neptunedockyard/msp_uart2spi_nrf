/*
 * spi.h
 *
 *  Created on: Aug 8, 2016
 *      Author: Michal
 */

#ifndef SPI_H_
#define SPI_H_

#include "msp430g2553.h"

void Init_SPI();
char SPI_txrxChar(char msg);

#endif /* SPI_H_ */
