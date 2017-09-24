/*
 * Functions_Alerte.h
 *
 *  Created on: Feb 28, 2015
 *      Author: cedric
 */

#ifndef FUNCTIONS_ALERTE_H_
#define FUNCTIONS_ALERTE_H_


/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stddef.h>
#include <pthread.h>

#include "../Lib_main.h"
#include "../Lib_socket/Lib_socket.h"
#include "../Lib_Config/Lib_Config.h"


/*******************************************
*               D E F I N E                *
********************************************/
#define PHONE_CEDRIC		2
#define PHONE_AURELIE		1
#define NO_PHONE			0

#define NO_SMS				0
#define SMS_OK				1

#define ERROR				-1
#define OK					0

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
int 		isystemON;

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
int 	Ping_Phone();
int 	send_Alert(int iSmsok, char strCaptor[5]);
int 	sendEmail(char strCaptor[5]);

#endif /* FUNCTIONS_ALERTE_H_ */
