#include <xc.h>
#include "SPI_COMM_BOARD_DRIVER.h"
#include "COMM_BOARD_INTERFACE.h"
#include "SerialCommunication.h"
#include <spi.h>
#include "SPI.h"
#include <stdlib.h>

#define SPI_COMM_FIFO_SIZE 64 //Size of communication receive buffer

char *receivePointerSPI1;
int receiveBufferSizeSPI1 = 0;
int bufferLenSPI1 = 0;

//Used for the input receive character buffer
char receiveBufferSPI1[SPI_COMM_FIFO_SIZE];
char * inputStringSPI1;

int SPI1_WriteFlag;     //To indicate that we are sending data and the interrupt should not ack an ack
int outputTOCommmData;

static void * float_ptr;
static int outputDataInt;
static float outputDataFloat;

void initSPICommBoard(){

   INTCON2 = 0;
   INTCON1 = 0;
   U1MODE = 0;

    TRISFbits.TRISF6 = 1;   //SCK2 as Input on G6
    TRISFbits.TRISF2 = 1;   //SDI2 as Input on G7
    TRISFbits.TRISF3 = 0;   //SDO2 as output on G8

    TRISBbits.TRISB14 = 1;   //CS as Input on G9

    SPI1CON1=0;
    SPI1STAT=0;
    SPI1STATbits.SPIROV = 0;
    SPI1STATbits.SPIEN = 0;
    SPI1CON2=0;

    SPI1CON1bits.DISSCK=0;
    SPI1CON1bits.DISSDO = 0;
    SPI1CON1bits.MODE16=0;
    SPI1CON1bits.SMP=0;
    SPI1CON1bits.CKE=0;
    SPI1CON1bits.CKP=0;
    SPI1CON1bits.SSEN = 1;
    SPI1CON1bits.PPRE=0;
    SPI1CON1bits.SPRE = 0;
    SPI1CON1bits.MSTEN=0;

    IFS0bits.SPI1IF = 0;
    IEC0bits.SPI1IE = 1;
    IPC2bits.SPI1IP = 2;    //Interrupt priority for SPI1

    SPI1STATbits.SPIEN=1;

    FIFOinitSPI1();

    SPI1_WriteFlag = 0;
}

void sendPollIntToComm(int data){
    outputDataInt = data;
    float_ptr = &outputDataInt;

    //float_ptr = &data;
    //outputData = data;
}

void sendIntToComm(int data){
    SPI1_WriteFlag = 1;
    IEC0bits.SPI1IE = 0;
    writeSPI1(data);
    IEC0bits.SPI1IE = 0;
    SPI1_WriteFlag = 0;
}

void sendPollFloatToComm(float data){
    outputDataFloat = data;
    float_ptr = &outputDataFloat;
    float_ptr+=3;
}

void sendFloatToComm(float data){
    SPI1_WriteFlag = 1;
    void *ptr = &data;
    int i;
    ptr+=3;

    for(i=3;i>=0;i--){
       writeSPI1(*(char *)ptr);
       ptr--;
    }
    SPI1_WriteFlag = 0;
}

int writeSPI1( int i){
    
    SPI1BUF = i; // write to buffer for TX
    //while( !SPI1STATbits.SPIRBF); // wait for transfer complete
   // int buffer = SPI1BUF;
    
    return 1; // read the received value

}//writeSPI1

//Initializes receive string fifo buffer
void FIFOinitSPI1(void){
    bufferLenSPI1 = 0;
    receivePointerSPI1 = receiveBufferSPI1;
}

void FIFOputSPI1(char data){
    putUART2(data);

   *receivePointerSPI1 = data;
    receivePointerSPI1++;
    bufferLenSPI1++;
}


void flushFIFOBufferSPI1(void){
    
    inputStringSPI1 = (char *)malloc(bufferLenSPI1*sizeof(char));
    int i;
    for(i=0;i<bufferLenSPI1;i++){
        inputStringSPI1[i]=receiveBufferSPI1[i];
    }
     FIFOinitSPI1();
}

void __attribute__((__interrupt__,auto_psv)) _SPI1Interrupt(void){

    int buffer = SPI1BUF;
    SPI1BUF = *(char *)float_ptr;
    //delayDebug(10);

    float_ptr--;

    //Simple protocol that assumes all the trash bytes we receive (i.e. real messages) are not zero
    if(buffer != 0){
        receiveMessageComm(buffer);
    }
    //This code is for an ACK (if you'd like one)
    //Need to make sure timing is correct
    /*
    if(!SPI1_WriteFlag){    //Don't ack an ack
       SPI1BUF = 6;        //Acknowledge the receipt (ACK=6)
    } */

    SPI1STATbits.SPIROV = 0;
    IFS0bits.SPI1IF = 0;
 
}
