/*
 * Lib_Siren.h
 *
 *  Created on: Mar 4, 2015
 *      Author: cedric
 */

#ifndef LIB_SIREN_H_
#define LIB_SIREN_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <pthread.h>

#include "../Lib_Gpio/beh_BBB_gpio.h"
#include "../Lib_Detect/Lib_Detect.h"

/*******************************************
*               D E F I N E                *
********************************************/
#define SIREN_ON		1
#define SIREN_OFF		0

#define SIREN			P9_15
#define BUZZER			P9_11

// Buzzer V2 With PWM	P9_16


/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
void    Start_Thread_Warning();
void *  Thread_WarningSytemON();
void 	WarningSystemOn();
void* 	Thread_Siren();
void 	Start_Siren();
void 	Stop_Siren();
void 	Init_Siren();


#endif /* LIB_SIREN_H_ */
