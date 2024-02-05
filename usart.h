#ifndef __CRC_H
#define __CRC_H

#include <msp430x16x.h>
#include "crc.h"
#include <iostructures.h>
#include "pAM-definitions.h"

extern unsigned char checksum;

void usart_setup();
void usart_reset_checksum();
void usart_send_byte(unsigned char byte); 
void usart_send_word_msb_first(unsigned int word);
void usart_send_msg(volatile unsigned char *bytes, unsigned int length);

#endif
