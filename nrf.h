/*
 * nrf.h
 *
 *  Created on: Aug 9, 2016
 *      Author: Michal
 */

#ifndef NRF_H_
#define NRF_H_

//Command to send for requesting to read a register from the NRF
#define NRF_CMD_RREG 0x00

//Command to send when only reading a result from the NRF
#define NRF_CMD_NOP	0xff

//Address of the configuration register of the NRF
#define NRF_REG_CONFIG	0x00

#define _NRF24_CONFIG      0x00
#define _NRF24_EN_AA       0x01
#define _NRF24_RF_CH       0x05
#define _NRF24_RF_SETUP    0x06
#define _NRF24_RPD         0x09

#endif /* NRF_H_ */
