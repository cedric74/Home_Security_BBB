/*
 ============================================================================
 Name        : Lib.h
 Author      : Cedric T.
 Version     : 1.0
 Date		 : 29/01/2015
 Description : 
 ============================================================================
 */
#ifndef _LIB_DETECT_H
#define _LIB_DETECT_H

/*******************************************
*			  I N C L U D E 			   *			
********************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "../Lib_Gpio/beh_BBB_gpio.h"
#include "../Functions_Alert/Functions_Alert.h"
#include "../Lib_Siren/Lib_Siren.h"
#include "../Lib_Camera/Lib_Camera.h"
#include "../Lib_main.h"

/*******************************************
*               D E F I N E                *			
********************************************/
#define CAPTOR_MAIN_DOOR		P9_14
#define CAPTOR_BACK_DOOR		P9_12
#define INTER_ON_OFF			P9_13


#define	STATE_OFF			0
#define STATE_ON			1

#define DETECT_OK			0
#define INTRUSION_OK		1
#define INTRUSION_NO		0

#define COUNT_DETECTION		50		// ,Changed in version 1.13 from 100->75, try fix fast door opening

#define COUNT_INTERRUPTER	500

#define OFF					0
#define ON					1

#define OFF_INTERRUPT		1
#define ON_INTERRUPT		0

#define DELAYS_TO_DISABLE	15		// In Sec , 30 Sec Maybe To Long
#define DELAYS_TO_ENABLE	(60*2)	// 2 MIN

#define SIZE_MESS_CAPTOR	4

#define WAIT_1MS			1000
/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *			
********************************************/
typedef enum {STATE_NO_DETECTION = 0, STATE_DETECTION  = 1}stateCaptor;

typedef struct {
	stateCaptor stateCapt;
	int 		icountDete;
	GPIO_Pins	ePinCaptor;
	char*		sMessage;
}structCaptor;

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *			
********************************************/
unsigned char   stateInterrupter;

structCaptor 	sCaptorMainDoor;
structCaptor 	sCaptorBackDoor;

structCaptor 	* ptrCaptorMainDoor;
structCaptor 	* ptrCaptorBackDoor;

/*******************************************
*	        F U N C T I O N S   	       *			
********************************************/
void	main_Detect();

void 	Read_Captor(structCaptor * sCaptor);
void 	Read_Interrupter();
void 	Init_Lib_Detect();

void * 	Thread_Read_Input();



#endif /*_LIB_DETECT_H */
