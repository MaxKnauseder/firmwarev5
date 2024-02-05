#ifndef _PAM_DEFINITIONS_H
#define _PAM_DEFINITIONS_H

#include <signal.h>
#include <io.h>
#include <iostructures.h>
#include <msp430.h>
#include <stdint.h> // contains definition of uint16_t
/*!
 * @file includes various definitions for ports, pins and registers to simplify the program code.
 */
/*!
 * The Revision of the device, to ensure proper readout of data and correct application of calibration
 */
#define REVISION 5
/* Configuration register */
#define	nWEN		0x80

#define	RS1		0x20
#define	RS0		0x10

#define RS_COMM		0
#define RS_STATUS	0
#define RS_MODE		RS0
#define RS_FILTER	RS1
#define RS_DATA		(RS1 | RS0)

#define	READ		0x08
#define	WRITE		0x00

#define	CREAD		0x04

#define	CH1		0x02
#define	CH0		0x01

#define	CH_IN		0
#define CH_SHORT	CH1
#define CH_VDD		(CH1 | CH0)

/* Mode register */
#define	MD1		0x80
#define	MD0		0x40
#define	G1		0x20
#define	G0		0x10
#define	BO		0x08
#define	BUF		0x02

#define MD_CONTINUOUS	0
#define MD_SINGLE	MD1
#define	MD_POWERDOWN	(MD0 | MD1)

#define G_FULLRANGE	0
#define	G_HALFRANGE	G0
#define	G_QUARTERRANGE	G1
#define	G_EIGHTRANGE	(G0 | G1)

/* Filter register */
#define	CDIV1		0x20
#define CDIV0		0x10
#define	FS2		0x04
#define	FS1		0x02
#define	FS0		0x01

#define CLKDIV_1	0
#define CLKDIV_2	CDIV0
#define CLKDIV_4	CDIV1
#define CLKDIV_8	(CDIV0 | CDIV1)

#define	FS_120		(0	| 0	| 0	)
#define	FS_100		(0	| 0	| FS0	)
#define	FS_33_3		(0	| FS1	| 0	)
#define	FS_20		(0	| FS1	| FS0	)
#define	FS_16_6		(FS2	| 0	| 0	)
#define	FS_16_7		(FS2	| 0	| FS0	)
#define	FS_13_3		(FS2	| FS1	| 0	)
#define	FS_9_5		(FS2	| FS1	| FS0	)

#define VLD9 0x90
/******iNTERUPT VECTORS******/
#define ADC_PORT_VECTOR PORT2_VECTOR
#define REED_PORT_VECTOR PORT1_VECTOR

////////////////////////////////////////////////////// Port Mapping ////////////////////////////////////////////////////////////////////
/*!@file
 * The pins of the MSP430 are grouped in ports with 8 pins each. So some ports are used for different purposes.
 * To ensure readability of the software they have an extra name for each purpose. Ports 1 and 2 have interupt capability and hence more registers to be defined.
 * For details on this refer to the @MSP430MANUAL
 */
/******Port1 definitions****/
#define REED_PORT_in    port1_in
#define REED_PORT_dir   port1_dir
#define REED_PORT_ie    port1_ie
#define REED_PORT_ies   port1_ies
#define REED_PORT_ifg   port1_ifg

#define RELAY_PORT_in   port1_in
#define RELAY_PORT_out  port1_out
#define RELAY_PORT_dir  port1_dir
#define RELAY_PORT_ie   port1_ie
#define RELAY_PORT_ies  port1_ies
#define RELAY_PORT_ifg  port1_ifg

/******Port 2 definitions****/
#define ADC_IRQ_PORT_in  port2_in
#define ADC_IRQ_PORT_out port2_out
#define ADC_IRQ_PORT_dir port2_dir
#define ADC_IRQ_PORT_ie  port2_ie
#define ADC_IRQ_PORT_ies port2_ies
#define ADC_IRQ_PORT_ifg port2_ifg

#define ADC_PORT_in   port2_in
#define ADC_PORT_out  port2_out
#define ADC_PORT_dir  port2_dir

#define TEMP_PORT_in   port2_in
#define TEMP_PORT_out  port2_out
#define TEMP_PORT_dir  port2_dir


/******Port 3 definitions****/
#define XBEE_SLEEP_PORT_in  port3_in
#define XBEE_SLEEP_PORT_out port3_out
#define XBEE_SLEEP_PORT_dir port3_dir



#define XBEE_PORT_in   port3_in
/*#define XBEE_PORT_out  port3_out
#define XBEE_PORT_dir  port3_dir*/

/******Port 4 definitions****/
#define MODE_RELAY_PORT_in   port4_in
#define MODE_RELAY_PORT_out  port4_out
#define MODE_RELAY_PORT_dir  port4_dir

/******Port 5 definitions****/
#define SVS_PORT_in     port1_in
#define SVS_PORT_dir    port1_dir
#define SVS_PORT_ie     port1_ie
#define SVS_PORT_ies    port1_ies
#define SVS_PORT_ifg    port1_ifg



/******Port 6 definitions****/
#define DEBUG_LED_PORT_in  port6_in
#define DEBUG_LED_PORT_out port6_out
#define DEBUG_LED_PORT_dir port6_dir

////////////////////////////////////////////////////// PIN Mapping ////////////////////////////////////////////////////////////////////
/******Port1 Pins****/
#define SET_PLUS9V_PIN      pin3
#define RESET_PLUS9V_PIN    pin4
#define REED_PIN            pin5
#define RESET_MINUS9V_PIN   pin2
#define SET_MINUS9V_PIN     pin1

/******Port2 Pins****/
#define ADC_IRQ_PIN     pin0    /* Not Connected on Board */
#define ADC_SCLK        pin3    /* P3.3 SCLK */
#define ADC_DOUT        pin2    /* P3.2 SOMI */
#define ADC_DIN         pin1    //* Not Connected on Board */
#define ADC_CNV         pin4    /* P3.0 */

#define SDA             pin6
#define SCL             pin7


/******Port3 Pins****/
#define XBEE_SLEEP  pin3
#define XBEE_DOUT   pin5
#define XBEE_DIN    pin4

/******Port4 Pins****/
#define   RESET_500M_PIN       pin6
#define   SET_500M_PIN         pin7
#define   RESET_50M_PIN        pin4
#define   SET_50M_PIN          pin5
#define   RESET_5M_PIN      pin2
#define   SET_5M_PIN        pin3
//#define   SET_100R_PIN      pin4
//#define RESET_100R_PIN      pin5



/******Port5 Pins****/
#define SVS_PIN         pin5


/******Port6 Pins****/
#define DEBUG_LED_PIN1      pin3
#define DEBUG_LED_PIN2      pin4
#define DEBUG_LED_BITS      ((1 << 5) | (1 << 6)| (1 << 7))



#endif
