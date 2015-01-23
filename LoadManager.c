#include "LoadManager.h"
#include "p33FJ64GS406.h"
#include <libq.h>
#include <dsp.h>

void initializePorts(){
    turnLightOff();
    turnPort1Off();
    turnPort2Off();
}

void turnLightOn(){
    PDC4 = 0.25*PTPER;
}

void turnLightOff(){
    PDC4 = 0;
}

void turnPort1On(){
    LATEbits.LATE7 = 1; // Turns on Port 1
}


void turnPort1Off(){
    LATEbits.LATE7 = 0; // Turns on Port 1
}

void turnPort2On(){
    LATGbits.LATG6 = 1;
}

void turnPort2Off(){
    LATGbits.LATG6 = 0;
}
