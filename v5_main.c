/*
 *
 *
 *  Created on: Nov 13, 2019
 *      Author: roessing
 */
#include <v5_main.h>

volatile unsigned char DEST_ADDR_high = 0;
volatile unsigned char DEST_ADDR_low = 0;
volatile unsigned char message[MESSAGE_LENGTH];
volatile unsigned char dataID = 0;
volatile unsigned char delay = 0;

volatile unsigned char set_up = 0;
volatile unsigned char mode = 0;
volatile unsigned int running = 0;
volatile int pos = -1;
volatile int timeout = 0;
volatile signed short int mem[NUM];

/*!
 * sets up the output ports and pins for startup of a pAMeter
 */
void port_setup()
{
    I2CIE set_low RXRDYIE;
    relay_setup();
    adc_setup();
    temperature_sensor_setup();
    xbee_setup();

    /* LED */
    P6DIR |= DEBUG_LED_BITS;

    /* Global interrupt enable, start watchdog*/
    _EINT();
    IE1 &= ~WDTIE;
    WDTCTL = WDTPW + WDTHOLD;

    /*Set 10 mA mode */
    set_mode(0);
    Reed_setup();
}

void debug_led_on()
{
    P6OUT |= DEBUG_LED_BITS;
}

void debug_led_off()
{
    P6OUT &= ~DEBUG_LED_BITS;
}

/*!
 * lets the led blink a set number of times
 * \param cycles number of blinks
 * \param time time between switching
 */
void blink_led(int cycles, int time)
{
    int j;
    for (j = cycles; j > 0; j--) {
        debug_led_on();
        wait(time);
        debug_led_off();
        wait(time);
    }
}

/*!
 * Controlls the program flow. A flow diagram can be found below. In contrast to the old firmware this one is optimized for a fast data taking.
 * The ADC is capable to do 500 ksamples per second.
 * The seperation in taking data long and taking data short is done to avoid a repetetive use of an if statement costing valuable computing time.
 *
 */
void main(void){
    running=1;
    WDTCTL = WDTPW + WDTHOLD; // stop watchdog for setup
    clock_setup();
    port_setup();
    usart_setup();
    //setup_temperature_sensor();
    debug_led_on();
    WDTCTL = WDTPW + WDTHOLD; // stop watchdog for setup
    MODE_RELAY_PORT_dir = 0xFF;


    IE1 enable URXIE0;
    U0TCTL enable URXSE;
    _EINT();

    WDTCTL = WDT_ARST_1000;

    IFG1 = 0;
    set_up = 0x00;
    SLEEP(0);

    /* Timer initialization for taking data acquisition time */
    TBCTL = CNTL_0 + TBSSEL_1 + ID_3 + MC_2; //Counter length 16bit, Clock ACLK, Divider 1/8, Mode control continuous
    while(!set_up){};//interrupted by usart interrupt
    while(1){
        if(dataID==0x00) taking_data_short();
        else taking_data_long();
    }
    debug_led_on();

}
/*!
 * regulates the data taking if only the mean values are to be transmitted (so dataFormat is 0), a flow diagram can be found below.
 * The function taking_data_long() is basically analog to this, just sends out the 128 intermediate values as well.
 */
void taking_data_short(){
    volatile unsigned char data[LENGTH];
    volatile int CYCLE_COUNTER=999;
    unsigned long int Counter_B_value;
    volatile stat_t temperature;
    int temp;
    while (1) {
        SLEEP(1);
            WDTCTL = WDT_ARST_1000; // watchdog now supervises correct measurement procedure
            /* Blinking, shows current mode*/
            if(CYCLE_COUNTER>=100/delay){
                int i;
                for (i = 0; i < mode; i++) {
                    WDTCTL = WDT_ARST_1000;
                    temperature_sensor_write(0x03, 0xA0, 1);
                    debug_led_on();
                    wait(300);
                    //WDTCTL = WDT_ARST_1000;
                    debug_led_off();
                    wait(300);
                }
                if (!(temp = temperature_sensor_read(0x00, 2)))
                        temperature=255;
                    if (temp > 0) {
                        temperature = temp / 128.;
                    } else {
                        temperature = (temp - 65536.) / 128.;
                    }
                CYCLE_COUNTER=0;
            }else{ wait(130*(delay-1)); }
//            port2_out set_low pin1;

            //wait(500);
            WDTCTL = WDT_ARST_1000;
            timeout = 0;

            //setting up timer B to measure the time of data aquisition
            TBCTL &= ~(MC_2); //stop counter (if it is accidentially running)
            TBR = 0; //set counter 0
            TBCTL |= MC_2; //start counter in continuos mode

            for (pos = 0; pos < NUM ; /* pos++ by ISR */) {
                WDTCTL = WDT_ARST_250;
                ADC_PORT_out set_high ADC_CNV;
                ADC_PORT_out set_low ADC_CNV;
                __delay_cycles(10);
                mem[pos++] = clock_bits(0x0000, 16);
            }
            WDTCTL = WDT_ARST_1000;

            /* Data taking finished now prepare and send data*/
            /* Stop counter after ADC has finished and calculate time */
            TBCTL &= ~(MC_2); //stop counter
            Counter_B_value = (unsigned long int)TBR; //write counter value to variable
            TBR = 0;
            TBCTL |= MC_2; //immediately start again for measurement of time until sending
            stat_t time_ADC = (stat_t)Counter_B_value / (stat_t)32768 * 8.0; //calculating time from the counted clock cycles

            WDTCTL = WDT_ARST_250;
            if (timeout) {
                /* ADC did not respond*/
                *((stat_t*)&data[0]) = NAN;
                *((stat_t*)&data[sizeof(stat_t)]) = NAN;
                *((unsigned char*)&data[2 * sizeof(stat_t)]) = mode;
                *((stat_t*)&data[2 * sizeof(stat_t) + sizeof(unsigned char)]) = 0; //measurement time
                *((unsigned char*)&data[4 * sizeof(stat_t) + sizeof(unsigned char)]) = temperature;
                *((unsigned char*)&data[5 * sizeof(stat_t) + 1 * sizeof(unsigned char)]) = 0; //remaining TX
            } else {
                WDTCTL = WDT_ARST_250;
                stat_t m, error;
                /* Mean */
                m = mean(mem, NUM);
                /* Error */
                error = sd(mem, m, NUM);

                *((stat_t*)&data[0]) = m;
                *((stat_t*)&data[sizeof(stat_t)]) = error;
                *((unsigned char*)&data[5 * sizeof(stat_t)]) = mode;//nolonger necessary
                *((stat_t*)&data[2 * sizeof(stat_t)]) = time_ADC; //ADC meas. time for 128 values. Time until sending calculated shortly before sending
                *((stat_t*)&data[4 * sizeof(stat_t)]) = temperature;
                *((unsigned char*)&data[5 * sizeof(stat_t) + 2 * sizeof(char)]) = 0;
            }
            WDTCTL = WDT_ARST_1000;
            if (!timeout) {
                /* Wake XBEE and wait with a timer for its resurrection (after 13.2ms for some reason) */
                SLEEP(0);
                //debug_led_on();
                wait(14);
                //debug_led_off();
                /* measure time before sending to reconstruct time of measuring events */
                TBCTL &= ~(MC_2); //stop counter
                Counter_B_value = (unsigned long int)TBR; //write counter value to variable
                stat_t time_send = (stat_t)Counter_B_value / (stat_t)32768 * 8.0; // *8 due to CLK divider of 1/8 (set by ID_3)
                *((stat_t*)&data[3 * sizeof(stat_t)]) = time_send; //ADC meas. time for 128 values

                /* TX request: (mean value | error | mode | time | number of remaining TXs) */
                tx_request(data, LENGTH, DEST_ADDR_high, DEST_ADDR_low);
                SLEEP(1); //next ADC reset follows at the beginning of the while(true) loop
            }
            CYCLE_COUNTER++;
        }
}

inline void taking_data_long(){
    volatile unsigned char data[LENGTH];
    volatile unsigned char rawdata1[RAW_NUM*sizeof(short)];
    volatile unsigned char rawdata2[RAW_NUM*sizeof(short)];
    volatile unsigned char rawdata3[RAW_NUM*sizeof(short)];
    volatile unsigned char rawdata4[RAW_NUM*sizeof(short)];
    volatile int CYCLE_COUNTER=0;
    unsigned long int Counter_B_value;
    volatile stat_t temperature;
    int temp;
    while (1) {
        WDTCTL = WDT_ARST_1000; // watchdog now supervises correct measurement procedure
      /* Blinking, shows current mode*/
        if(CYCLE_COUNTER>=16){
            int i;
            for (i = 0; i < mode; i++) {
                WDTCTL = WDT_ARST_1000;
                temperature_sensor_write(0x03, 0xA0, 1);
                debug_led_on();
                wait(300);
                debug_led_off();
                wait(300);
            }
            if (!(temp = temperature_sensor_read(0x00, 2)))
                temperature=255;
            if (temp > 0) {
                temperature = temp / 128.;
            } else {
                temperature = (temp - 65536.) / 128.;
            }
            CYCLE_COUNTER=0;
        }
        WDTCTL = WDT_ARST_1000;
        timeout = 0;
        WDTCTL = WDT_ARST_1000;
        //temperature = get_temperature();
        //get current values, takes approx 60ms per value times 128 values=8s
        //setting up timer B to measure the time of data aquisition
        TBCTL &= ~(MC_2); //stop counter (if it is accidentially running)
        TBR = 0; //set counter 0
        TBCTL |= MC_2; //start counter in continuos mode

        for (pos = 0; pos < NUM ; /* pos++ by ISR */) {
            WDTCTL = WDT_ARST_1000;
            ADC_PORT_out set_high ADC_CNV;
            ADC_PORT_out set_low ADC_CNV;
            mem[pos++] = clock_bits(0x0000, 16);
        }
        WDTCTL = WDT_ARST_1000;

        /* Data taking finished now prepare and send data*/
        /* Stop counter after ADC has finished and calculate time */
        TBCTL &= ~(MC_2); //stop counter
        Counter_B_value = (unsigned long int)TBR; //write counter value to variable
        TBR = 0;
        TBCTL |= MC_2; //immediately start again for measurement of time until sending
        stat_t time_ADC = (stat_t)Counter_B_value / (stat_t)32768 * 8.0; //calculating time from the counted clock cycles

        //temperature = (temperature + get_temperature()) / 2;

        WDTCTL = WDT_ARST_250;

        if (timeout) {
            /* ADC did not respond, happens on overflow or floating gates */
            *((stat_t*)&data[0]) = NAN;
            *((stat_t*)&data[sizeof(stat_t)]) = NAN;
            *((unsigned char*)&data[2 * sizeof(stat_t)]) = mode;
            *((stat_t*)&data[2 * sizeof(stat_t) + sizeof(unsigned char)]) = 0; //measurement time
            *((unsigned char*)&data[4 * sizeof(stat_t) + sizeof(unsigned char)]) = temperature;
            *((unsigned char*)&data[5 * sizeof(stat_t) + 1 * sizeof(unsigned char)]) = 0; //remaining TX
        } else {
            WDTCTL = WDT_ARST_250;
            stat_t m, error;
            /* Mean */
            m = mean(mem, NUM);
            /* Error */
            error = sd(mem, m, NUM);

            *((stat_t*)&data[0]) = m;
            *((stat_t*)&data[sizeof(stat_t)]) = error;
            *((unsigned char*)&data[5 * sizeof(stat_t)]) = mode;
            *((stat_t*)&data[2 * sizeof(stat_t)]) = time_ADC; //ADC meas. time for 128 values. Time until sending calculated shortly before sending
            *((stat_t*)&data[4 * sizeof(stat_t)]) = temperature;
            *((unsigned char*)&data[5 * sizeof(stat_t) + sizeof(char)]) = 4;
            //put raw values into 2 separate words
            /*Filling raw data arrays*/

            unsigned int k;
            for (k = 0; k < RAW_NUM; k++) {
                *((short*)&rawdata1[k * sizeof(short)]) = (short)((int)mem[k] - 32768);
                *((short*)&rawdata2[k * sizeof(short)]) = (short)((int)mem[k + RAW_NUM] - 32768);
                *((short*)&rawdata3[k * sizeof(short)]) = (short)((int)mem[k + 2 * RAW_NUM] - 32768);
                *((short*)&rawdata4[k * sizeof(short)]) = (short)((int)mem[k + 3 * RAW_NUM] - 32768);
            }
            /*Last byte of each data array denotes the number of remaining TXs*/
            *((unsigned char*)&rawdata1[RAW_NUM * sizeof(short)]) = 3;
            *((unsigned char*)&rawdata2[RAW_NUM * sizeof(short)]) = 2;
            *((unsigned char*)&rawdata3[RAW_NUM * sizeof(short)]) = 1;
            *((unsigned char*)&rawdata4[RAW_NUM * sizeof(short)]) = 0;
        }
        WDTCTL = WDT_ARST_1000;

        if (!timeout) {
            /* Wake XBEE and wait with a timer for its resurrection (after 13.2ms for some reason) */
            SLEEP(0);
            wait(14);
            /* measure time before sending to reconstruct time of measuring events */
            TBCTL &= ~(MC_2); //stop counter
            Counter_B_value = (unsigned long int)TBR; //write counter value to variable
            stat_t time_send = (stat_t)Counter_B_value / (stat_t)32768 * 8.0; // *8 due to CLK divider of 1/8 (set by ID_3)

            *((stat_t*)&data[3 * sizeof(stat_t)]) = time_send; //ADC meas. time for 128 values

            /* TX request: (mean value | error | mode | time | number of remaining TXs) */

            tx_request(data, LENGTH, DEST_ADDR_high, DEST_ADDR_low);
            tx_request(rawdata1, sizeof(rawdata1), DEST_ADDR_high, DEST_ADDR_low);
            tx_request(rawdata2, sizeof(rawdata2), DEST_ADDR_high, DEST_ADDR_low);
            tx_request(rawdata3, sizeof(rawdata3), DEST_ADDR_high, DEST_ADDR_low);
            tx_request(rawdata4, sizeof(rawdata4), DEST_ADDR_high, DEST_ADDR_low);
            SLEEP(1); //next ADC reset follows at the beginning of the while(true) loop
        }
        CYCLE_COUNTER++;
    }
}
