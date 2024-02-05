
#include "wait.h"
/*!@file
 * Contains clock related functions
 */

void clock_setup(void)
{

    BCSCTL1 &= ~XT2OFF;
    do {
        IFG1 &= ~OFIFG; // Clear OSCFault flag
        int i;
        for (i = 0xff; i > 0; i--) { // Time for flag to set
            _NOP();
        }
    } while ((IFG1 & OFIFG) == OFIFG); // OSCFault flag still set?
    BCSCTL2 = SELM_2 + DIVM_0 + SELS + DIVS_0; // SMCLK = MCLK = XT2
    //  _BIC_SR(SCG0);  /* DCO off doesnt work for now, why? */

    SVSCTL = VLD9 + PORON; //sets SVS (Supply Voltage Supervisor) control register to 2.9V threshold (VLD_9);
        //PORON enables SVSFG flag to cause POR device reset
}

/*!
 * sets the system to wait for a given time. Starts a timer and sends the device to sleep until the timer expires.
 * Timer expiration triggers an interupt, ending the LPM3 started here.
 * The function can only be used for times greater than around 20ms. If the time is set to low the interrupt may trigger before the device enters the lowpoermode, leading to a non-functional pAMeter.
 * For shorter wating times the delay_cycles() function can be used. But it does not offer power saving while running.
 * @param ms wait time in miliseconds
 */
void wait(unsigned int ms) {
    WDTCTL=WDTPW+WDTHOLD; // stop watchdog
    CCTL0=CCIE;
    TACCR0=(unsigned int) (ms*32.768);
    TACTL=TASSEL_1 + MC_1 + ID_0;
    __bis_SR_register(LPM0_bits + GIE);
   return;//while(!(TACCTL0 & CCIFG));
}

/* Synchronous wait, must not be called during another asynchronous wait (also using TIMER A) */
/*!
 * Used to interrupt on expiration of timer A, which is used for the wait function. Ends the wait.
 */
#pragma vector=TIMERA0_VECTOR
__interrupt void timerA0_irq (void) {
    CCTL0 disable CCIE;
    __bic_SR_register_on_exit(LPM3_bits);
    CCTL0 set_low CCIFG;

}



/**/
/*!
 * Schedules the task func to be executed in ms miliseconds.Previously scheduled but not yet executed tasks are dropped.  Subjected to the same rules as wait()
 * @param func a function
 * @param ms time until execution
 */
void (*task)(void) = 0;
void schedule(void (*func)(void), unsigned int ms) {
	task = func;
    TBCCTL0=CCIE;
    TBCCR0=ms*32.768/2;
    TBCTL=TBSSEL_1 + MC_1 + ID_0;
    _BIS_SR(LPM3 + GIE);             // Enter LPM0 w/ interrupt
}

/*!
 * Used to interrupt on expiration of timer B, which is used for the schedule() function.
 */
/* IRQ for schedule */
#pragma vector=TIMERB0_VECTOR
__interrupt void timerB0_irq (void) {
	/* Hold timer */
	LPM3_EXIT;

	/* Execute scheduled task */
	(*task)();
}
/*!
 * Interrupt for a timeout of the internal watchdog timer. will lead to a restart of the pAMeter.
 */
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void) {
    timeout = 1;
}
