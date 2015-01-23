#include "p33FJ64GS406.h"
#include "init.h"
#include <libq.h>
#include <math.h>

#include "SPI_COMM_BOARD_DRIVER.h"
#include "COMM_BOARD_INTERFACE.h"
#include "EnableModel.h"
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
    srand(0);
    set_communication_enable(0);
    init_clock();
    init_PWM();
    timer1_setup();
    ADC_init();
    initUART2();

    initializePorts();
    initSPICommBoard();

    // comms off to get a sample of all appliances off current.
    long long i;
    for(i=0; i<1000000LL; ++i);
    set_communication_enable(1);
    set_light_port_intensity(0.75);


    while(1){
        //set_light_port_intensity(0.5);
        //readingsUpdateStep();
    }
}
