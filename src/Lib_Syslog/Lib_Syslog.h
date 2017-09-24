/*
 * Lib_Syslog.h
 *
 *  Created on: Sep 23, 2017
 *      Author: cedrictoncanier
 */

#ifndef SRC_LIB_SYSLOG_LIB_SYSLOG_H_
#define SRC_LIB_SYSLOG_LIB_SYSLOG_H_


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

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/
/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
void  Init_Syslog();
void  Send_Syslog_To_Supervisor();
void  Syslog_Message(char *message);


#endif /* SRC_LIB_SYSLOG_LIB_SYSLOG_H_ */
