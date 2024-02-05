/*
 * main.h
 *
 *  Created on: Nov 13, 2019
 *      Author: roessing
 */

#ifndef V5_MAIN_H_
#define V5_MAIN_H_

#include <globals.h>

#include <msp430f169.h>     //General header providing the compatibility with the msp430 microcontrollers
#include <io.h>
#include <iostructures.h>
#include <signal.h>

#include "stat.h"
#include "pAM-definitions.h" //contains all important port mappings
#include "wait.h"            //contains all time dependend functions

#include "ltc2327.h"        //introduces all functions for operating the LTC2327-ADC
#include "Relay.h"          //contains all relay related functions

#include "usart.h"          //provides the USART related functions
#include "xbee.h"           //provides an intrface to xbee and the setup function


#include "adt7410.h"        //provides an interface to the ADT7410 temperature sensor

void debug_led_on();
void debug_led_off();
void taking_data_short();
void taking_data_long();





#endif /* V5_MAIN_H_ */
