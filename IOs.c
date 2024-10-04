#include "xc.h"
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"


int g_isSystemOn = 0; 
int g_isLEDOnPot = 0;
int g_isLEDBlinking = 0;

//// IOinit() with IO interrupts
void IOinit(void)
{
    REFOCONbits.ROEN = 1;   // Enable Reference Oscillator
    REFOCONbits.ROSEL = 0;  // Output base clock shows clock switching
    REFOCONbits.ROSSLP = 0; // Reference oscillator is disabled in sleep
    REFOCONbits.RODIV = 0b0000;
    TRISBbits.TRISB8 = 0;   // Set RB8 as output for Reference Oscillator

    TRISAbits.TRISA2 = 1;   // Set RA2 as Input
    TRISAbits.TRISA4 = 1;   // Set RA4 as Input
    TRISBbits.TRISB15 = 0;  // Set RB15 as Output

    CNPU1bits.CN0PUE = 1;   // Enables pull-up resistor on RA4/CN0
    CNPU2bits.CN30PUE = 1;  // Enables pull-up resistor on RA2/CN30

    CNEN2bits.CN30IE = 1;   // Enable Change Notification on RA2
    CNEN1bits.CN0IE = 1;    // Enable Change Notification on RA4

    LATBbits.LATB8 = 0;

    IEC1bits.CNIE = 1;      // Enable Change Notification interrupts
    IPC4bits.CNIP = 6;      // Set priority
    IFS1bits.CNIF = 0;      // Clear Change Notification flag
   
    return;
}

void IOcheck(void)
{
//    IEC1bits.CNIE = 0; //disable CN interrupts to avoid debounces
//    delay_ms(500);   // 400 msec delay to filter out debounces 
//    IEC1bits.CNIE = 1; //Enable CN interrupts to detect pb release
    
    if(PORTAbits.RA2 == 0) //If only RA2 pb is pressed (PB1)
    {
        g_isSystemOn = !g_isSystemOn;
        g_isLEDOnPot = !g_isLEDOnPot;
    }
    
    //Turn on blinking LED
    if(PORTAbits.RA4 == 0) //If only RA4 pb is pressed (PB2)
    {
        g_isSystemOn = !g_isSystemOn;
        g_isLEDBlinking = !g_isLEDBlinking;
    }
    
    return;

}

void toggleLED(unsigned int delayTime)
{    unsigned int x11 = 1023;
    int tmp = delayTime / 2; 
    Disp2Dec((float)x11);
    Disp2String("\n");
    delay_ms(tmp);
    LATBbits.LATB8 ^= 1; // Toggle LED on RB8
    delay_ms(tmp);
    Disp2Dec((float)0);
    Disp2String("\n");
    
}

void ledCheck(void) {
    if (g_isSystemOn == 1 && g_isLEDBlinking == 1) toggleLED(500);
    else if (g_isSystemOn == 1 && g_isLEDOnPot == 1) {
        NewClk(8);
        unsigned int ADCvalue;
        float adcPercent, timeOn, timeOff;

        ADCvalue = do_ADC();
        Disp2Dec((float)ADCvalue);
        Disp2String("\n");

        // Calculate ADC percentage
        adcPercent = (float)ADCvalue / 0x3FF;
        // Calculate on and off times based on ADC percentage
        timeOn = 15.0 * adcPercent;
        timeOff = 15.0 * (1 - adcPercent);

        // Control LED based on calculated times
        LATBbits.LATB8 = 1; // Turn on LED
        delay_ms((unsigned int)timeOn);

        LATBbits.LATB8 = 0; // Turn off LED
        delay_ms((unsigned int)timeOff);
        NewClk(32);
    } else {
        LATBbits.LATB8 = 0; // Ensure LED is off if system is not in correct state

        // Reset flags
        g_isSystemOn = 0;
        g_isLEDBlinking = 0;
        g_isLEDOnPot = 0;
    }
}

