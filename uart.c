#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/* include files. */
#include "uart.h"
#include "calculation.h"
#include "control.h"
#include "semphr.h"
#include "vtUtilities.h"


#define INSPECT_STACK 1
#define baseStack 2
#if PRINTF_VERSION == 1
#define conSTACK_SIZE		((baseStack+5)*configMINIMAL_STACK_SIZE)
#else
#define conSTACK_SIZE		(baseStack*configMINIMAL_STACK_SIZE)
#endif

#define UARTQLength 10

typedef struct __UARTMsg{
 	uint8_t msgType;
	uint8_t length;
	uint8_t buf[UARTMaxLength +1];	
}UARTMsg;

static portTASK_FUNCTION_PROTO(UARTTask, pvParameters);

void vStartUARTTask(UARTStruct * ptr, unsigned portBASE_TYPE uxPriority, CalcStruct * calcdata, ControlStruct  * controldata)
{
	if( (ptr->inQ = xQueueCreate(UARTQLength, sizeof(UARTMsg))) == NULL){
		VT_HANDLE_FATAL_ERROR(0);
	}

	portBASE_TYPE retval;
	ptr->controlData = controldata;
	ptr->calcData = calcdata;
	
	if( (retval = xTaskCreate( UARTTask, (signed char *) "UART", conSTACK_SIZE, (void*)ptr, uxPriority, ( xTaskHandle * ) NULL )) != pdPASS){
		VT_HANDLE_FATAL_ERROR(retval);
	}	
}

static portTASK_FUNCTION(UARTTask, pvParameters)
{
 	UARTMsg msgBuffer;
	UARTStruct * uartPtr = (UARTStruct *) pvParameters;

	CalcStruct * calcData = uartPtr->calcData;
	ControlStruct * controlData = uartPtr->controlData;

	for(;;){
		// Wait for a message from the I2C (Encoder data) or from the Navigation Task (motor command)
		if (xQueueReceive(uartPtr->inQ,(void *) &msgBuffer,portMAX_DELAY) != pdTRUE) {
			VT_HANDLE_FATAL_ERROR(0);
		}
		switch(msgBuffer.msgType){

			case sensorDataMsg:
		 	{
				short sensorData = (short)(256 * msgBuffer.buf[0]) + msgBuffer.buf[1];
				

				break;
		 	}
			case motorControlMsg:
			{	

			/* == Here, the motor control message sent from the control task is 
			 * received by the UART task and ready to send
			 */
			   	short motorMsg = (short)(256 * msgBuffer.buf[0]) + msgBuffer.buf[1];
				break;
			}
			default:
			{	
	
				VT_HANDLE_FATAL_ERROR(0);
				break;
			}
		}//end switch
	}
}
portBASE_TYPE sendUARTSensorDataMsg(UARTStruct * uartData, unsigned short sensordata)
{
	if(uartData == NULL) VT_HANDLE_FATAL_ERROR(0);
   	
	UARTMsg buffer;
	buffer.length = 2;
	if(buffer.length > UARTMaxLength)VT_HANDLE_FATAL_ERROR(0);

	buffer.buf[1] = sensordata & 0xFF;
	buffer.buf[0] = (sensordata >> 8)	& 0xFF;

	buffer.msgType = sensorDataMsg;
	return(xQueueSend(uartData->inQ, (void *) (&buffer), portMAX_DELAY));
}

portBASE_TYPE sendMotorControlMsg(UARTStruct * uartData, unsigned short motorControlData)
{
	if(uartData == NULL) VT_HANDLE_FATAL_ERROR(0);
   	
	UARTMsg buffer;
	buffer.length = 2;
	if(buffer.length > UARTMaxLength)VT_HANDLE_FATAL_ERROR(0);

	buffer.buf[1] = motorControlData & 0xFF;
	buffer.buf[0] = (motorControlData >> 8)	& 0xFF;

	buffer.msgType = motorControlMsg;
	return(xQueueSend(uartData->inQ, (void *) (&buffer), portMAX_DELAY));
}

