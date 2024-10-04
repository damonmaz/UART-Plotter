#ifndef IOS_H
#define IOS_H

#include "xc.h"

// Global variables
extern int g_isSystemOn; //1 if system is on, 0 if system is off
extern int g_isLEDOnPot; //1 if LED w/ potentiometer is on, 0 if off
extern int g_isLEDBlinking; //1 if LED is blinking, 0 if its not

extern uint8_t CNflag; 
void IOinit(void);
void IOcheck(void);
void toggleLED(unsigned int delayTime);
void ledCheck(void);


#endif /* IOS_H */


