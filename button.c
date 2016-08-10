/*
 * button.c
 *
 *  Created on: Aug 9, 2016
 *      Author: Michal
 */

#include "button.h"

void Init_Button(){
	P1SEL |= ~BIT3;				// Make sure P1.1 and P1.4 are set as a Digital I/O
	P1SEL2 |= ~BIT3;			// Make sure P1.1 and P1.4 are set as a Digital I/O
	P1OUT = BIT3;
	P1REN |= BIT3;

	P1IE |= BIT3;				// P1.3 interrupt enabled
	P1IES |= BIT3;				// P1.3 Hi/lo edge
	P1IFG &= ~BIT3;				// P1.3 IFG cleared

	__bis_SR_register(GIE);
}
