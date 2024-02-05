/*
 * adt7410.c
 *
 *  Created on: Oct 28, 2019
 *  Author: roessing
 */

#include "pAM-definitions.h"
#include "stat.h"
#include "wait.h"
#include <adt7410.h>
#include <iostructures.h>
#include <msp430x16x.h>

/*!
 * @file controls the ADT7410 temperature sensor. The sensor utilizes I2C for cummunication, the protocolls are implemented here.
 *  For details refer to the manual.
 */
/*!
 * setup the ports for the sensor
 */
void temperature_sensor_setup()
{
    /*Temprature sensor setup*/
    TEMP_PORT_dir set_as_output SCL;
    TEMP_PORT_out set_high SCL;
    TEMP_PORT_dir set_as_output SDA;
    TEMP_PORT_out set_high SCL;
}
/*!
 * set SCL state
 * @param status 1 for high, 0 for low
 */
void CLOCK(char status)
{
    do {
        if (status) {
            TEMP_PORT_out set_high SCL;
        } else {
            TEMP_PORT_out set_low SCL;
        }
    } while (0);
}



/*!
 * Used to transfer data to the ADT
 * @param byte data to be transfered
 */
void clock_out(char byte)
{
    unsigned int mask = 1 << (7);
    char bit;
    TEMP_PORT_dir set_as_output SDA;
    for (int i = 0; i < 8; i++) {
        bit = !!(byte & mask);
        CLOCK(0);
        if (bit) {
            TEMP_PORT_out set_high SDA;
        } else {
            TEMP_PORT_out set_low SDA;
        }
        CLOCK(1);

        mask >>= 1;
    }
}

/*!
 * used to read data from the ADT
 * @return a char representing the message received
 */
char clock_in()
{
    TEMP_PORT_dir set_as_input SDA;
    char data = 0;
    char mask = 1 << (7);
    for(int i = 0; i < 8; i++) {
        CLOCK(0);
        //wait(1);
        CLOCK(1);
        data += (TEMP_PORT_in & SDA) ? mask : 0;
        mask >>= 1;
    }
    return data;
}

/*!
 * checks if the ADT acknowledged the data transmission.
 * @return
 */
int check_ack()
{
    volatile int ack = 0;
    CLOCK(0);
    TEMP_PORT_dir set_as_input SDA;
    //wait(1);
    CLOCK(1);
    ack = TEMP_PORT_in & SDA;
    return ack;
}

/*!
 * Utilizes clock_out() to transfer data to the ADT
 * @param reg register to write to
 * @param message message to write to reg
 * @param length length of the message in bytes
 * @return status of the write
 * @retval 0 no success
 * @retval 1 successfull
 */
int temperature_sensor_write(char reg, int message, int length)
{
    unsigned char address = 0x90;
    unsigned char write = 0;
    char byte;

    //sending start signal
    TEMP_PORT_out set_low SDA;
    //sending data
    clock_out(address + write);
    if (check_ack())
        return 0;
    TEMP_PORT_out set_low SDA;
    clock_out(reg);
    if (check_ack())
        return 0;
    TEMP_PORT_out set_low SDA;
    for (int i = 1; i <= length; i++) {
        byte = message >> (length - 1) * 8;
        clock_out(byte);
        if (check_ack())
            return 0;
        TEMP_PORT_out set_low SDA;
    }
    //sending stop signal
    TEMP_PORT_dir set_as_output SDA;
    CLOCK(0);
    //wait(1);
    CLOCK(1);
    TEMP_PORT_out set_high SDA;

    return 1;
}

/*!
 * reads data from a register of the ADT
 * @param reg register to read from
 * @param length how many bytes to read
 * @return status of the write
 * @retval 0 no success
 * @retval 1 successfull
 */
int temperature_sensor_read(int reg, int length)
{
    int value = 0;
    unsigned char address = 0x90;
    const char read = 0x01;
    const char write = 0x00;
    //send start signal
    TEMP_PORT_out set_low SDA;
    //send address
    clock_out(address + write);
    if (check_ack())
        return 0;
    clock_out(reg);
    if (check_ack())
        return 0;
    //send start signal again
    CLOCK(0);
    TEMP_PORT_out set_high SDA;
    CLOCK(1);
    TEMP_PORT_out set_low SDA;
    clock_out(address + read);
    if (check_ack())
        return 0;
    TEMP_PORT_out set_low SDA;
    for (int i = 1; i <= length; i++) {
        value += clock_in();
        if (i < length) {
            value = value << 8;
            CLOCK(0);
            TEMP_PORT_out set_low SDA;
            TEMP_PORT_dir set_as_output SDA;
            CLOCK(1);
            CLOCK(0);
        } else {
            //no ack by master
            TEMP_PORT_dir set_as_output SDA;
            CLOCK(0);
            TEMP_PORT_out set_high SDA;
            CLOCK(1);
            //wait(1);
            CLOCK(0);
            //send stop
            TEMP_PORT_out set_low SDA;
            CLOCK(1);
            TEMP_PORT_out set_high SDA;
        }
    }
    return value;
}


void setup_temperature_sensor()
{
    temperature_sensor_write(0x03, 0xA0, 1);
}

/*!
 * Used to read a temperature value
 * @return the temperature measured as a float
 */
stat_t get_temperature()
{
    volatile stat_t temperature;
    int temp;
    temperature_sensor_write(0x03, 0xA0, 1);
    wait(250);
    if (!(temp = temperature_sensor_read(0x00, 2)))
        return 255;
    if (temp > 0) {
        temperature = temp / 128.;
    } else {
        temperature = (temp - 65536.) / 128.;
    }
    return temperature;
}
