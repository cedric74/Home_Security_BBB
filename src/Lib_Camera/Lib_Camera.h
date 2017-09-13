/*
 * Lib_Camera.h
 *
 *  Created on: Feb 28, 2015
 *      Author: cedric
 */

#ifndef LIB_CAMERA_H_
#define LIB_CAMERA_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

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
void	Start_Thread_Pic();
void * 	ThreadPic();
void 	Take_Picture();
void 	Save_Pic();

#endif /* LIB_CAMERA_H_ */
