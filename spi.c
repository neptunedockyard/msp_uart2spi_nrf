/*
 * spi.c
 *
 *  Created on: Aug 8, 2016
 *      Author: Michal
 */

#include "spi.h"

void Init_SPI(){
	P1SEL |= BIT5 | BIT6 | BIT7;
	P1SEL2 |= BIT5 | BIT6 | BIT7;

	UCA0CTL1 = UCSWRST;
	UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; 	// 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2; 							// SMCLK
	UCA0BR0 |= 0x02; 								// /2
	UCA0BR1 = 0; 									//
	UCA0MCTL = 0; 									// No modulation
	UCA0CTL1 &= ~UCSWRST; 							// **Initialize USCI state machine**

	__bis_SR_register(GIE);
}

char SPI_txrxChar(char msg){
	P1OUT &= (~BIT5); // Select Device

	while (!(IFG2 & UCA0TXIFG)); // USCI_A0 TX buffer ready?
	UCA0TXBUF = 0xAA; // Send 0xAA over SPI to Slave
	while (!(IFG2 & UCA0RXIFG)); // USCI_A0 RX Received?
	msg = UCA0RXBUF; // Store received data

	P1OUT |= (BIT5); // Unselect Device

	return msg;
}
