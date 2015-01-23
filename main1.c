#include "p33FJ64GS406.h"
#include "init.h"
#include <libq.h>
#include <math.h>

#include "SPI_COMM_BOARD_DRIVER.h"
#include "COMM_BOARD_INTERFACE.h"
#include "SensorManager.h"
#include "LoadManager.h"
#include "SerialCommunication.h"

extern unsigned int TimerInterruptCount;

#define DEBUG_MODE_ON           1
#define DEBUG_MODE_OFF          0

/* Configuration Bit Settings */
_FBS(BWRP_WRPROTECT_OFF);
_FGS(GSS_OFF&GCP_OFF&GWRP_OFF);
_FOSCSEL(FNOSC_FRC)
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE ) //it was CSECMDandON
_FWDT(FWDTEN_OFF)
_FPOR(FPWRT_PWR128 )
_FICD(ICS_PGD2 & JTAGEN_OFF)

void init_PWM(void);
void ADC_init (void);
void timer1_setup (void);

int main()
{
/* Configure Oscillator to operate the device at 40Mhz
	   Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
 	   Fosc= 7.37*(43)/(2*2)=80Mhz for Fosc, Fcy = 40Mhz */

	/* Configure PLL prescaler, PLL postscaler, PLL divisor */
	PLLFBD=41; 				/* M = PLLFBD + 2 */
	CLKDIVbits.PLLPOST=0;   /* N1 = 2 */
	CLKDIVbits.PLLPRE=0;    /* N2 = 2 */

        __builtin_write_OSCCONH(0x01);			/* New Oscillator FRC w/ PLL */
        __builtin_write_OSCCONL(0x01);  		/* Enable Switch */

	while(OSCCONbits.COSC != 0b001);		/* Wait for new Oscillator to become FRC w/ PLL */
        while(OSCCONbits.LOCK != 1);			/* Wait for Pll to Lock */

	/* Now setup the ADC and PWM clock for 120MHz
	   ((FRC * 16) / APSTSCLR ) = (7.37 * 16) / 1 = ~ 120MHz*/

	//delayDebug(200);

        ACLKCONbits.FRCSEL = 1;					/* FRC provides input for Auxiliary PLL (x16) */
	ACLKCONbits.SELACLK = 1;				/* Auxiliary Oscillator provides clock source for PWM & ADC */
	ACLKCONbits.APSTSCLR = 7;				/* Divide Auxiliary clock by 1 */
	ACLKCONbits.ENAPLL = 1;					/* Enable Auxiliary PLL */

	while(ACLKCONbits.APLLCK != 1);			/* Wait for Auxiliary PLL to Lock */

	init_PWM();
        timer1_setup();
        ADC_init();


        TRISEbits.TRISE7 = 0;    //make 12v enable an output
        TRISGbits.TRISG6 = 0;   //make 5v enable an output

        LATEbits.LATE7 = 1; // Turns on Fan
        LATGbits.LATG6 = 1; // Turns 5V on

        initializePorts();
        initSPICommBoard();
        initSensorManager(DEBUG_MODE_OFF);

        while(1){
            //readingsUpdateStep();                  
        }
}

void timer1_setup(){

    T1CONbits.TCKPS1 = 0;      //Timer1 Input Clock Prescale Select bits
    T1CONbits.TCKPS0 = 1;       // 01= divid by 8
    T1CONbits.TCS = 0;          //Use Internal clock
    T1CONbits.TGATE = 0;      //Gated time accumulation disabled
    T1CONbits.TON = 1;         //Starts 16-bit Timer1
}


void ADC_init ()
{
    ADCONbits.FORM    = 0;       // Integer data format
    ADCONbits.EIE     = 0;       // Early Interrupt disabled
    ADCONbits.ORDER   = 0;       // Convert even channel first
    ADCONbits.SEQSAMP = 0;       // Select simultaneous sampling
    ADCONbits.ADCS    = 5;       // ADC clock = FADC/6 = 120MHz / 6 = 20MHz,
				 // 12*Tad = 1.6 MSPS

    //IFS6bits.ADCP0IF  = 0;	 // Clear ADC interrupt flag pair 0
    //IPC27bits.ADCP0IP = 3;	 // Set ADC interrupt priority pair 0
    //IEC6bits.ADCP0IE  = 0;       // Enable the ADC pair 0 interrupt

    IFS6bits.ADCP1IF  = 0;	 // Clear ADC interrupt flag pair 1
    IPC27bits.ADCP1IP = 3;	 // Set ADC interrupt priority pair 1
    IEC6bits.ADCP1IE  = 1;       // Enable the ADC pair 1 interrupt

    ADPCFGbits.PCFG0 = 0; 	 // Primary Voltage
    ADPCFGbits.PCFG1 = 0; 	 // 5 volts
    ADPCFGbits.PCFG2 = 0; 	 // 12 volts out
    ADPCFGbits.PCFG3 = 0;         //current out

    //ADSTATbits.P0RDY   = 0; 	 // Clear Pair 0 data ready bit
    //ADCPC0bits.IRQEN0  = 1;      // Enable ADC Interrupt
    ADCPC0bits.TRGSRC0 = 12; 	 // ADC Pair 0 triggered by PWM4

    ADSTATbits.P1RDY   = 0; 	 // Clear Pair 1 data ready bit
    ADCPC0bits.IRQEN1  = 1;      // Enable ADC Interrupt for pair 1
    ADCPC0bits.TRGSRC1 = 12; 	 // ADC Pair 1 triggered by PWM4

    ADCONbits.ADON = 1;		 // Enable the ADC Module early for ADC Settling Time
}


void init_PWM()
{
    PTPER = 48100;             				/* PTPER = 1/(500kHz*1.04ns) = 1923, where 500 khz is freq
							of the PWM  and 1.04ns is PWM resolution. */
    /* ~~~~~~~~~~~~~~~~~~~~~~ PWM1 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  //  IOCON3bits.PENH = 1;   					/* PWM1H is controlled by PWM module */
    IOCON4bits.PENL = 1;   					/* PWM1L is controlled by PWM module */
    IOCON4bits.PMOD = 0;   					/* Select Complementary Output PWM mode */
    IOCON4bits.SWAP = 1;

    PDC4 = 0.1*PTPER;                 			/* PDC = (PTPER / DutyCycle)  */
    DTR4    = 30;               			/* Deadtime = (80ns / 1.04ns) where 65ns is desired deadtime */
    ALTDTR4 = 30;    						/* ALTDeadtime = (65ns / 1.04ns) where 65ns is desired deadtime */
    PHASE4 = 0;     			            /* No phase shift */

    TRGCON4bits.TRGDIV = 1000;       // Trigger interrupt generated once every 15 PWM cycle
    TRGCON4bits.TRGSTRT = 1;      // Trigger generated after waiting 0 PWM cycles

    PTCONbits.PTEN = 1;			   			/* Enable the PWM Module */

}