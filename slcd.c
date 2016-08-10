/*
 * slcd.c
 *
 *  Created on: Aug 9, 2016
 *      Author: Michal
 */

#include "slcd.h"
#include "si2c.h"

void send(int cmd, int mode){
	int high = cmd&0xf0;
	int low = (cmd<<4)&0xf0;
	Transmit(high|mode);
	Transmit(low|mode);
}

void write(int cmd){
	send(cmd, RS);
}

void command(int cmd){
	send(cmd, 0);
}
