/*
 * pwm.h
 *
 *  Created on: Aug 14, 2016
 *      Author: Michal
 */

#ifndef PWM_H_
#define PWM_H_

#include <msp430g2553.h>

int Init_pwm(int period, int duty_cycle, int pin);
int Init_timerA1(void);
void pin_blink_m(int pin, int number, int length);

#endif /* PWM_H_ */
