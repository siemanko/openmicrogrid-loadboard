/* 
 * File:   SPI_COMM_BOARD.h
 * Author: Dan
 *
 * Created on December 5, 2014, 8:18 PM
 */

#ifndef SPI_COMM_BOARD_DRIVER_H
#define	SPI_COMM_BOARD_DRIVER_H

void initSPICommBoard();
void sendPollIntToComm(int data);
void sendIntToComm(int data);
void sendPollFloatToComm(float data);
void sendFloatToComm(float data);
int writeSPI1( int i);

void FIFOinitSPI1();
void FIFOputSPI1(char data);
void flushFIFOBufferSPI1(void);

#endif	/* SPI_COMM_BOARD_H */

