/*
*	These functions calculate the area of the enclosure
*/


#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H


#include "msgtypes.h"
#define WebMaxLength 256

/* =========================================
 * vStartWebServerTask
 *
 * Starts the webserver task
 * 
 * Args:
 * 	ptr - a pointer to the webstruct that
 *	uxPriority - the priority you want this task to be run at
 *	controldata - a control struct used to communicate with the control task
 *
 * Returns: nothing
 * ======================================== */  
void vStartWebServerTask(WebStruct *ptr, unsigned portBASE_TYPE uxPriority, ControlStruct * controldata);

/* =========================================
 * sendWSFromMappingMsg
 *
 * sends a message to the webserver from the mapping task
 * 
 * Args:
 * 	webData - the WebStruct that contains the queue for the web server task
 *	webCommand  - the message to be sent, a 64 character array
 *
 * Returns: Result of the call to xQueueSend()	
 * ======================================== */
portBASE_TYPE sendWSFromMappingMsg(WebStruct * webData, unsigned char webCommand[]);


/* =========================================
 * sendWSFromControlMsg
 *
 * sends a message to the webserver from the control task
 * 
 * Args:
 * 	webData - the WebStruct that contains the queue for the web server task
 *	webCommand  - the message to be sent, a 64 character array
 *
 * Returns: Result of the call to xQueueSend()	
 * ======================================== */
portBASE_TYPE sendWSFromControlMsg(WebStruct * webData, unsigned char webCommand[]);


/* =========================================
 * sendWSFromCalcMsg
 *
 * sends a message to the webserver from the calculation task
 * 
 * Args:
 * 	webData - the WebStruct that contains the queue for the web server task
 *	webCommand  - the message to be sent, a 64 character array
 *
 * Returns: Result of the call to xQueueSend()	
 * ======================================== */
portBASE_TYPE sendWSFromCalcMsg(WebStruct * webData, unsigned char webCommand[]);



#endif