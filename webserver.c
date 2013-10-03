#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "mywebserver.h"
#include "webserver.h"
#include "control.h"
#include "vtUtilities.h"
#define INSPECT_STACK 1
#define baseStack 2
#if PRINTF_VERSION == 1
#define conSTACK_SIZE		((baseStack+5)*configMINIMAL_STACK_SIZE)
#else
#define conSTACK_SIZE		(baseStack*configMINIMAL_STACK_SIZE)
#endif

#define webserverQLength 10

typedef struct __WebMsg{

 	uint8_t msgType;
	uint8_t length;
	uint8_t buf[WebMaxLength +1];
}WebMsg;


static portTASK_FUNCTION_PROTO( WebServerTask, pvParameters );

//Initializer
void vStartWebServerTask(WebStruct *ptr, unsigned portBASE_TYPE uxPriority, ControlStruct * controldata)
{

	if( (ptr->inQ = xQueueCreate(webserverQLength, sizeof(WebMsg))) == NULL){
		VT_HANDLE_FATAL_ERROR(0);
	}
	
	portBASE_TYPE retval;
	ptr->controlData = controldata;
	
	if( (retval = xTaskCreate( WebServerTask, (signed char *) "Web Server", conSTACK_SIZE, (void*)ptr, uxPriority, ( xTaskHandle * ) NULL )) != pdPASS){
		VT_HANDLE_FATAL_ERROR(retval);
	}
}


//Actual Task that is run
static portTASK_FUNCTION( WebServerTask, pvParameters )
{

	WebMsg msgBuffer;
	WebStruct * webPtr = (WebStruct *) pvParameters;

	ControlStruct * controlData = webPtr->controlData;

	//infinite loop
	for(;;){

		// Wait for a message from the I2C (Encoder data) or from the Navigation Task (motor command)
		if (xQueueReceive(webPtr->inQ,(void *) &msgBuffer,portMAX_DELAY) != pdTRUE) {
			VT_HANDLE_FATAL_ERROR(0);
		}
		switch(msgBuffer.msgType){

		
			case wsFromControlMsg:
			{
				char message[64];
				//get the char array:
				int i, j = 0;
				for(i = 0; i < 64; i++){					
				
					message[i] =  (char)((msgBuffer.buf[j]   << 24) |
							 			 (msgBuffer.buf[j+1] << 16) |
										 (msgBuffer.buf[j+2] << 8 ) |
										 (msgBuffer.buf[j+3] << 0));
					j = j+4;
				}
				
			
				break;
			}
			case wsFromCalcMsg:
			{
			   	char message[64];
				//get the char array:
				int i, j = 0;
				for(i = 0; i < 64; i++){					
				
					message[i] =  (char)((msgBuffer.buf[j]   << 24) |
							 			 (msgBuffer.buf[j+1] << 16) |
										 (msgBuffer.buf[j+2] << 8 ) |
										 (msgBuffer.buf[j+3] << 0));
					j = j+4;
				}

			 	break;
			}
			case wsFromMappingMsg:
			{
				char message[64];
				//get the char array:
				int i, j = 0;
				for(i = 0; i < 64; i++){					
				
					message[i] =  (char)((msgBuffer.buf[j]   << 24) |
							 			 (msgBuffer.buf[j+1] << 16) |
										 (msgBuffer.buf[j+2] << 8 ) |
										 (msgBuffer.buf[j+3] << 0));
					j = j+4;
				}

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

portBASE_TYPE sendWSFromCalcMsg(WebStruct * webData, unsigned char webCommand[])
{
	if(webData == NULL) VT_HANDLE_FATAL_ERROR(0);
   	
	WebMsg buffer;
	buffer.length = 256;
	if(buffer.length > WebMaxLength)VT_HANDLE_FATAL_ERROR(0);
	int i = 0;
	int j = 0;
	int shift = 8;

	/* ==================================================
	 * So chars in c are stored like an int -> 4 bytes each.
	 * here i'm filling the buffer with the char array.
	 * the 64 bit char array becomes a 256 byte buffer
	 * ==================================================== */

	for(i = 0; i <= 64; i++){
		buffer.buf[j]   =  webCommand[j]        & 0xFF;
		buffer.buf[j+1] = (webCommand[j] >> 8 ) & 0xFF;
		buffer.buf[j+2] = (webCommand[j] >> 16) & 0xFF;
		buffer.buf[j+3] = (webCommand[j] >> 24) & 0xFF;
		j = j + 4;
	}

	buffer.msgType = wsFromCalcMsg;
	return(xQueueSend(webData->inQ, (void *) (&buffer), portMAX_DELAY));
}

portBASE_TYPE sendWSFromControlMsg(WebStruct * webData, unsigned char webCommand[])
{
	if(webData == NULL) VT_HANDLE_FATAL_ERROR(0);
   	
	WebMsg buffer;
	buffer.length = 64;
	if(buffer.length > WebMaxLength)VT_HANDLE_FATAL_ERROR(0);

	int i = 0;
	int j = 0;

	/* ==================================================
	 * So chars in c are stored like an int -> 4 bytes each.
	 * here i'm filling the buffer with the char array.
	 * the 64 bit char array becomes a 256 byte buffer
	 * ==================================================== */

	for(i = 0; i <= 64; i++){
		buffer.buf[j]   =  webCommand[j]        & 0xFF;
		buffer.buf[j+1] = (webCommand[j] >> 8 ) & 0xFF;
		buffer.buf[j+2] = (webCommand[j] >> 16) & 0xFF;
		buffer.buf[j+3] = (webCommand[j] >> 24) & 0xFF;
		j = j + 4;
	}


	buffer.msgType = wsFromControlMsg;
	return(xQueueSend(webData->inQ, (void *) (&buffer), portMAX_DELAY));
}

portBASE_TYPE sendWSFromMappingMsg(WebStruct * webData, unsigned char webCommand[])
{
	if(webData == NULL) VT_HANDLE_FATAL_ERROR(0);
   	
	WebMsg buffer;
	buffer.length = 64;
	if(buffer.length > WebMaxLength)VT_HANDLE_FATAL_ERROR(0);

	int i = 0;
	int j = 0;

	/* ==================================================
	 * So chars in c are stored like an int -> 4 bytes each.
	 * here i'm filling the buffer with the char array.
	 * the 64 bit char array becomes a 256 byte buffer
	 * ==================================================== */

	for(i = 0; i <= 64; i++){
		buffer.buf[j]   =  webCommand[j]        & 0xFF;
		buffer.buf[j+1] = (webCommand[j] >> 8 ) & 0xFF;
		buffer.buf[j+2] = (webCommand[j] >> 16) & 0xFF;
		buffer.buf[j+3] = (webCommand[j] >> 24) & 0xFF;
		j = j + 4;
	}
	buffer.msgType = wsFromMappingMsg;
	return(xQueueSend(webData->inQ, (void *) (&buffer), portMAX_DELAY));
}



