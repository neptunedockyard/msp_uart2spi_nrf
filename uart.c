/*
 * uart.c
 *
 *  Created on: Aug 8, 2016
 *      Author: Michal
 */

#include "uart.h"

int Init_UART(){
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

	UART_puts((char *)"\033[0;37m");

	return 1;
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
	//to use:
	//uart_puts((char *)")\n\r");
	while(*str) UART_putc(*str++);
}

void UART_putsn(const char *str){
	//to use:
	//uart_puts((char *)")\n\r");
	UART_puts((char *)"\033[0;2K");			//this clears the current line
	while(*str) UART_putc(*str++);
}

void UART_printLogo(){
	UART_putcolor('g');
	UART_puts((char *)"*********************\r\n* ");
	UART_putcolor('b');
	UART_puts((char *)"TI MSP430 uSystem");
	UART_putcolor('g');
	UART_puts((char *)" *\r\n*********************\r\n");
	UART_putcolor('w');
}

void UART_printStatus(int i2c, int spi, int button, int clock, int led, int uart, int sonar, int timer0, int timer1, int pwm){
	UART_puts((char *)"\033[0;37mStarting MSP430 Subsystem\r\n");
	if(clock)
		UART_puts((char *)"Clock init ........... \033[0;32mOK\r\n\033[0;37m");
	else
		UART_puts((char *)"Clock init ......... \033[0;31mFAIL\r\n\033[0;37m");
	if(led)
		UART_puts((char *)"LED init ............. \033[0;32mOK\r\n\033[0;37m");
	else
		UART_puts((char *)"LED init ........... \033[0;31mFAIL\r\n\033[0;37m");
	if(uart)
		UART_puts((char *)"UART init ............ \033[0;32mOK\r\n\033[0;37m");
	else
		UART_puts((char *)"UART init .......... \033[0;31mFAIL\r\n\033[0;37m");
	if(spi)
		UART_puts((char *)"SPI init ............. \033[0;32mOK\r\n\033[0;37m");
	else
		UART_puts((char *)"SPI init ........... \033[0;31mFAIL\r\n\033[0;37m");
	if(i2c)
		UART_puts((char *)"I2C init ............. \033[0;32mOK\r\n\033[0;37m");
	else
		UART_puts((char *)"I2C init ........... \033[0;31mFAIL\r\n\033[0;37m");
	if(button)
		UART_puts((char *)"Btn init ............. \033[0;32mOK\r\n\033[0;37m");
	else
		UART_puts((char *)"Btn init ........... \033[0;31mFAIL\r\n\033[0;37m");
	if(sonar)
		UART_puts((char *)"Sonar init ........... \033[0;32mOK\r\n\033[0;37m");
	else
		UART_puts((char *)"Sonar init ......... \033[0;31mFAIL\r\n\033[0;37m");
	if(timer0)
		UART_puts((char *)"Timer0 init .......... \033[0;32mOK\r\n\033[0;37m");
	else
		UART_puts((char *)"Timer0 init ........ \033[0;31mFAIL\r\n\033[0;37m");
	if(timer1)
		UART_puts((char *)"Timer1 init .......... \033[0;32mOK\r\n\033[0;37m");
	else
		UART_puts((char *)"Timer1 init ........ \033[0;31mFAIL\r\n\033[0;37m");
	if(pwm)
		UART_puts((char *)"PWM init ............. \033[0;32mOK\r\n\033[0;37m");
	else
		UART_puts((char *)"PWM init ........... \033[0;31mFAIL\r\n\033[0;37m");
}

void printreg(char *name, int n){
	int size = 8;
    int i;

	UART_puts((char *)"- ");
	UART_puts(name);
	UART_puts((char *)": ");

    int mask = 1 << (size - 1);

    for(i = 0; i < size; i++) {
         if((n & (mask >> i)) != 0) {
        	 UART_putc('1');
         } else {
        	 UART_putc('0');
         }
    }
    UART_puts((char *)" (");
    UART_putc(n);
	UART_puts((char *)")\n\r");
}

void ftoa(char *head, char *tail, float flt){
	int intpart = (int)flt;
	int decpart = (int)(100*(flt - intpart));
	itoa(intpart, head, 10);
	itoa(decpart, tail, 10);
}


static char *i2a(unsigned i, char *a, unsigned r)
{
	if (i/r > 0) a = i2a(i/r,a,r);
	*a = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i%r];
	return a+1;
}

char *itoa(int i, char *a, int r)
{
	if ((r < 2) || (r > 36)) r = 10;

	if (i < 0)
	{
		*a = '-';
		*i2a(-(unsigned)i,a+1,r) = 0;
	}
	else *i2a(i,a,r) = 0;
	return a;
}

void UART_putcolor(char color){
	switch(color){
	case 'k': {									//black
		UART_puts((char *)"\033[0;30m");
	} break;
	case 'r': {									//red
		UART_puts((char *)"\033[0;31m");
	} break;
	case 'g': {									//green
		UART_puts((char *)"\033[0;32m");
	} break;
	case 'y': {									//yellow
		UART_puts((char *)"\033[0;33m");
	} break;
	case 'b': {									//blue
		UART_puts((char *)"\033[0;34m");
	} break;
	case 'm': {									//magenta
		UART_puts((char *)"\033[0;35m");
	} break;
	case 'c': {									//cyan
		UART_puts((char *)"\033[0;36m");
	} break;
	case 'w': {									//white
		UART_puts((char *)"\033[0;37m");
	} break;
	default: {
		UART_puts((char *)"\033[0;39m");
	} break;
	}
}
