#include "COMM_BOARD_INTERFACE.h"
#include "SPI_COMM_BOARD_DRIVER.h"
#include "EnableModel.h"
#include "ReadingModel.h"
#include "CommBoardMessageConsts.h"

static int communications_on = 0;

int next_index = 0;
uint8_t buffer[4];


void set_communication_enable(int enabled) {
    communications_on = enabled;
}


uint8_t receiveMessageComm(uint8_t msg){
    if(communications_on == 0) return 132;
    switch(msg){
        case LOAD_REQUEST_DATA:
            if (next_index < 4) {
                return buffer[next_index++];
            } else {
                // should never happen.
                return 0;
            }
            break;
        case TURN_LIGHT_ON_MSG:
            set_light_port_intensity(0.75);
            return 0;
            break;
        case TURN_LIGHT_OFF_MSG:
            set_light_port_intensity(0.0);
            return 0;
            break;
        case TURN_PORT1_ON_MSG:
            set_port1(1);
            return 0;
            break;
        case TURN_PORT1_OFF_MSG:
            set_port1(0);
            return 0;
            break;
        case TURN_PORT2_ON_MSG:
            set_port2(1);
            return 0;
            break;
        case TURN_PORT2_OFF_MSG:
            set_port2(0);
            return 0;
            break;

        //Following cases are sensor values
        case GET_CURRENT_FROM_LOAD_BOARD_MSG: ;
            put_float_in_buffer(get_output_current());
            return 0;
            //sendPollFloatToComm(9.5);
            break;
        case GET_LINE_VOLTAGE_FROM_LOAD_BOARD_MSG: ;
            put_float_in_buffer(get_network_voltage());
            return 0;
            break;            
        case GET_PORT1_VOLTAGE_FROM_LOAD_BOARD_MSG: ;
            put_float_in_buffer(get_output_voltage());
            return 0;
            break;
        case GET_PORT2_VOLTAGE_FROM_LOAD_BOARD_MSG: ;
            put_float_in_buffer(get_phone_voltage());
            return 0;
            break;
        default:
           break;
    }

}


void put_float_in_buffer(float f) {
    uint8_t* ptr = (uint8_t*)&f;
    int i;
    for (i=0; i<4; ++i) {
        buffer[i] = *ptr;
        ptr += 1;
    }
    next_index = 0;
}







