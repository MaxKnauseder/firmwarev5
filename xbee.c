/*!@file
 * This file contains all functions used to cummunicate with the xbee device, for details on the device functionality consult the  \XBeeMANUAL.
 */
#include "xbee.h"
#include "globals.h"
#include "v5_main.h"
unsigned int success=0;
/*!
 * The XBee module sleeps when the sleep pin is pulled high.
 * \param i if 1 the device is send to sleep, if 0 its waken up, wakeup takes 13 ms
 */
void SLEEP(int i)
{
/*enables/disables the xbee*/
    if (i) {
        P2OUT set_high pin0;
    } else {
        P2OUT set_low pin0;
    }
}
void xbee_setup()
{
//setup xbee port
    XBEE_SLEEP_PORT_dir set_as_output XBEE_SLEEP;
/* Start sleeping! */
    SLEEP(1);
}

unsigned char frame_id = 0;

/*! Handles the sending process of the pAMeters, this function is the counterpart to pAMeter::TX()  from the PAMOS software.
 * The pAMeter will always send mode and revision as the first two bytes of data! on the  receiver side these are read and into the corresponding pAMmessage.
 * \imageSize{TX.png,width:900px;,a TX api frame}
 */
void tx_request(volatile unsigned char* data, int length, volatile unsigned char address_high, volatile unsigned char address_low)
{

    if (length + 9 > 255) {
        return;
    }

    success=0;
    //do{
    usart_send_byte(START_DELIMITER);
    usart_send_word_msb_first(length + 7);

    usart_reset_checksum();

    usart_send_byte(API_TX16);
    usart_send_byte(frame_id++);
    usart_send_byte(address_high);
    usart_send_byte(address_low);
    usart_send_byte(0x00);
    usart_send_byte(mode);
    usart_send_byte(REVISION);
    usart_send_msg(data, length);
    usart_send_byte(checksum);
//    unsigned int count=0;
//    unsigned char txstatus[7];
//    WDTCTL = WDTPW + WDTHOLD;
//    U0TCTL enable URXSE;
//    SLEEP(0);
//    do {
//            while ((IFG1 & URXIFG0) == 0){}
//            txstatus[count] = U0RXBUF;
//            count++;
//        } while (count < 7);
//    if(txstatus[0]==0x7E){
//        unsigned int length=(txstatus[1]<<8)+txstatus[2];
//        if(length==3){
//            if(txstatus[3]==0x89){
//                if(txstatus[5]==0){
//                    success=1;
//                }
//            }
//        }
//   }
//
//        port2_ie=0;
//        port2_dir = 0xFF;
//    IE1 enable URXIE0;
//    port2_out = 0xFF;
//    wait(10);
//    port2_out set_low pin1;
//   }while(!success);
}

/*!
 * Used to decode a received message send by the PAMOS. The counterpart for this is the pAMeter::send_init() function, always change these together.
 * @param message an array containing a message received via the xbee
 */
void message_decode(volatile unsigned char* message)
{
    unsigned int i;
    for (i = 0; i < MESSAGE_LENGTH; i++) {
        if (message[i] == 254) {
            DEST_ADDR_high = message[i + 1]; //0x00;// << 8;
            DEST_ADDR_low = message[i + 2]; //0x02;
        }
        if (message[i] == 253) {
            mode = message[i + 1];
            dataID = message[i + 2];
            delay = message[i+3];
        }
    }
}
/*!
 * Receive an xbee message after an usart interrupt. Utilizes flagpoling to read the next byte from the U0RXBUF and counts the message length.
 * @param message an array to save the received message in it
 * @param length length of message, is 256. Used to limit the reading to a message of this length
 *
 *sends out a confirm message to verify a correct setup, evaluated by the pAMeter::initiate() function from PAMOS
 */
void rx_request(volatile unsigned char* message, unsigned int length)
{
    WDTCTL = WDTPW + WDTHOLD;
    U0TCTL enable URXSE;
    SLEEP(0);
    unsigned int count = 0;
    do {
        while ((IFG1 & URXIFG0) == 0){}
        message[count] = U0RXBUF;
        count++;
    } while (count < length && message[count - 2] != 0xFF);
    message_decode(message); //, DEST_ADDR_high,  DEST_ADDR_low);
    volatile unsigned char confirm[] = { mode, dataID, REVISION,delay, 0xFF };
    set_up = 1;
    tx_request(confirm, sizeof(confirm), DEST_ADDR_high, DEST_ADDR_low);

}

/** interrupt routine triggered when there is a signal received on the usart port, which is connected to the XBee. Therefore this is used for receiving the initiation frame
 *
 */
#pragma vector = USART0RX_VECTOR
__interrupt void setup_isr(void)
{
    if(!set_up){
        IE1 disable URXIE0;
        WDTCTL = WDTPW + WDTHOLD;
        rx_request(message, sizeof(message));
        set_mode(mode);
        debug_led_off(); //signals that setup was successful
        IFG1 = 0;
        set_up = 1;
        WDTCTL = WDT_ARST_1000;
    /*disable interupt to fix device in current setup, enabling interrupt would lead to malfunction*/
    }
//    }else{
//        success=1;
//        __bic_SR_register_on_exit(LPM3_bits);
//    }
    __bic_SR_register_on_exit(LPM3_bits);
    IE1 disable URXIE0;
    U0TCTL disable URXSE;
    SLEEP(1);

}
