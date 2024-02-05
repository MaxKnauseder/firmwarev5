/*
 * ltc2327.c
 *
 *  Created on: Nov 13, 2019
 *      Author: roessing
 */
#include "ltc2327.h"
#include "globals.h"

/*!@file
 * Provides controll functions for the LTC2327 ADC.
 * For details refer to ADCMANUAL
 */

/*!
 * Defining all the ports for the ADC
 */
void adc_setup()
{
/* Setting up Pins for LTC2327-ADC*/
/* Interrupt Pin*/
    ADC_IRQ_PORT_dir set_as_input ADC_IRQ_PIN;
    ADC_IRQ_PORT_ie disable ADC_IRQ_PIN; /* Start with disabled IRQ */
    ADC_IRQ_PORT_ies set_falling ADC_IRQ_PIN; /* Trigger on ADC-Busy high to low transition*/
/* Comunication Pins */
    ADC_PORT_out set_low ADC_SCLK;

    ADC_PORT_dir set_as_output ADC_SCLK;
    ADC_PORT_dir set_as_input ADC_DOUT;
    ADC_PORT_dir set_as_output ADC_DIN;
    ADC_PORT_out set_low ADC_CNV;
    ADC_PORT_dir set_as_output ADC_CNV;
    ADC_PORT_out set_low ADC_DIN;

}

/*!
 * reads data from the adc. An adc conversion is started by pulling ADC_CNV low. Conversion takes a few microseconds
 */

signed short int clock_bits(int c, int len){

    volatile short int result = 0;
    len=16;
    int i;
    for (i = len - 2; i >= 0; i--) {
        result |= ((ADC_PORT_in & ADC_DOUT)?1:0); // gets the status of the bit assigned to ADC_DOUT pin
        result = result << 1;
        ADC_PORT_out set_high ADC_SCLK;
        ADC_PORT_out set_low ADC_SCLK;
    }
    result |= ((ADC_PORT_in & ADC_DOUT)?1:0); // gets the status of the bit assigned to ADC_DOUT pin
    ADC_PORT_out set_high ADC_SCLK;
    ADC_PORT_out set_low ADC_SCLK;
    return result;
}

