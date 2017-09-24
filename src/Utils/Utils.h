/*
 * Utils.h
 *
 *  Created on: Sep 24, 2017
 *      Author: cedrictoncanier
 */

#ifndef SRC_UTILS_UTILS_H_
#define SRC_UTILS_UTILS_H_

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
int Internet_Connection_Status();



#endif /* SRC_UTILS_UTILS_H_ */
