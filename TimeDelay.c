
#include "xc.h"
#include "TimeDelay.h"
#include "ChangeClk.h"
#include <stdio.h>

uint32_t MIN_TIMER_PERIOD = 0;


void configureTimer(uint32_t period, uint32_t clkVal) {
    NewClk(clkVal);  //Change clock speed
    T2CONbits.TON = 1;
    T2CONbits.TSIDL = 0;
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 0;
    IPC1bits.T2IP = 7;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
    PR2 = period; //Period to Idle
    Idle();
    NewClk(8);
}

void delay_ms(uint16_t time_ms) {
    if (time_ms == 0) {
        return; // Return immediately if delay is 0 ms
    }

    uint32_t timerPeriod = time_ms * 16; // Calculate timer period

    configureTimer(timerPeriod, 32);
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0;
    TMR2 = 0;
    return;
}
