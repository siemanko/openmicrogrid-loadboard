/*
 Serial communcation for dsPIC33E
 * POWER CHIP

 */
#include <xc.h>
#include <pps.h>
#include <string.h>
#include <stdlib.h>
#include "SerialCommunication.h"


#define FP 39600000
#define BAUDRATE 9600
#define BRGVAL ((FP/BAUDRATE)/16)-1
#define FIFO_SIZE 64 //Size of communication receive buffer

char *receivePointerU2;
int receiveBufferSizeU2 = 0;
int bufferLenU2 = 0;

//Used for the input receive character buffer
char receiveBufferU2[FIFO_SIZE];
char * inputStringU2;


//POWER BOARD COMMUNICATION
void initUART2(){
    
    TRISFbits.TRISF4 = 1;   //PIN 76, RB9 as INPUT
    TRISFbits.TRISF5 = 0;   //PIN 77, RC6 as OUTPUT

    U2MODEbits.STSEL = 0; // 1-Stop bit
    U2MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U2MODEbits.ABAUD = 0; // Auto-Baud disabled
    U2MODEbits.BRGH = 0; // Standard-Speed mode

    U2BRG = BRGVAL;

    U2MODEbits.UARTEN = 1; // Enable UART
    U2STAbits.UTXEN = 1; // Enable UART TX
}


//Will send a string on the network
void SendSerialMessage(char * output){
    int len = strlen(output);
    int i;
    for(i = 0; i < len ; i++){
        putUART2(output[i]);
    }
}

void SendSerialFloat(float data){

    void *ptr = &data;
    int i;
    ptr+=3;

    for(i=3;i>=0;i--){
       putUART2(*(char *)ptr);
       ptr--;
    }
}

void putUART2(char data){
    while(U2STAbits.UTXBF == 1);
    U2TXREG = data;
}