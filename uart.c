/*
 * uart.c
 *
 *  Created on: Aug 8, 2016
 *      Author: Michal
 */

#include "uart.h"

void Init_UART(){
	/* Configure hardware UART */
	P1SEL |= BIT1 + BIT2 ;			// P1.1 = RXD, P1.2=TXD
	P1SEL2 |= BIT1 + BIT2 ;			// P1.1 = RXD, P1.2=TXD

	UCA0CTL1 |= UCSSEL_2;			// Use SMCLK
	UCA0BR0 = 104;					// Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
	UCA0BR1 = 0;					// Set baud rate to 9600 with 1MHz clock
	UCA0MCTL = UCBRS0;				// Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;			// Initialize USCI state machine
	IE2 |= UCA0RXIE;				// Enable USCI_A0 RX interrupt

	//__bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
	__bis_SR_register(GIE);
}

void UART_sendChar(char msg){
	while (!(IFG2 & UCA0TXIFG)); 	// wait for buffer ready
	UCA0TXBUF = msg;
	__delay_cycles(100);
}

void UART_sendTxt(char *TxArray,  char ArrayLength){
	// Send number of bytes Specified in ArrayLength in the array at using the hardware UART 0
	// Example usage: UARTSendArray("Hello", 5);
	// int data[2]={1023, 235};
	// UARTSendArray(data, 4); // Note because the UART transmits bytes it is necessary to send two bytes for each integer hence the data length is twice the array length

	while(ArrayLength--){ 				// Loop until StringLength == 0 and post decrement
		while(!(IFG2 & UCA0TXIFG)); 	// Wait for TX buffer to be ready for new data
		UCA0TXBUF = *TxArray; 			//Write the character at the location specified py the pointer
		TxArray++; 						//Increment the TxString pointer to point to the next character
	}
}

void UART_putc(unsigned char c){
	while(!(IFG2&UCA0TXIFG));
	UCA0TXBUF = c;
}

void UART_puts(const char *str){
	while(*str) UART_putc(*str++);
}

void UART_printLogo(){
    UART_sendTxt(Logo1, sizeof(Logo1));
    UART_sendTxt("\r\n", 2);
    UART_sendTxt(Logo2, sizeof(Logo2));
	UART_sendTxt("\r\n", 2);
	UART_sendTxt(Logo1, sizeof(Logo1));
	UART_sendTxt("\r\n", 2);
}

void UART_printStatus(){
	UART_sendTxt("Starting MSP430 Subsystem\r\n", 27);
	UART_sendTxt("Clock init ....... OK\r\n", 23);
	UART_sendTxt("LED init ......... OK\r\n", 23);
	UART_sendTxt("UART init ........ OK\r\n", 23);
	UART_sendTxt("I2C init ......... OK\r\n", 23);
	UART_sendTxt("Btn init ......... OK\r\n", 23);
}
