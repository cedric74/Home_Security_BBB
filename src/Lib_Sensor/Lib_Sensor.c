/*
 * Lib_Sensor.c
 *
 *  Created on: 2015-03-12
 *      Author: cedric
 */


/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <stdlib.h>

#include "Lib_Sensor.h"
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

/*
 ============================================
 Function     : readSensor()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
int readSensor()
{

	// Declarations Varaibles
	FILE *fp;
	char buffer[100];
	char ctemp[5];
	char chumi[5];
	int i;

	 /* Open the command for reading. */
	 fp = popen("/home/debian/Desktop/Adafruit_Python_DHT/examples/./AdafruitDHT.py 22 p9_11", "r");
	 if (fp == NULL) {
	   printf("Failed to run command\n" );
	   return DHT_ERROR_ARGUMENT;
	 }
	  /* Read the output a line at a time - output it. */
	  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		 // printf("%s \n", buffer);
	  }

	 if( buffer[0]=='F'){
		 //printf("ERROR DHT SENSOR \n");
		 pclose(fp);
		 return DHT_ERROR_TIMEOUT;
	 }

	  /* close */
	  pclose(fp);

	  // Get Temperature
	  char * token  = strtok( buffer, "=");
	  token = strtok( NULL, "*");
	  for(i = 0 ; i < 5 ; i++){
		  ctemp[i] = *token++;
		  //printf("cTemp[%d] : %c \n", i, ctemp[i]);
	  }

	  // Get Humidity
	  token = strtok( NULL, "=");
	  token = strtok( NULL, "%");
	  for(i = 0 ; i < 5 ; i++){
		  chumi[i] = *token++;
		  //printf("chumi[%d] : %c \n", i, chumi[i]);
	  }

	  // Save value
	  temperature = atof(ctemp);
	  humidity = atof(chumi);

	  // Print Value
	 printf(" Temp : %2.2f,", temperature);
	 printf(" Hum : %2.2f \n", humidity);

	 return DHT_SUCCESS;

}
