#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "vtUtilities.h"
/* include files. */
#include "calculation.h"
#include "webserver.h"
#include "mapping.h"


#define INSPECT_STACK 1
#define baseStack 2
#if PRINTF_VERSION == 1
#define conSTACK_SIZE		((baseStack+5)*configMINIMAL_STACK_SIZE)
#else
#define conSTACK_SIZE		(baseStack*configMINIMAL_STACK_SIZE)
#endif

#define calcQLength 10



typedef struct __CalcMsg{
	uint8_t msgType;
	uint8_t length;
	uint8_t buf[maxCalcMsgLength +1];
}CalcMsg;

static portTASK_FUNCTION_PROTO( CalcTask, pvParameters);


void vStartCalcTask(CalcStruct * ptr, unsigned portBASE_TYPE uxPriority, MappingStruct * mapdata, WebStruct * webdata)
{
	if( (ptr->inQ = xQueueCreate(calcQLength, sizeof(CalcMsg))) == NULL){
		VT_HANDLE_FATAL_ERROR(0);
	}

	portBASE_TYPE retval;
	ptr->webData = webdata;
	ptr->mapData = mapdata;

	if( (retval = xTaskCreate(CalcTask, (signed char *) "Calculation", conSTACK_SIZE, (void*)ptr, uxPriority, ( xTaskHandle * ) NULL )) != pdPASS){
		VT_HANDLE_FATAL_ERROR(0);
	}	
}

static portTASK_FUNCTION(CalcTask, pvParameters)
{
	CalcMsg msgBuffer;
	CalcStruct * calcPtr = (CalcStruct *) pvParameters;

	WebStruct * webData = calcPtr->webData;
	MappingStruct * mapData = calcPtr->mapData;
	for(;;){
		if (xQueueReceive(calcPtr->inQ,(void *) &msgBuffer,portMAX_DELAY) != pdTRUE) {
			VT_HANDLE_FATAL_ERROR(0);
		}


		switch(msgBuffer.msgType){

			case sensorDataMsg:	 //From UART
		 	{
				//get the message:
				unsigned short data = (256 * msgBuffer.buf[0]) + msgBuffer.buf[1];
				
				//whatever needs to be done with the data...

				//example: maybe send as side length to mapping task?
				//sendSideLengthDataMsg(mapData, data);

		 		break;
			 }
			 default:
			{
				VT_HANDLE_FATAL_ERROR(0);
				break;
			}
		}
	}
}

portBASE_TYPE sendSensorDataMsg(CalcStruct * calcdata, unsigned short sensorData)
{
	if(calcdata == NULL) VT_HANDLE_FATAL_ERROR(0);
   	
	CalcMsg buffer;
	buffer.length = 2;
	if(buffer.length > maxCalcMsgLength)VT_HANDLE_FATAL_ERROR(0);

	buffer.buf[1] = sensorData & 0xFF;
	buffer.buf[0] = (sensorData >> 8) & 0xFF;
	buffer.msgType = sensorDataMsg;
	return(xQueueSend(calcdata->inQ, (void *) (&buffer), portMAX_DELAY));

}

double calcIntegrationArea(uint8_t* buffer)
{
	double area = 0;
	//calculate area via finite integral
	
	return area;
}


double calcIrregularPolygonArea(uint8_t* buffer)
{
	double area = 0;
	//calculate area via area of irregular polygon equation
	
	return area;
}

double calcRegularPolygonArea(uint8_t* buffer)
{
	double area = 0;
	//calculate area via area of regular polygon equation
	
	return area;
}


//set up a Task to calculate the area using the methods above.

