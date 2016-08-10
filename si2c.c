/*
 * si2c.c
 *
 *  Created on: Aug 8, 2016
 *      Author: Michal
 */

#include "si2c.h"

void Init_i2c(){
//	P1OUT = BIT6 + BIT7;
	P1SEL |= BIT6 + BIT7;						// Assign I2C pins to USCI_B0
	P1SEL2 |= BIT6 + BIT7;						// Assign I2C pins to USCI_B0
//	P1REN |= BIT6 + BIT7;						// Use external pull up resistors

	UCB0CTL1 |= UCSWRST;						// Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;		// I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;				// Use SMCLK, keep SW reset
	UCB0BR0 = 12;								// fSCL = SMCLK/12 = ~100kHz
	UCB0BR1 = 0;
	UCB0I2CSA = tgtAddress;						// Slave Address
	UCB0CTL1 &= ~UCSWRST;						// Clear SW reset, resume operation
	IE2 |= UCB0TXIE + UCB0RXIE;					// Enable TX interrupt
}

void Set_TGT(char address){
	UCB0I2CSA = address;
}

void Transmit(){
	PTxData = (unsigned char *)TXData;
	TXByteCtr = sizeof(TXData);
	while(UCB0CTL1 & UCTXSTP);
	UCB0CTL1 |= UCTR + UCTXSTT;
	__bis_SR_register(GIE);
	__delay_cycles(100);
}

void Transmit_char(char input){
	TXByteCtr = 2;
	PTxData = input;
	while(UCB0CTL1 & UCTXSTP);
	UCB0CTL1 |= UCTR + UCTXSTT;
	__bis_SR_register(GIE);
	__delay_cycles(100);
}

void Receive(){
	while (UCB0CTL1 & UCTXSTP);					// Ensure stop condition got sent
	UCB0CTL1 &= ~UCTR;							// Clear UCTR
	UCB0CTL1 |= UCTXSTT;						// I2C start condition
	while (UCB0CTL1 & UCTXSTT);					// Start condition sent?
	UCB0CTL1 |= UCTXSTP;						// I2C stop condition
	__bis_SR_register(GIE);
}
