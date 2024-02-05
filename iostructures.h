/*
 * $Id: iostructures.h, Florian Rössing, Uni Bonn, 2019
 */
#ifndef __ASSEMBLER__

#ifndef __IOSTRUCTURES_H__
#define __IOSTRUCTURES_H__

#define pin0 BIT0
#define pin1 BIT1
#define pin2 BIT2
#define pin3 BIT3
#define pin4 BIT4
#define pin5 BIT5
#define pin5 BIT5
#define pin6 BIT6
#define pin7 BIT7

#define set_high |=
#define set_low &= ~
#define set_as_input set_low
#define set_as_output set_high
#define enable set_high
#define disable set_low
#define set_rising set_low
#define set_falling set_high
#define set_alt set_high
#define set_pin set_low

#define port1_in P1IN
#define port1_out P1OUT
#define port1_dir P1DIR
#define port1_ifg P1IFG
#define port1_ies P1IES
#define port1_ie P1IE
#define port1_sel P1SEL

#define port2_in P2IN
#define port2_out P2OUT
#define port2_dir P2DIR
#define port2_ifg P2IFG
#define port2_ies P2IES
#define port2_ie P2IE
#define port2_sel P2SEL

#define port3_in P3IN
#define port3_out P3OUT
#define port3_dir P3DIR
#define port3_sel P3SEL

#define port4_in P4IN
#define port4_out P4OUT
#define port4_dir P4DIR
#define port4_sel P4SEL

#define port5_in P5IN
#define port5_out P5OUT
#define port5_dir P5DIR
#define port5_sel P5SEL

#define port6_in P6IN
#define port6_out P6OUT
#define port6_dir P6DIR
#define port6_sel P6SEL

#ifdef __MSP430_PORT_INIT_
#undef __VOLATILE
#endif

#endif /* __IOSTRUCTURES_H__ */

#endif /* __ASSEMBLER__ */
