/*
 * Functions_State_Machine.h
 *
 *  Created on: Mar 4, 2015
 *      Author: cedric
 */

#ifndef FUNCTIONS_STATE_MACHINE_H_
#define FUNCTIONS_STATE_MACHINE_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../Functions_Alert/Functions_Alert.h"
#include "../Lib_socket/Lib_socket.h"
#include "../Lib_Detect/Lib_Detect.h"
#include "../Lib_Gpio/beh_BBB_gpio.h"
#include "../Lib_Camera/Lib_Camera.h"
#include "../Lib_Siren/Lib_Siren.h"
#include "../Lib_Sensor/Lib_Sensor.h"
#include "../Lib_Com/Lib_Com.h"

/*******************************************
*               D E F I N E                *
********************************************/

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
void 			Init_State_Machine();
void * 			Thread_State_Machine(void * p);
unsigned char	state_machine(void);
void* 			Thread_Read_Command();
void* 			Thread_Send_Data_PC(void * p);
void 			StopThread_Send_Data();
void 			StartThread_Send_Data();
void* 			Thread_ReadSensor();
void 			StartThread_ReadSensor();



#endif /* FUNCTIONS_STATE_MACHINE_H_ */
