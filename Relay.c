/*
 * Relay.c
 *
 *  Created on: Nov 13, 2019
 *      Author: roessing
 */

#include "Relay.h"
#include "globals.h"

void relay_setup(){
/* supply voltage relay pins */
    RELAY_PORT_dir set_as_output SET_PLUS9V_PIN;
    RELAY_PORT_dir set_as_output RESET_PLUS9V_PIN;
    RELAY_PORT_dir set_as_output SET_MINUS9V_PIN;
    RELAY_PORT_dir set_as_output RESET_MINUS9V_PIN;

    RELAY_PORT_out = 0x00;


/* Mode relays */
    RELAY_PORT_dir set_as_output SET_5M_PIN;
    RELAY_PORT_dir set_as_output RESET_5M_PIN;
    RELAY_PORT_dir set_as_output SET_50M_PIN;
    RELAY_PORT_dir set_as_output RESET_50M_PIN;
    RELAY_PORT_dir set_as_output SET_500M_PIN;
    RELAY_PORT_dir set_as_output RESET_500M_PIN;
    RELAY_PORT_dir = 0xFF;
    MODE_RELAY_PORT_out = 0x00;
      //all Relay Pins are Output

    port5_dir set_as_output pin4;
// SET supply voltage relays for 10ms
    SET_PLUS9V_RELAY(1);
    wait(10);
    SET_PLUS9V_RELAY(0);

    RESET_MINUS9V_RELAY(1);
    wait(10);
    RESET_MINUS9V_RELAY(0);
}

void Reed_setup(){
/* Reed Switch */
    REED_PORT_dir set_as_input REED_PIN;
    REED_PORT_ifg = 0x00;
    REED_PORT_ie enable REED_PIN;
    REED_PORT_ies set_rising REED_PIN;
    REED_PORT_ifg set_low REED_PIN;
}


void set_mode(unsigned char mode){
            WDTCTL=WDTPW+WDTHOLD;
            //blink_led(mode,1000);
            MODE_RELAY_PORT_dir = 0xFF;

            switch(mode) {
                case 0: /* Exit 10 mA mode, enter 100 uA mode */
                    //RESET_100R(1);
                    mode=0;
                    int i;
                    for(i=0;i<600;i++);
                    MODE_RELAY_PORT_out set_high RESET_500M_PIN;
                    MODE_RELAY_PORT_out set_high RESET_50M_PIN;
//                    for(int i=0;i<600;i++);
//                    MODE_RELAY_PORT_out set_low RESET_1G_PIN;
//                    MODE_RELAY_PORT_out set_low RESET_10G_PIN;
                    MODE_RELAY_PORT_out set_high SET_5M_PIN;
                    break;
                case 1: /* Exit 100 uA mode, enter 1 uA mode */
                    mode=1;
                    for(i=0;i<600;i++);
                    MODE_RELAY_PORT_out set_high RESET_500M_PIN;
                    MODE_RELAY_PORT_out set_high SET_50M_PIN;
                    MODE_RELAY_PORT_out set_high RESET_5M_PIN;
                    break;
                case 2: /* Exit 1 uA mode, enter 10 nA mode */
                    mode=2;
                    for(i=0;i<600;i++);

                    MODE_RELAY_PORT_out set_high RESET_5M_PIN;
                    MODE_RELAY_PORT_out set_high RESET_50M_PIN;
//                    for(int i=0;i<600;i++);
//                    MODE_RELAY_PORT_out set_low RESET_100M_PIN;
//                    MODE_RELAY_PORT_out set_low RESET_1G_PIN;

                    MODE_RELAY_PORT_out set_high SET_500M_PIN;
                    for(i=0;i<600;i++);

                    break;
                case 4:
                    mode=3;
                    for(i=0;i<600;i++);

                    MODE_RELAY_PORT_out set_high RESET_5M_PIN;
                    MODE_RELAY_PORT_out set_high RESET_50M_PIN;
                    //for(int i=0;i<600;i++);
                    //MODE_RELAY_PORT_out set_low RESET_100M_PIN;
                    //MODE_RELAY_PORT_out set_low RESET_1G_PIN;

                    MODE_RELAY_PORT_out set_high RESET_500M_PIN;
                    for(i=0;i<600;i++);

                    break;
            }
            int i;
            for(i=0;i<600;i++);
            MODE_RELAY_PORT_out = 0x00;
            WDTCTL=WDT_ARST_1000;
}



void activate_relay_irq(void) {
    REED_PORT_ifg set_low REED_PIN;
    REED_PORT_ie enable REED_PIN;
}

/* reed contact irq */
#pragma vector=REED_PORT_VECTOR
__interrupt void port1_isr(void) {
    if (!!(REED_PORT_ifg & REED_PIN)) {
        WDTCTL=WDTPW+WDTHOLD;
        REED_PORT_ie disable REED_PIN;
        timeout = 1;
        mode=0;
        //blink_led(mode,1000);
        if (running){
            /* Cut off power, immediately */
            running=0;
            debug_led_off();
            SLEEP(1);
            //blink_led(1,5000);
           _EINT();
            SET_MINUS9V_RELAY(1);
            wait(20);
            SET_MINUS9V_RELAY(0);
            wait(20);
            _DINT();
            RESET_PLUS9V_RELAY(1);
            _BIS_SR(LPM4+GIE);
        }
        _EINT();
        wait(20);
        MODE_RELAY_PORT_out = 0x00;
        //wait(500);
        //activate_relay_irq();
        schedule(activate_relay_irq, 300);
    }
    REED_PORT_ifg = 0x00;
    WDTCTL=WDT_ARST_1000;
}
