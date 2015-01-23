//EXPERIMENTAL

#ifndef INIT_H
#define	INIT_H

#include "p33FJ64GS406.h"
#include "dsp.h"
//#include "define.h"

void init_clock(void);
void Delay_ms(unsigned int delay);
void Delay_us(unsigned int delay);
void ADC_init(void);

#endif	/* INIT_H */