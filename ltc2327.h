/*
 * ltc2327.h
 *
 *  Created on: Nov 13, 2019
 *      Author: roessing
 */

#ifndef LTC2327_H_
#define LTC2327_H_

#include "pAM-definitions.h"
#include <math.h>

void adc_setup();
signed short int clock_bits(int c, int len);

#endif /* LTC2327_H_ */
