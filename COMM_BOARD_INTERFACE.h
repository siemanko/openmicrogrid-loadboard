/* 
 * File:   COMM_BOARD_INTERFACE.h
 * Author: Dan
 *
 * Created on December 27, 2014, 4:47 PM
 */

#ifndef COMM_BOARD_INTERFACE_H
#define	COMM_BOARD_INTERFACE_H

#include <stdint.h>

void set_communication_enable(int enabled);

void receiveMessageComm(uint8_t msg);

void put_float_in_buffer(float f);

#endif	/* COMM_BOARD_INTERFACE_H */

