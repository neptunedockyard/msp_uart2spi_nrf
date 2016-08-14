/*
 * sonar.c
 *
 *  Created on: Aug 13, 2016
 *      Author: Michal
 */

#include "sonar.h"

static int milliseconds = 0;
static float distance = 0;
static long sensor = 0;

int Init_sonar(void){
	//setup TRIG as an output
//	P2SEL = TRIG;
	P2OUT |= TRIG;
	P2OUT ^= TRIG;
	P2DIR |= TRIG;

	//setup ECHO as an input with interrupt enabled
	P2IES |= ECHO;
	P2IFG &= ~ECHO;
	P2IE |= ECHO;

	__bis_SR_register(GIE);

	return 1;
}

int Init_timerA0(void){
	CCTL0 = CCIE;
	TA0CTL |= TASSEL_2 + MC_1;
	CCR0 = 10000;
	__bis_SR_register(GIE);

	return 1;
}

void delay_us(int length){
	while(--length){
		asm("NOP");
	}
}

void trig(void){
	P2OUT ^= TRIG;
//	__delay_cycles(10);
	delay_us(10);
	P2OUT ^= TRIG;
}

void calc(long input){
	distance = (float)input/58;
	if((distance < 2 && distance != 0) || (distance > 400)){
		UART_putcolor('r');
		UART_putsn((char *)"Out of range!\r");
		UART_putcolor('w');
	}else{
		UART_putsn((char *)"");
		char buf1[12];
		char buf2[12];
//		itoa(distance, buffer, 10);
		ftoa(buf1, buf2, distance);
		UART_puts((char *)"Distance: ");
		UART_putcolor('g');
		UART_puts(buf1);
		UART_puts(".");
		UART_puts(buf2);
		UART_putcolor('w');
		UART_puts((char *)" cm, time of flight: ");
		UART_putcolor('g');
		itoa(input, buf1, 10);
		UART_puts(buf1);
		UART_putcolor('w');
		UART_puts((char *)" ms\r");
		Delay_ms(300);
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	if(P2IFG&ECHO){
		if(!(P2IES&ECHO)){
			TACTL |= TACLR;
			milliseconds = 0;
		} else {
			sensor = (long)milliseconds*1000 + (long)TAR;
			calc(sensor);
		}
		P2IES ^= ECHO;
		P2IFG &= ~ECHO;
	}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
	milliseconds++;
}
