/*
 * Lib_Com.h
 *
 *  Created on: Mar 25, 2015
 *      Author: cedric.toncanier
 */

#ifndef SRC_LIB_COM_LIB_COM_H_
#define SRC_LIB_COM_LIB_COM_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

/*******************************************
*               D E F I N E                *
********************************************/
#define SIZE_CMD_MESSAGE	5

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/
typedef enum{
	CMD_APPLI		= 0 ,
	INPUTS_STATUS	= 1 ,
}enumTypeMess;

typedef enum{
    NO_CMD            = 0,
    SOUND_CMD         = 1,
    PICTURE_CMD       = 2,
    VIDEO_CMD         = 3,
    REPORT_FILE_CMD   = 4,
    TEST_CAPTOR       = 5,
    DISCONNECT_CMD    = 6,
    STOP_CMD          = 7,
}cmdApplication;


typedef struct{
	enumTypeMess	bType;
	unsigned char	bInputs;
	float		  	fTemp;
	float		  	fHum;
	unsigned char   bsize;
	unsigned char 	bChecksum;
	char 			sMess[15];
}strInputStatus;

typedef struct{
	enumTypeMess  	bType;
	cmdApplication 	bTypeCmd;
	unsigned char 	bParam;
	unsigned char   bsize;
	unsigned char 	bChecksum;
}strCmdAppli;

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
void 			libcom_InputStatus(strInputStatus * ptr, unsigned char captor1, unsigned char captor2, unsigned char interrupter, float fTemp,float fHum);
cmdApplication 	libcom_cmdAppli(unsigned char  * cmd);

#endif /* SRC_LIB_COM_LIB_COM_H_ */
