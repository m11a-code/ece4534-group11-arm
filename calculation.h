/*
*	These functions calculate the area of the enclosure
*/

#ifndef CALCULATION_H
#define CALCULATION_H
#include "queue.h"
#include "timers.h"
#include "msgtypes.h"


#define maxCalcMsgLength 2

/* =========================================
 * vStartCalcTask()
 *
 * starts the calculation task
 * 
 * Args:
 * 	ptr - a pointer to a calc struct that contains the queue used to communicate with this task
 * 	uxPriority -- the priority you want this task to be run at
 *	mapData - contains the queue to the mapping task. used to communicate with calc task
 *	webData - contains the queue to the web server task
 *
 * Returns: void	
 * ======================================== */
void vStartCalcTask(CalcStruct * ptr, unsigned portBASE_TYPE uxPriority, MappingStruct * mapdata, WebStruct * webdata);


/* =========================================
 * sendSensorDataMsg()
 *
 * sends sensor readings to the calc task
 * 
 * Args:
 * 	calcData - contains the queue to the calculation task
 *	sensorData - the sensor data to be sent
 *
 * Returns: Result of the call to xQueueSend()	
 * ======================================== */ 
portBASE_TYPE sendSensorDataMsg(CalcStruct * calcdata, unsigned short sensorData);

double calcIntegrationArea(uint8_t *buffer);

double calcIrregularPolygonArea(uint8_t* buffer);

double calcRegularPolygonArea(uint8_t* buffer);


#endif