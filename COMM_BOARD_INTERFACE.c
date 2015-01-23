#include "COMM_BOARD_INTERFACE.h"
#include "SPI_COMM_BOARD_DRIVER.h"
#include "SensorManager.h"
#include "LoadManager.h"
#include "CommBoardMessageConsts.h"

void receiveMessageComm(int msg){

    switch(msg){

        case TURN_LIGHT_ON_MSG:
            turnLightOn();
            break;
        case TURN_LIGHT_OFF_MSG:
            turnLightOff();
            break;
        case TURN_PORT1_ON_MSG:
            turnPort1On();
            break;
        case TURN_PORT1_OFF_MSG:
            turnPort1Off();
            break;
        case TURN_PORT2_ON_MSG:
            turnPort2On();
            break;
        case TURN_PORT2_OFF_MSG:
            turnPort2Off();
            break;

        //Following cases are sensor values
        case GET_CURRENT_FROM_LOAD_BOARD_MSG: ;
            float currentReading = getCurrentSensorReading();
            sendPollFloatToComm(currentReading);
            //sendPollFloatToComm(9.5);
            break;
        case GET_LINE_VOLTAGE_FROM_LOAD_BOARD_MSG: ;
            float inputVoltage = getInputLineVolatage();
            sendPollFloatToComm(inputVoltage);
            break;            
        case GET_PORT1_VOLTAGE_FROM_LOAD_BOARD_MSG: ;
            float port1Voltage = getPort1Voltage();
            sendPollFloatToComm(port1Voltage);
            break;
        case GET_PORT2_VOLTAGE_FROM_LOAD_BOARD_MSG: ;
            float port2Voltage = getPort2Voltage();
            sendPollFloatToComm(port2Voltage);
            break;
        default:

           break;
    }

}








