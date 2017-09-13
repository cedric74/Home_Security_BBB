/*
 * Lib_Sonar.h
 *
 *  Created on: 2015-03-31
 *      Author: cedric
 */

#ifndef LIB_PWM_H_
#define LIB_PWM_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/

/*******************************************
*               D E F I N E                *
********************************************/
/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../Lib_Gpio/beh_BBB_gpio.h"

/*******************************************
*               D E F I N E                *
********************************************/
#define 	BUFFER_SIZE 	128

#define		PATH_P9_16			"/sys/devices/ocp.3/pwm_test_P9_16."

#define 	PATH_POLARITY  	"/polarity"
#define 	PATH_PERIOD  	"/period"
#define 	PATH_DUTY    	"/duty"
#define     PATH_RUN		"/run"


#define 	NO_ERROR_PWM	 0
#define 	ERROR_PWM_OPEN	-1
#define 	ERROR_PWM_WRITE	-2

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
int Lib_pwm_init();
int Lib_pwm_control(int iPeriod, int iDuty );
int Lib_pwm_stop();
int Lib_pwm_start();

#endif /* LIB_PWM_H_ */
