
#include "usart.h"
/*!@file
 * Manages the control of the internal USART port, used to communicate with the XBee module.
 * For details on the USART control flow refer to
 */
/*!
 * Macro to write out a byte over the USART port, also calculates the checksum
 */
#define SEND(x) do { unsigned char _x = (x); while(!(U0TCTL & TXEPT)); TXBUF0 = _x; checksum -= _x; } while(0)

void usart_setup() {
	/* TX */
	//port3.dir.pin4 = IO_DIRPIN_OUTPUT;
    port3_dir set_as_output pin4;
	//port3.sel.pin4 = IO_ALTPIN_SELECT;
    port3_sel set_alt pin4;

	/* RX */
	port3_dir set_as_input pin5;
	port3_sel set_alt pin5;
	//UCTL0 |= SWRST;
    U0CTL set_high SWRST;            /* Block USART for programming */

	ME1 |= UTXE0 + URXE0;			// USART0 TX, RX enable
	U0CTL |= CHAR;	// 8 data bits, 1 stop bit, no parity (8N1)
	U0CTL set_low SYNC;
	U0TCTL |= 0x30;			// UCLK <- SMCLK

	/* clock: 1843200Hz
	   desired baud rate: 115200bps
	   division factor: 16
	   effective baud rate: 115200bps
	   maximum error: 0us   0.00%
	*/

	U0BR0=0x10; U0BR1=0x00; U0MCTL=0x00;	/* uart1 1843200Hz 115200bps */

	U0CTL &= ~SWRST;			/* USART freigeben */
	//IFG1 set_low URXIFG0;
	//U0TCTL enable URXSE;
	//IE1 enable URXIE0;
}

unsigned char checksum;

void usart_reset_checksum() {
	checksum = 0xff;
}

void usart_send_byte(unsigned char byte) {
	SEND(byte);
}

void usart_send_word_msb_first(unsigned int word) {
	char byte = word >> 8;
	SEND(byte);
	byte = word & 0x00ff;
	SEND(byte);
}

void usart_send_msg(volatile unsigned char *bytes, unsigned int length) {
	for(unsigned int i = 0; i < length; i++) {
		SEND(bytes[i]);
	}
}
