/*
 * pwm.c
 *
 *  Created on: Aug 14, 2016
 *      Author: Michal
 */

#include "pwm.h"

int Init_pwm(int period, int duty_cycle, int pin)
{
	int dc = 0;

	P2DIR |= pin;
	P2SEL |= pin;
	P2SEL2 &= ~pin;

	dc = (duty_cycle * period) / 100;

	TA1CCR0 = period-1;
	TA1CCR1 = dc;
//	TA1CCR2 = dc;
	TA1CCTL1 = OUTMOD_7;
	TA1CTL = (TASSEL_2 | MC_1);

	__bis_SR_register(GIE);

	return 1;
}

int Init_timerA1(void){
	TA1CCR0 = 1000-1;
	TA1CCR1 = 10;
	TA1CCTL0 |= OUTMOD_7;
	TA1CTL |= TASSEL_2 + MC_1;
	__bis_SR_register(GIE);

	return 1;
}

void pin_blink_m(int pin, int number, int length){
	P2DIR |= pin;
	int p = 190;
	while(number--){
		P2OUT ^= pin;
		while(length--)
			__delay_cycles(100);
		P2OUT &= ~pin;
		while(p--)
			__delay_cycles(100);
	}
	P2DIR &= ~pin;
}
