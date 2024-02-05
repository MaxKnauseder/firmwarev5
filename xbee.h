#ifndef __XBEE_H
#define __XBEE_H
#include "crc.h"
#include "pAM-definitions.h"
#include "usart.h"
#include "xbee_definitions.h"
#include "Relay.h"
#include <iostructures.h>
#include <msp430x16x.h>

void SLEEP(int i);

void xbee_setup();
void tx_request(volatile unsigned char* data, int length, volatile unsigned char address_high, volatile unsigned char address_low);
void message_decode(volatile unsigned char* message);
void rx_request(volatile unsigned char* message, unsigned int length);

#endif
