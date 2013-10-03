#ifndef UART_H
#define UART_H
#include "queue.h"
#include "msgtypes.h"

#include "timers.h"
#define UARTMaxLength 2


/* =========================================
 * vStartUARTTask()
 *
 * starts the UART task
 * 
 * Args:
 * 	ptr - a pointer to a UART struct that contains the queue used to communicate with this task
 * 	uxPriority -- the priority you want this task to be run at
 *	calcData - contains the queue to the Calc task. used to communicate with calc task
 *	controlData - contains the queue to the Control task
 *
 * Returns: void	
 * ======================================== */ 
void vStartUARTTask(UARTStruct * ptr, unsigned portBASE_TYPE uxPriority, CalcStruct * calcdata, ControlStruct * controldata);


/* =========================================
 * sendUARTSensorDataMsg()
 *
 * Sends a sensor reading to the UART task from the rover
 * 
 * Args:
 * 	uartData - contains the queue to the UART task
 *	sensorData - the sensor data to be sent
 *
 * Returns: Result of the call to xQueueSend()	
 * ======================================== */ 
portBASE_TYPE sendUARTSensorDataMsg(UARTStruct * uartData, unsigned short sensorData);


/* =========================================
 * sendMotorControlMsg()
 *
 * sends motor control data to the UART task to be sent to the rover
 * 
 * Args:
 * 	uartData - contains the queue to the UART task
 *	motorControlData - the motor control data
 *
 * Returns: Result of the call to xQueueSend()	
 * ======================================== */ 									 
portBASE_TYPE sendMotorControlMsg(UARTStruct * uartData, unsigned short motorControlData);

#endif