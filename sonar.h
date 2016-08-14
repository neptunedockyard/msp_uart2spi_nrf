/*
 * sonar.h
 *
 *  Created on: Aug 13, 2016
 *      Author: Michal
 */

#ifndef SONAR_H_
#define SONAR_H_

#include "msp430g2553.h"
#include "uart.h"

#define ECHO BIT1	//on P2, pin 2.1
#define TRIG BIT0	//on P2, pin 2.0



int Init_sonar(void);
int Init_timerA0(void);
void delay_us(int length);
void trig(void);
void calc(long);

#endif /* SONAR_H_ */
