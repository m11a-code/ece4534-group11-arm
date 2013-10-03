#ifndef MSGTYPES_H
#define MSGTYPES_H 

typedef struct __MappingStruct MappingStruct;
typedef struct __CalcStruct CalcStruct;
typedef struct __UARTStruct UARTStruct;
typedef struct __WebStruct WebStruct;
typedef struct __ControlStruct ControlStruct;

/* MAPPING STUFF */

struct __MappingStruct{
   	WebStruct * webData;
	xQueueHandle inQ;
};
//Mapping Msg Types:
#define vertexDataMsg 1
#define sideLengthMsg 2


/* CALCULATION STUFF */
struct __CalcStruct{
	MappingStruct * mapData;
	WebStruct * webData;
	xQueueHandle inQ;	
};
//Calculation Msg Types:
// NOTE: PROBABLY WANT MORE THAN 1 TYPE FOR DIFFERENT SENSORS
#define sensorDataMsg 1

/* UART STUFF */
struct __UARTStruct{
   	ControlStruct * controlData;
	CalcStruct * calcData;
	xQueueHandle inQ;
};
//UART Msg Types:
#define sensorDataMsg 1
#define motorControlMsg 2

/* WEB SERVER STUFF */
struct __WebStruct{
	
	ControlStruct * controlData;
	xQueueHandle inQ;
};
//Web Server Msg Types:
#define wsFromControlMsg 1
#define wsFromCalcMsg 2
#define wsFromMappingMsg 3

/* CONTROL STUFF */
struct __ControlStruct{

	UARTStruct * uartData;
	WebStruct * webData;
	xQueueHandle inQ;
};				   
//Control Msg Types:
#define sensorDataFromUARTMsg 1
#define sensorDataFromWSMsg 2


#endif
