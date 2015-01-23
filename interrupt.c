//EXPERIMENTAL

#include "p33FJ64GS406.h"
#include <libq.h>
#include <dsp.h>
#include "init.h"

unsigned int TimerInterruptCount = 0;

#define refv 2.1f

float ADCvalue0;
float ADCvalue1;
float ADCvalue2;
int led_count = 0;
float old_current_value  = 0.0;

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt()
{

  TimerInterruptCount ++; 	// Increment interrupt counter
  IFS0bits.T1IF = 0; 		// Clear Interrupt Flag
}

void __attribute__((__interrupt__, no_auto_psv)) _ADCP1Interrupt()
{

    //One of these input current, one of these input current, one input voltage
    //(from the grid)
    //ADCvalue1=ADCBUF2*3.3/1024;     // 5 out sense
    //ADCvalue2=ADCBUF3*3.3/1024;
    //ADCvalue1=ADCBUF2*3.3/1024;        //I_S sense, current going into the load
    //float ADCvalue3 = ADCBUF1*3.3/1024;   //12V out sense
    //ADCvalue0 = ADCBUF0;   //input voltage sense

    float new_output_current = (0.9*old_current_value) + 0.1*(ADCBUF3*3.3/1024*(20.0/22.0));
    setOutputCurrent(new_output_current);
    old_current_value = new_output_current;


    IFS6bits.ADCP1IF = 0; //clear interrupt flag
    ADSTATbits.P1RDY = 0; //Clear ready bit

}




