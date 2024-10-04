#ifndef TIMEDELAY_H
#define TIMEDELAY_H

#include <stdint.h>  // For uint16_t

// Function to delay for the specified number of milliseconds
void delay_ms(uint16_t time_ms);

// Configure the timer with a specific period
void configureTimer(uint32_t period, uint32_t clkVal);

// Timer2 interrupt handler
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);

#endif // TIMEDELAY_H
