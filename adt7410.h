/*
 * adt7410.h
 *
 *  Created on: Oct 28, 2019
 *      Author: roessing
 */

#ifndef ADT7410_H_
#define ADT7410_H_

#include "stat.h"
void temperature_sensor_setup();
void tick();
void clock_out(char byte);
char clock_in();
int check_ack();
int temperature_sensor_write(char reg, int message, int length);
int temperature_sensor_read(int reg, int length);
void setup_temperature_sensor();
stat_t get_temperature();

#endif /* ADT7410_H_ */
