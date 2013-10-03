/*
*	These functions deal with the control of the rover.
*/
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
#include "control.h"
#include "uart.h"
#include "webserver.h"


#define INSPECT_STACK 1
#define baseStack 2
#if PRINTF_VERSION == 1
#define conSTACK_SIZE		((baseStack+5)*configMINIMAL_STACK_SIZE)
#else
#define conSTACK_SIZE		(baseStack*configMINIMAL_STACK_SIZE)
#endif


#define ControlQLength 10


// define message types

typedef struct __ControlMsg{
	
 	uint8_t msgType;
	uint8_t length;
	uint8_t buf[ControlMaxLength +1];

}ControlMsg;

static portTASK_FUNCTION_PROTO(ControlTask, pvParameters);


void vStartControlTask(ControlStruct *ptr, unsigned portBASE_TYPE uxPriority, UARTStruct * uartdata, WebStruct * webdata)
{
	if( (ptr->inQ = xQueueCreate(ControlQLength, sizeof(ControlMsg))) == NULL){
		VT_HANDLE_FATAL_ERROR(0);
	}

	portBASE_TYPE retval;
	ptr->webData = webdata;
	ptr->uartData = uartdata;

	if( (retval = xTaskCreate( ControlTask, (signed char *) "Control", conSTACK_SIZE, (void*)ptr, uxPriority, ( xTaskHandle * ) NULL )) != pdPASS){
		VT_HANDLE_FATAL_ERROR(retval);
	}
}

portBASE_TYPE sendSensorDataFromUARTMsg(ControlStruct * controlData, unsigned short sensorData)
{
	if(controlData == NULL)
		VT_HANDLE_FATAL_ERROR(0);

	ControlMsg buffer;
	buffer.length = 2;

	if(buffer.length > ControlMaxLength)
		VT_HANDLE_FATAL_ERROR(0);

	buffer.buf[1] = sensorData & 0xFF;
	buffer.buf[0] = (sensorData >> 8)	& 0xFF;;

	buffer.msgType = sensorDataFromUARTMsg;
	return(xQueueSend(controlData->inQ,(void *) (&buffer),portMAX_DELAY)); 

}

portBASE_TYPE sendSensorDataFromWebServerMsg(ControlStruct * controlData, unsigned short sensorData)
{
	if(controlData == NULL)
		VT_HANDLE_FATAL_ERROR(0);

	ControlMsg buffer;
	buffer.length = 2;

	if(buffer.length > ControlMaxLength)
		VT_HANDLE_FATAL_ERROR(0);


   	buffer.buf[1] = sensorData & 0xFF;
	buffer.buf[0] = (sensorData >> 8)	& 0xFF;
 
	buffer.msgType = sensorDataFromWSMsg;
	return(xQueueSend(controlData->inQ,(void *) (&buffer),portMAX_DELAY)); 

	

}

static portTASK_FUNCTION( ControlTask, pvParameters)
{
	ControlMsg msgBuffer;
	ControlStruct * controlPtr = (ControlStruct *) pvParameters;

	WebStruct * webData = controlPtr->webData;
	UARTStruct * uartData = controlPtr->uartData;

	for(;;){

		// Wait for a message from the I2C (Encoder data) or from the Navigation Task (motor command)
		if (xQueueReceive(controlPtr->inQ,(void *) &msgBuffer,portMAX_DELAY) != pdTRUE) {
			VT_HANDLE_FATAL_ERROR(0);
		}

		switch(msgBuffer.msgType){

			case sensorDataFromUARTMsg:
			{
			//Sensor Data Received from UART...
				short sensorData;
				sensorData = (256 * msgBuffer.buf[0]) + msgBuffer.buf[1];
			
			//do what needs to be done with the data...
			
			//most likely will send a message to the UART task with motor control data
				

			 	break;
			}
			case sensorDataFromWSMsg:
			{
				short sensorData;
				sensorData = (256 * msgBuffer.buf[0]) + msgBuffer.buf[1];

				break;
			}
			default:
			{

				VT_HANDLE_FATAL_ERROR(0);
				break;
			}
		}//end switch
	}//end for


}




