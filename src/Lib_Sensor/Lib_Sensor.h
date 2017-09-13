/*
 * Lib_Sensor.h
 *
 *  Created on: 2015-03-12
 *      Author: cedric
 */

#ifndef LIB_SENSOR_H_
#define LIB_SENSOR_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

/*******************************************
*               D E F I N E                *
********************************************/

// Define errors and return values.
#define DHT_ERROR_TIMEOUT 	-1
#define DHT_ERROR_CHECKSUM 	-2
#define DHT_ERROR_ARGUMENT 	-3
#define DHT_ERROR_GPIO 		-4
#define DHT_SUCCESS 		 0

// Define sensor types.
#define DHT11 11
#define DHT22 22
#define AM2302 22

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/
#define TICK_READ_SENSOR	30	// Each Minutes

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
float temperature;
float humidity;

pthread_t threadId_Sensor;

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
int  readSensor();

#endif /* LIB_SENSOR_H_ */
