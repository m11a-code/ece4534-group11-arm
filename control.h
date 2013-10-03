/*
*	These functions deal with the control of the rover.
*/

#ifndef CONTROL_H
#define CONTROL_H
#include "queue.h"
#include "timers.h"
#include "msgtypes.h"

#define ControlMaxLength 2

/* =========================================
 * vStartControlTask()
 *
 * starts the UART task
 * 
 * Args:
 * 	ptr - a pointer to a control struct that contains the queue used to communicate with this task
 * 	uxPriority -- the priority you want this task to be run at
 *	uartData - contains the queue to the uart task. used to communicate with calc task
 *	webData - contains the queue to the webserver task
 *
 * Returns: void	
 * ======================================== */ 
void vStartControlTask(ControlStruct *ptr, unsigned portBASE_TYPE uxPriority, UARTStruct * uartdata, WebStruct * webdata);


/* =========================================
 * sendSensorDataFromWSMsg()
 *
 * Sends sensor data from Web server to control task
 * 
 * Args:
 * 	controlData - contains the queue to the control task
 *	sensorData - the sensor data to be sent
 *
 * Returns: Result of the call to xQueueSend()	
 * ======================================== */ 
portBASE_TYPE sendSensorDataFromWSMsg(ControlStruct * controlData, unsigned short sensorData);


/* =========================================
 * sendSensorDataFromUARTMsg()
 *
 * Sends a sensor reading from UART to the control task
 * 
 * Args:
 * 	controlData - contains the queue to the control task
 *	sensorData - the sensor data to be sent
 *
 * Returns: Result of the call to xQueueSend()	
 * ======================================== */ 
portBASE_TYPE sendSensorDataFromUARTMsg(ControlStruct * controlData, unsigned short sensorData);




#endif