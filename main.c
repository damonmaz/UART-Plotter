#include <xc.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"
#include "ADC.h"

// PRE-PROCESSOR DIRECTIVES - CONFIGURATION BITS

// Code protection 
#pragma config BSS = OFF // Boot segment code protect disabled
#pragma config BWRP = OFF // Boot sengment flash write protection off
#pragma config GCP = OFF // general segment code protecion off
#pragma config GWRP = OFF

// CLOCK CONTROL 
#pragma config IESO = OFF    // 2 Speed Startup disabled
#pragma config FNOSC = FRC  // Start up CLK = 8 MHz
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation
#pragma config POSCFREQ = MS  //Primary Oscillator/External clk freq betwn 100kHz and 8 MHz. Options: LS, MS, HS
#pragma config OSCIOFNC = ON  //CLKO output disabled on pin 8, use as IO. 
#pragma config POSCMOD = NONE  // Primary oscillator mode is disabled

// WDT
#pragma config FWDTEN = OFF // WDT is off
#pragma config WINDIS = OFF // STANDARD WDT/. Applicable if WDT is on
#pragma config FWPSA = PR32 // WDT is selected uses prescaler of 32
#pragma config WDTPS = PS1 // WDT postscler is 1 if WDT selected

//MCLR/RA5 CONTROL
#pragma config MCLRE = ON // RA5 pin configured as input, MCLR reset on RA5 diabled

//BOR  - FPOR Register
#pragma config BORV = LPBOR // LPBOR value=2V is BOR enabled
#pragma config BOREN = BOR0 // BOR controlled using SBOREN bit, BOR off
#pragma config PWRTEN = OFF // Powerup timer disabled
#pragma config I2C1SEL = PRI // Default location for SCL1/SDA1 pin

//JTAG FICD Register
#pragma config BKBUG = OFF // Background Debugger functions disabled
#pragma config ICS = PGx2 // PGC2 (pin2) & PGD2 (pin3) are used to connect PICKIT3 debugger

// Deep Sleep RTCC WDT
#pragma config DSWDTEN = OFF // Deep Sleep WDT is disabled
#pragma config DSBOREN = OFF // Deep Sleep BOR is disabled
#pragma config RTCOSC = LPRC// RTCC uses LPRC 32kHz for clock
#pragma config DSWDTOSC = LPRC // DeepSleep WDT uses Lo Power RC clk
#pragma config DSWDTPS = DSWDTPS7 // DSWDT postscaler set to 32768 


// MACROS for Idle, Sleep modes
#define Nop()                    \
    {                            \
        __asm__ volatile("nop"); \
    }
#define ClrWdt()                    \
    {                               \
        __asm__ volatile("clrwdt"); \
    }
#define Sleep()                        \
    {                                  \
        __asm__ volatile("pwrsav #0"); \
    } // set sleep mode
#define Idle()                         \
    {                                  \
        __asm__ volatile("pwrsav #1"); \
    }
#define dsen()                               \
    {                                        \
        __asm__ volatile("BSET DSCON, #15"); \
    }

void main(void) {
    
    NewClk(32);  // set clock to 32
    IOinit();    // Initializes IO
    InitUART2(); //Initialize UART settings and enable UART module

    while (1) // infinite while loop
    {  
        while (g_isSystemOn == 0){
            Disp2Dec((float)0);
            Disp2String("\n");
        }
        do {ledCheck();} while(g_isSystemOn == 1);
        Idle();
    }
    return 0;
    
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    IFS1bits.CNIF = 0; // clear IF flag
    IEC1bits.CNIE = 0; // disable interrupt
    IOcheck();
    IEC1bits.CNIE = 1; // enable interrupt
    Nop();
}