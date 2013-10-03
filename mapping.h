/*
*	These functions deal with the mapping of the enclosure.
*/

#ifndef MAPPING_H
#define MAPPING_H
#include "queue.h"
#include "timers.h"
#include "msgtypes.h"

#define MappingMaxLength 2


/* =========================================
 * vStartMappingTask()
 *
 * starts the Mapping task
 * 
 * Args:
 * 	ptr - a pointer to a mapping struct that contains the queue used to communicate with this task
 *	uxPriority -- the priority you want this task to be run at
 *	webData - contains the queue to the Control task
 *
 * Returns: void	
 * ======================================== */ 
void vStartMappingTaskMsg(MappingStruct *ptr, unsigned portBASE_TYPE uxPriority, WebStruct * webdata);


/* =========================================
 * sendVertexDataMsg()
 *
 * sends vertex data to the mapping task
 * 
 * Args:
 * 	mappingData - contains the queue to the mapping task
 *	vertexData - the vertex data being sent 
 *
 * Returns: Result of the call to xQueueSend()	
 * ======================================== */ 
portBASE_TYPE sendVertexDataMsg(MappingStruct * mappingData, unsigned short vertexData);


/* =========================================
 * sendSideLengthDataMsg()
 *
 * Sends a sensor reading to the UART task from the rover
 * 
 * Args:
 * 	mappingData - contains the queue to the mapping task
 *	sideLen - the side length measurement being sent
 *
 * Returns: Result of the call to xQueueSend()	
 * ======================================== */ 
portBASE_TYPE sendSideLengthDataMsg(MappingStruct * mappingData, unsigned short sideLen);


#endif