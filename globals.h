/*
 * globals.h
 *
 *  Created on: Nov 13, 2019
 *      Author: roessing
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_
/*!\file
 * This header contains important varaibles that need to be accessed from different parts of the firmware
 */
/* Numeric constants */
#define ADC_MAX_VAL 32767
#define ADC_MIN_VAL -32768
#define CALIB
#define NUM 128
#define SQRT_NUM 11.313708
#define RAW_NUM 32 //number of raw values per transmission
/*!
 * Determines the maximal length of a message that could be received.
 */
#define MESSAGE_LENGTH 25

/*!
 * Length of the messages containing the data. For details refer to pAMeter::readData() or the taking_data_short() functions.
 */

#define LENGTH (2 * sizeof(unsigned char) + 5 * sizeof(stat_t)) //2 char for mode and remaining TX; 2 stat_t for mean and error; \
    //3 additional stat_t for ADC measuring time, time until sending and temperature

// #define MAX_NOISE 0.01       /* Allow mode switching when Signal-to-noise less than 1 percent */

/* important global parameters*/
/*!
 * Filled by the message_decode() function together with DEST_ADDR_low
 */
extern volatile unsigned char DEST_ADDR_high;
extern volatile unsigned char DEST_ADDR_low;
extern volatile unsigned char message[MESSAGE_LENGTH];
/*!
 * Filled by the message_decode() function. Determines what data to be transmitted.
 */
extern volatile unsigned char dataID;
extern volatile unsigned char delay;
extern volatile unsigned char set_up;
/*!
 * Filled by the message_decode() function. Contains which mode the pAMeter should be running
 */
extern volatile unsigned char mode;
extern volatile int pos;
extern volatile unsigned int running;
volatile extern int timeout;
volatile extern signed short int mem[NUM];


#endif /* GLOBALS_H_ */
