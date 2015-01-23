#include "SensorManager.h"
#include "p33FJ64GS406.h"
#include <libq.h>
#include <dsp.h>
#include "init.h"

static int debug_mode;

void initSensorManager(int debugMode){
    debug_mode=debugMode;
}

float getCurrentSensorReading(){
    float sensorVal;
    if(!debug_mode){
        sensorVal = getOutputCurrent();
        //sensorVal = 7.5;
    }else{
        sensorVal = 18.2;        //Random Value
    }
    return sensorVal;     //I_S sense, current going into the load
}

float getInputLineVolatage(){
    float sensorVal;
    if(!debug_mode){
        sensorVal = -15*(ADCBUF0*3.3/1024-1.75)+30;
        //sensorVal = 7.5;
    }else{
        sensorVal = 18.9;        //Random Value
    }
    return sensorVal;     //I_S sense, current going into the load
}

float getPort1Voltage(){
    float sensorVal;
    if(!debug_mode){
        sensorVal = ADCBUF1*3.3/1024*(7.2);
    }else{
        sensorVal = 18.5;        //Random Value
    }
    return sensorVal;     //I_S sense, current going into the load
}

float getPort2Voltage(){
    float sensorVal;
    if(!debug_mode){
        sensorVal = ADCBUF2*3.3/1024*(69.0/22.0);
    }else{
        sensorVal = 18.6;        //Random Value
    }
    return sensorVal;     //I_S sense, current going into the load
}