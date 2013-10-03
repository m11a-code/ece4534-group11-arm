#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "mapping.h"
#include "webserver.h"
#include "vtUtilities.h"

#define INSPECT_STACK 1
#define baseStack 2
#if PRINTF_VERSION == 1
#define conSTACK_SIZE		((baseStack+5)*configMINIMAL_STACK_SIZE)
#else
#define conSTACK_SIZE		(baseStack*configMINIMAL_STACK_SIZE)
#endif

#define mappingQLength 10

typedef struct __MappingMsg{

 	uint8_t msgType;
	uint8_t length;
	uint8_t buf[MappingMaxLength +1];
}MappingMsg;


static portTASK_FUNCTION_PROTO( MappingTask, pvParameters );

//Initializer
void vStartMappingTask(MappingStruct *ptr, unsigned portBASE_TYPE uxPriority, WebStruct * webdata)
{

	if( (ptr->inQ = xQueueCreate(mappingQLength, sizeof(MappingMsg))) == NULL){
		VT_HANDLE_FATAL_ERROR(0);
	}

	portBASE_TYPE retval;
	ptr->webData = webdata;
	
	if( (retval = xTaskCreate( MappingTask, (signed char *) "Mapping", conSTACK_SIZE, (void*)ptr, uxPriority, ( xTaskHandle * ) NULL )) != pdPASS){
		VT_HANDLE_FATAL_ERROR(retval);
	}
}


//Actual Task that is run
static portTASK_FUNCTION( MappingTask, pvParameters )
{

	MappingMsg msgBuffer;
	MappingStruct * mapPtr = (MappingStruct *) pvParameters;

	WebStruct * webData = mapPtr->webData;

	//infinite loop
	for(;;){

		// Wait for a message from the I2C (Encoder data) or from the Navigation Task (motor command)
		if (xQueueReceive(mapPtr->inQ,(void *) &msgBuffer,portMAX_DELAY) != pdTRUE) {
			VT_HANDLE_FATAL_ERROR(0);
		}
		switch(msgBuffer.msgType){
			
			case vertexDataMsg:
			{  	
				short vertexData = (256 * msgBuffer.buf[0]) + msgBuffer.buf[1];
			
				break;
			}
			case sideLengthMsg:
			{
				short sideLengthData = (256 * msgBuffer.buf[0]) + msgBuffer.buf[1];

			 	break;
			}
			default:
			{
				VT_HANDLE_FATAL_ERROR(0);
				break;
			}
		}//end switch
	}//END FOR
}

portBASE_TYPE sendVertexDataMsg(MappingStruct * mappingData, unsigned short vertexData)
{
	if(mappingData == NULL) VT_HANDLE_FATAL_ERROR(0);
   	
	MappingMsg buffer;
	buffer.length = 2;
	if(buffer.length > MappingMaxLength)VT_HANDLE_FATAL_ERROR(0);

	buffer.buf[1] = vertexData & 0xFF;
	buffer.buf[0] = (vertexData >> 8)	& 0xFF;
	buffer.msgType = vertexDataMsg;
	return(xQueueSend(mappingData->inQ, (void *) (&buffer), portMAX_DELAY));
}

portBASE_TYPE sendSideLengthDataMsg(MappingStruct * mappingData, unsigned short sideLen)
{
	if(mappingData == NULL) VT_HANDLE_FATAL_ERROR(0);

	MappingMsg buffer;
	buffer.length = 2;

	if(buffer.length > MappingMaxLength)VT_HANDLE_FATAL_ERROR(0);

	buffer.buf[1] = sideLen & 0xFF;
	buffer.buf[0] = (sideLen >> 8)	& 0xFF;

	buffer.msgType = sideLengthMsg;
	return(xQueueSend(mappingData->inQ, (void *) (&buffer), portMAX_DELAY));	

}

