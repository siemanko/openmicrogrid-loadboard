//EXPERIMENTAL

#include "p33FJ64GS406.h"
#include <libq.h>
#include <dsp.h>

#include "init.h"
#include "ReadingModel.h"

unsigned int TimerInterruptCount = 0;

#define refv 2.1f

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt()
{

  TimerInterruptCount ++; 	// Increment interrupt counter
  IFS0bits.T1IF = 0; 		// Clear Interrupt Flag
}

void __attribute__((__interrupt__, no_auto_psv)) _ADCP1Interrupt()
{

    double current_reading = (ADCBUF3*3.3/1024*(20.0/22.0));
    double network_voltage = -15*(ADCBUF0*3.3/1024-1.75)+30;
    double output_voltage = ADCBUF1*3.3/1024*(7.2);
    double phone_voltage = ADCBUF2*3.3/1024*(69.0/22.0);

    on_output_current_reading(current_reading);
    on_network_voltage_reading(network_voltage);
    on_output_voltage_reading(output_voltage);
    on_phone_voltage_reading(phone_voltage);

    IFS6bits.ADCP1IF = 0; //clear interrupt flag
    ADSTATbits.P1RDY = 0; //Clear ready bit

}




