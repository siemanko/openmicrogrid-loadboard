/* 
 * File:   NodalCommunication.h
 * Author: Dan Strawser
 *
 * This is a package which controls communication between uLinks
 *
 * Created on October 17, 2014, 4:35 PM
 */

#ifndef SERIALCOMMUNICATION_H
#define	SERIALCOMMUNICATION_H

extern int testGlobal;

void initUART2();
void SendSerialMessage(char * msg);
void SendSerialFloat(float data);

void putUART2(char data);


#endif	/* SERIALCOMMUNICATION_H */

