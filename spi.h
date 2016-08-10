/*
 * spi.h
 *
 *  Created on: Aug 8, 2016
 *      Author: Michal
 */

#ifndef SPI_H_
#define SPI_H_

#include "msp430g2553.h"

void Init_SPI(void);
char SPI_txrxChar(char msg);
void spi_CSH(void);
void spi_CSL(void);

#endif /* SPI_H_ */
