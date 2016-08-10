//#include <io.h>
#include <msp430.h>
#include <stdint.h>
#include "msp430g2553.h" //use your msp430 configuration!
#include "uart.h"
#include "spi.h"
#include "si2c.h"
#include "button.h"
#include "slcd.h"
#include "nrf.h"

static char data;
unsigned int i = 0;
unsigned char c = 0x00;

// function prototypes
void Delay_ms(unsigned int);
void Init_Clock(int);
void Init_Led(char);

int main() {
    WDTCTL = WDTPW + WDTHOLD;
    Init_Led('r');
//    Init_Button();
    Init_Clock(1);
    Init_UART();
    Init_SPI();
//    Init_i2c();

    for(i = 0; i < 30; i++)
    	UART_sendTxt("\r\n", 2);
    UART_printLogo();
    UART_printStatus();
    for(;;);
}

void Init_Led(char led){
	//r:Red = BIT0, g:Green = BIT6, b:Both
	int bit;
	switch(led){
	case 'r': {
		bit = BIT0;
	} break;
	case 'g': {
		bit = BIT6;
	} break;
	case 'b': {
		bit = BIT0 + BIT6;
	} break;
	default: {
		bit = 0;
	} break;
	}
	P1DIR |= bit;
	P1OUT = bit;
}

void Init_Clock(int speed){
	switch(speed) {
		case 1: {
			BCSCTL1 = CALBC1_1MHZ;
			DCOCTL = CALDCO_1MHZ;
		} break;
		case 8: {
			BCSCTL1 = CALBC1_8MHZ;
			DCOCTL = CALDCO_8MHZ;
		} break;
		case 16: {
			BCSCTL1 = CALBC1_16MHZ;
			DCOCTL = CALDCO_16MHZ;
		} break;
		default: {

		} break;
	}
}

void Delay_ms(unsigned int ms){
	while(ms--){
		__delay_cycles(1000);
	}
}

//interrupts

//UART ISR
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	data = UCA0RXBUF;
	UART_sendTxt("Rx: ", 4);
	UART_sendTxt(&data, 1);
	UART_sendTxt("\n\r", 2);

	switch(data){
		case 'R': {
			P1OUT |= 0x01;
		} break;
		case 'r': {
			P1OUT &= ~0x01;
		} break;
		case 'q': {
			P1OUT ^= BIT0;
		} break;
		case '1': {
			P1OUT ^= BIT0;
			for(c = 0x00; c < 0x3f; c++){
				Set_TGT(c);
				for(i = 0; i < 0x0f; i++){
					UART_sendTxt("input: ", 7);
					UART_sendChar(i);
					UART_sendTxt("\r\n", 2);
					Transmit_char(i);
				}
			}
			UART_sendTxt("Done\r\n", 6);
			P1OUT ^= BIT0;
		} break;
		case '2': {
					P1OUT ^= BIT0;
					Set_TGT(0x27);
					Transmit();
		} break;
		case '3': {
					P1OUT ^= BIT0;
					Set_TGT(0x27);
					command(LCD_DISPLAYCONTROL | LCD_BLINKON);
//					write();
				} break;
		case '4': {
					P1OUT ^= BIT0;
					unsigned char stat;
					unsigned char conf;
					spi_CSL();
					//send command
					stat = spi_xfer_byte(NRF_CMD_RREG|NRF_REG_CONFIG);
					//read response
					conf = spi_xfer_byte(NRF_CMD_NOP);
					spi_CSH();
					UART_sendTxt("NRF registers\r\n", 15);
					printreg((char *)"status", stat);
					printreg((char *)"config", conf);
				} break;
		case '5': {
					P1OUT ^= BIT0;
					powerUp();
					Delay_ms(130);
					unsigned char conf = getRegister(_NRF24_RF_SETUP);
					printreg((char *)"conf", conf);
				} break;
		case '6': {
			spi_CSL();
		} break;
		case '0': {
			//software reset
			WDTCTL = WDTPW | WDTHOLD;
			asm ("push &0xfffe");
			asm ("ret");
		} break;
		default: {
			UART_sendTxt("?: ", 3);
			UART_sendTxt(&data, 1);
			UART_sendTxt("\n\r", 2);
		} break;
	}
}

//Possible I2C RX ISR
__interrupt void USCIAB0RX_ISR(void)
{
	P1OUT ^= BIT0;
	PRxData = UCB0RXBUF;						// Get RX data
	UART_puts((char *)PRxData);
	__bic_SR_register_on_exit(GIE);			// Exit LPM0
}

//I2C TX ISR
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	if (TXByteCtr)                            // Check TX byte counter
	{
		UCB0TXBUF = *PTxData++;                 // Load TX buffer
		TXByteCtr--;                            // Decrement TX byte counter
	}
	else
	{
		UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
		IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
		__bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
	}
	P1OUT ^= BIT0;
}

//Button ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	UART_sendTxt("Break\r\n", 7);
	P1IFG &= ~BIT3;                           // P1.4 IFG cleared
}
