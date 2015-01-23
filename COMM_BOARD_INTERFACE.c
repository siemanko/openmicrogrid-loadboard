#include "COMM_BOARD_INTERFACE.h"
#include "SPI_COMM_BOARD_DRIVER.h"
#include "EnableModel.h"
#include "ReadingModel.h"
#include "CommBoardMessageConsts.h"

double output_current;
double network_voltage;
double output_voltage;
double phone_voltage;

void receiveMessageComm(int msg){

    switch(msg){

        case TURN_LIGHT_ON_MSG:
            set_light_port_intensity(0.5);
            break;
        case TURN_LIGHT_OFF_MSG:
            set_light_port_intensity(0.0);
            break;
        case TURN_PORT1_ON_MSG:
            set_port1(1);
            break;
        case TURN_PORT1_OFF_MSG:
            set_port1(0);
            break;
        case TURN_PORT2_ON_MSG:
            set_port2(1);
            break;
        case TURN_PORT2_OFF_MSG:
            set_port2(0);
            break;

        //Following cases are sensor values
        case GET_CURRENT_FROM_LOAD_BOARD_MSG: ;
            sendPollFloatToComm(output_current);
            //sendPollFloatToComm(9.5);
            break;
        case GET_LINE_VOLTAGE_FROM_LOAD_BOARD_MSG: ;
            sendPollFloatToComm(network_voltage);
            break;            
        case GET_PORT1_VOLTAGE_FROM_LOAD_BOARD_MSG: ;
            sendPollFloatToComm(output_voltage);
            break;
        case GET_PORT2_VOLTAGE_FROM_LOAD_BOARD_MSG: ;
            sendPollFloatToComm(phone_voltage);
            break;
        default:
           break;
    }

}








