/*
 * spi.c
 *
 *  Created on: Aug 8, 2016
 *      Author: Michal
 */

#include "spi.h"
#include "nrf.h"

void Init_SPI(void){
	P1SEL |= BIT5 + BIT6 + BIT7;
	P1SEL2 |= BIT5 + BIT6 + BIT7;
	P1DIR |= CS;
	P1OUT |= CS;

//	UCA0CTL1 = UCSWRST;
//	UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; 	// 3-pin, 8-bit SPI master
//	UCA0CTL1 |= UCSSEL_2; 							// SMCLK
//	UCA0BR0 |= 0x02; 								// /2
//	UCA0BR1 = 0; 									//
//	UCA0MCTL = 0; 									// No modulation
//	UCA0CTL1 &= ~UCSWRST; 							// **Initialize USCI state machine**

	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
	UCB0CTL1 |= UCSSEL_2;
	UCB0CTL1 &= ~UCSWRST;

	spi_CSH();

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

void spi_CSH(void){
	P1OUT |= CS;
}

void spi_CSL(void){
	P1OUT &= ~CS;
}

unsigned char spi_xfer_byte(unsigned char data){
	UCB0TXBUF = data;
	while(!(IFG2&UCB0TXIFG));
	return UCB0RXBUF;
}

unsigned char getRegister(char reg){
	spi_CSL();
	unsigned char stat = spi_xfer_byte(reg&0x1F);
	unsigned char conf = spi_xfer_byte(NRF_CMD_NOP);
	spi_CSH();
	return conf;
}

void setRegister(char reg, char value){
	spi_CSL();
	unsigned char stat = spi_xfer_byte((reg&0x1F)|0x20);
	unsigned char conf = spi_xfer_byte(value);
	spi_CSH();
}

void powerUp(void){
	setRegister(_NRF24_CONFIG,getRegister(_NRF24_CONFIG)|0x02);
}
