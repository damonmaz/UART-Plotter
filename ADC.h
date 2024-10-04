/* 
 * File:   ADC.h
 * Author: rvyas
 *
 * Created on Oct 30, 2020, 9:07 PM
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

void ADC_init(void);
unsigned int do_ADC(void);
// unsigned int do_ADCseq(void);
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void);

void DispADC(void); 


#endif	/* ADC_H */

